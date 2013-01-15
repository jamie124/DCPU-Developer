/**
DCPU Emulator.
Written by James Whitwell, 2012.

DCPU emulation class
This code was started as a port of dcpu-emu https://bitbucket.org/interfect/dcpu-emu

Started 7-Apr-2012 
*/

#include <iostream>
#include <stdio.h>
#include <QDebug>
#include <QFile>
#include <QByteArray>
#include <QDataStream>

#include "include/phrases.h"
#include "include/emulator.h"
#include "include/lem.h"

#include <math.h>

Emulator::Emulator(QObject* parent) : QThread(parent), emulatorRunning(false)
{
	DEBUG = false;
	OPCODE_DEBUGGING = true;

	stepMode = false;

	triggerInterrupts = true;
	returnedFromInterrupt = false;

	connectedDevices.clear();


	Lem *lemDevice = new Lem(this);
	lemDevice->show();

	connectedDevices.append(lemDevice);

	//connectedDevices.append(qobject_cast<Device*>(lemDevice.data()));

	//qDebug() << QString::number(connectedDevices.size());
}

Emulator::~Emulator()
{
	memory.clear();
	//memory.squeeze();
	registers.clear();
	literals.clear();

	emulatorRunning = false;

	//this->wait();
}

void Emulator::setFilename(QString filename)
{
	compiledFilename = filename;
}

void Emulator::startEmulator()
{
	reset();

	qDebug() << "Starting emulator";

	emulatorRunning = true;

	start();
}

void Emulator::setStepMode(bool stepMode)
{
	this->stepMode = stepMode;
}

void Emulator::toggleStepMode() 
{
	//QMutexLocker locker(&mutex);
	if (this->stepMode) {
		this->stepMode = false;
		this->skippingCurrentPass = false;
	} else {
		this->stepMode = true;
		this->skippingCurrentPass = true;
	}
}

// Tell emulator loop to execute one more opcode
void Emulator::step()
{
	skippingCurrentPass = false;
}

// Reset the emulator
void Emulator::reset() 
{
	literals.clear();


	for (int i = 0; i < ARG_LITERAL_END - ARG_LITERAL_START; i++) {
		literals[i] = i;
	}



	memory.clear();

	/*
	for (int i = 0; i < RAM_SIZE; i++) {
	memory.insert(i, 0);
	//memory[i] = 0;
	}
	*/



	registers.clear();

	for (word_t i = 0; i < NUM_REGISTERS; i++) {
		registers.append(0);
		//registers[i] = 0;
	}


	stackPointer = 0;
	interruptAddress = 0;
	ex = 0;

	currentOpcode = 0;

	cycle = 0;

}

void Emulator::stopEmulator()
{

}

// Borrowed from https://github.com/fogleman/DCPU-16/blob/master/emulator/emulator.c
int divMod(int x, int *quo)
{
	int quotient = x / MAX_SIZE;
	if (x < 0 && x % MAX_SIZE) {
		quotient--;
	}
	*quo = quotient;
	return x % MAX_SIZE;
}

/////////////////////////////////////
// Partially rewritten with logic from 0x10c.de
/////////////////////////////////////
int Emulator::getAddress(word_t value, arg_type &argType, bool a) {
	word_t reg, address;

	argType = MEMORY;

	//qDebug() << QString::number(value);
	if (value <= ARG_REG_NEXTWORD_INDEX_END || value == ARG_PICK) {
		reg = value % 8;

		if (value == ARG_PICK) {
			reg = SP;
		} 

		if (value >= ARG_REG_START && value <= ARG_REG_END) {

			argType = REGISTER;

			address = value;

		} else if (value >= ARG_REG_INDEX_START && value <= ARG_REG_INDEX_END) {
			address = registers.at(reg);
		} else {
			if (reg < registers.size()) {
				address = (nextWord() + registers.at(reg)) & 0xffff;
			} else {
				address = 0;
			}
		}

		return address;
	}

	// Encoded literals
	if(value >= 0x20 && value <= 0x3f) {
		argType = LITERAL;

		int output = value - 0x21;

		output &= MAX_VALUE;

		return output | LITERAL_SIZE;
	}

	// Other kinds of values
	switch(value) {
		// stack pointer
	case ARG_PUSH_POP:
		if(a) {
			word_t pre = registers.at(SP);
			registers[SP] = (pre + 1) & sizeof(word_t);
			return pre;
		} else {
			word_t output = ((registers.at(SP) - 1) & sizeof(word_t));
			registers[SP] = output;
			return output;
		}
	case ARG_PEEK:
		return registers.at(SP);

		// other registers
	case ARG_SP:
		argType = REGISTER;

		return SP;
	case ARG_PC:
		argType = REGISTER;

		return PC;
	case ARG_EX:
		argType = REGISTER;

		return EX;

		// extended instruction values
	case 0x1e: // as address
		return nextWord();
	case 0x1f: // as literal
		argType = LITERAL;
		return nextWord() | LITERAL_SIZE;

	default:
		return -1;
	}
}

word_t Emulator::getWord(word_t value) {
	// TODO: Add pop

	return memory.value(value, 0);
}

word_t Emulator::nextWord() {
	word_t word = getWord(registers[PC]);

	registers[PC] = (registers[PC] + 1) & 0xffff;

	cycle++;

	return word;
}

instruction_t Emulator::nextInstruction() {
	word_t word = nextWord();

	qDebug() << "Debug instruction: " + QString::number(word, 16);
	if (stepMode) {
		emit instructionChanged(word);
	}

	word = nextWord();

	qDebug() << "Actual instruction: " + QString::number(word, 16);

	instruction_t instruction;

	// Used to debug
	instruction.rawInstruction = word;

	if ((word & 0x1f) == 0) {
		//qDebug() << "Processing just A: " + QString:: number(word);
		instruction.opcode = word & 0x3ff;
		instruction.argA = getAddress((word & 0xfc00) >> 10, instruction.argTypeA, true);

		instruction.hasB = false;
	} else {
		//qDebug() << "Processing A and B: " + QString:: number(word);
		instruction.opcode = word & 0x1f;
		instruction.argA = getAddress((word & 0xfc00) >> 10, instruction.argTypeA, true);
		instruction.argB = getAddress((word & 0x3e0) >> 5, instruction.argTypeB, false);

		instruction.hasB = true;
	}

	return instruction;
}

word_t Emulator::getSigned(word_t value) {
	word_t sign = (value << 1) & 65536;		// Move sign bit to bit 16

	return value - sign;
}

word_t Emulator::roundTowardsZero(int value) {
	if (value > 0) {
		return (word_t)floor((double)value);
	} else {
		return (word_t)ceil((double)value);
	}
}

word_t Emulator::getValue(int key, arg_type argType) {
	switch (argType) {
	case REGISTER:
		if (DEBUG) {
			qDebug() << "Checking Register for value at: " << QString::number(key);
		}
		return registers.at(key);
	case MEMORY:
		if (DEBUG) {
			qDebug() << "Checking Memory for value at: " << QString::number(key);
		}
		return memory.value(key);
	case LITERAL:
		if (key & LITERAL_SIZE) {
			return key ^ LITERAL_SIZE;
		}
		break;
	default:
		if (DEBUG) {
			qDebug() << "Key: " << QString::number(key) << " doesn't match anything";
		}

		return 0;
	}

	return 0;
}

void Emulator::setValue(word_t key, int value, arg_type argType) {
	switch (argType) {
	case REGISTER:
		if (DEBUG) {
			qDebug() << "Setting Register: " << QString::number(key) << " with value: " << QString::number(value);
		}

		if (key < NUM_REGISTERS) {
			registers.replace(key, value);

		} else {
			// Program tried to write to a register that doesn't exist.
			emit emulationEnded(DCPU_BAD_REGISTER_ACCESS);
		}
		break;

	case MEMORY:
		if (DEBUG) {
			qDebug() << "Setting Memory: " << QString::number(key) << " with value: " << QString::number(value);
		}

		if (key < RAM_SIZE) {
			memory.insert(key, value);

			emit memoryUpdated(key);
		} else {
			// Program tried to write to an area of memory that doesn't exist.
			emit emulationEnded(DCPU_BAD_MEMORY_ACCESS);
		}
		break;

	case MEMORY_OPERATION:
		if (key == PUSH) {
			word_t stackPointer = registers.at(SP) - 1 & MAX_VALUE;

			registers.replace(SP, stackPointer);

			memory.insert(stackPointer, value);
		}

		break;

	case LITERAL:
		break;

	default:
		if (DEBUG) {
			qDebug() << "Key: " << QString::number(key) << " doesn't match anything";
		}
		break;

	}
}

void Emulator::skip() {
	int storedCycle = cycle;
	int storedSP = registers.at(SP);

	nextInstruction();

	registers.replace(SP, storedSP);
	cycle = storedCycle + 1;

}

void Emulator::skipTilNonIf() {
	word_t currentMemory = getValue((registers.at(PC) & 0x1f), MEMORY);

	while (currentMemory >= 0x10 && currentMemory <= 0x17) {
		skip();
	}

	skip();
}

void Emulator::interrupt(word_t value) {
	if (!triggerInterrupts) {
		if (interruptQueue.length() >= 256) {

			emulatorRunning = false;

			emit emulationEnded(DCPU_OVERFULL_INTERRUPT_QUEUE);

		} else {
			interruptQueue.append(value);
		}
	} else {
		trigger(value);
	}
}

void Emulator::trigger(word_t value) {
	if (registers.at(IA) != 0) {
		setValue(PUSH, registers.at(PC), MEMORY_OPERATION);
		setValue(PUSH, registers.at(A), MEMORY_OPERATION);
		setValue(PC, registers.at(IA), REGISTER);
		setValue(A, value, REGISTER);
	}
}

void Emulator::run()
{

	if (this->stepMode) {
		skippingCurrentPass = true; 
	} else {
		skippingCurrentPass = false;
	}

	QFile program(compiledFilename);

	if (!program.exists()) {
		qDebug() << "File does not exist.";
	}

	if (!program.open(QIODevice::ReadOnly)) {
		qDebug() << "File could not be opened.";
	}

	QByteArray tempArray = program.readAll();


	QDataStream inputStream(&tempArray, QIODevice::ReadOnly);


	// Store stream in memory
	int i = 0;

	
	while (!inputStream.atEnd()) {
		word_t currentWord;
		inputStream >> currentWord;

		memory.insert(i++, currentWord);
		//memory[i++] = currentWord;


	}
	program.close();

	// Send the initial memory block to main thread
	//memory_array memoryDump = new int[RAM_SIZE];
	/*
	for (int j = 0; j < RAM_SIZE; j++) {
	memoryDump[j] = memory.at(j);
	}
	*/

	emit fullMemorySync(memory);

	bool videoDirty = false;

	instruction_t instruction;
	int aValue, bValue, result;

	// Start emulator loop, will continue until either finished or emulatorRunning is set to false
	while(emulatorRunning) {

		if (skippingCurrentPass == false) {

			if (returnedFromInterrupt) {
				returnedFromInterrupt = false;
			} else if (triggerInterrupts && interruptQueue.length() > 0) {
				trigger(interruptQueue.last());

				interruptQueue.removeLast();
			}

			aValue = bValue = result = 0;

			instruction = nextInstruction();

			//qDebug() << QString::number(instruction.argA);
			aValue = getValue(instruction.argA, instruction.argTypeA);

			if (instruction.hasB) {
				bValue = getValue(instruction.argB, instruction.argTypeB);
			}

			//qDebug() << "A: " + QString::number(aValue) + ", B: " + QString::number(bValue);

			switch (instruction.opcode) {
			case OP_NULL:

				emulatorRunning = false;

				if (OPCODE_DEBUGGING) {
					qDebug() << "Null opcode: " << QString::number(instruction.rawInstruction);
				}

				break;
			case OP_SET:
				// Set Stores the value of A in B.
				setValue(instruction.argB, aValue, instruction.argTypeB);

				if (OPCODE_DEBUGGING) {
					qDebug() << "SET: " << QString::number(instruction.argB) << ", Value: " << QString::number(aValue) << ", Arg Type: " << QString::number(instruction.argTypeB);
				}

				cycle += 1;

				break;
			case OP_ADD:
				// Add Stores the value of B+A in B.
				result = bValue + aValue;

				setValue(EX, (result > MAX_VALUE) ? 0x0001 : 0x0000, REGISTER);
				setValue(instruction.argB, result, instruction.argTypeB);

				if (OPCODE_DEBUGGING) {
					qDebug() << "ADD: " << QString::number(aValue) << " to " << QString::number(bValue);
				}

				cycle += 2;

				break;
			case OP_SUB:
				// Subtract Stores the value of B−A in B.
				result = bValue - aValue;

				setValue(EX, (result < 0) ? MAX_VALUE : 0x0000, REGISTER);
				setValue(instruction.argB, result, instruction.argTypeB);

				if (OPCODE_DEBUGGING) {
					qDebug() << "SUB: " << QString::number(aValue) << " from " << QString::number(bValue);
				}

				cycle += 2;

				break;

			case OP_MUL:
				// Multiply Stores the value of B*A in B.
				result = bValue * aValue;

				setValue(EX, result >> 16, REGISTER);
				setValue(instruction.argB, result, instruction.argTypeB);

				if (OPCODE_DEBUGGING) {
					qDebug() << "MUL: " << QString::number(aValue) << " to " << QString::number(bValue);
				}

				cycle += 2;

				break;

			case OP_MLI:
				// Multiply Inverse Stores the value of B*A in B.
				// A and B treated as signed
				result = getSigned(bValue) * getSigned(aValue);

				setValue(EX, result >> 16, REGISTER);
				setValue(instruction.argB, result, instruction.argTypeB);

				if (OPCODE_DEBUGGING) {
					qDebug() << "MLI: " << QString::number(aValue) << " to " << QString::number(bValue);
				}

				cycle += 2;

				break;

			case OP_DIV:
				// Divide Stores the value of B/A in B. Sets B to 0 if A is 0 (not an error).

				if (aValue == 0) {
					setValue(instruction.argB, 0x0000, instruction.argTypeB);
					setValue(EX, 0x0000, REGISTER);
				} else {
					setValue(instruction.argB, (word_t)floor((double)bValue / (double)aValue), instruction.argTypeB);
					setValue(EX, (bValue * 0x10000) / aValue, REGISTER);
				}

				if (OPCODE_DEBUGGING) {
					qDebug() << "DIV: " << QString::number(bValue) << " by " << QString::number(aValue);
				}

				cycle += 3;

				break;

			case OP_DVI:
				// Divide Inverse Stores the value of B/A in B, where A and B are treated as signed. 
				// Sets B to 0 if A is 0 (not an error). Rounds the result towards 0.

				if (aValue == 0) {
					setValue(instruction.argB, 0x0000, instruction.argTypeB);
					setValue(EX, 0x0000, REGISTER);
				} else {
					int quotient = getSigned(bValue) / getSigned(aValue);

					setValue(instruction.argB, roundTowardsZero(quotient), instruction.argTypeB);
					setValue(EX, (getSigned(bValue) << 16) / getSigned(aValue), REGISTER);
				}

				if (OPCODE_DEBUGGING) {
					qDebug() << "DVI: " << QString::number(bValue) << " by " << QString::number(aValue);
				}

				cycle += 3;

				break;

			case OP_MOD:
				// Modulo Stores the remainder of B/A in B. Sets B to 0 if A is 0 (not an error).

				setValue(instruction.argB, (aValue == 0) ? 0x0000 : (bValue % aValue), instruction.argTypeB);


				if (OPCODE_DEBUGGING) {
					qDebug() << "MOD: " << QString::number(bValue) << " by " << QString::number(aValue);
				}

				cycle += 3;

				break;

			case OP_MDI:
				// Modulo Inverse Stores the remainder of B/A in B. Sets B to 0 if A is 0 (not an error).

				setValue(instruction.argB, (aValue == 0) ? 0x0000 : (getSigned(bValue) % getSigned(aValue)), instruction.argTypeB);


				if (OPCODE_DEBUGGING) {
					qDebug() << "MDI: " << QString::number(bValue) << " by " << QString::number(aValue);
				}

				cycle += 3;

				break;

			case OP_AND:
				// AND Stores bitwise AND of B and A in B

				setValue(instruction.argB, bValue & aValue, instruction.argTypeB);

				if (OPCODE_DEBUGGING) {
					qDebug() << "AND: " << QString::number(bValue) << " by " << QString::number(aValue);
				}

				cycle += 1;

				break;

			case OP_BOR:
				// Bitwise OR Stores bitwise OR of B and A in B.

				setValue(instruction.argB, bValue | aValue, instruction.argTypeB);

				if (OPCODE_DEBUGGING) {
					qDebug() << "OR: " << QString::number(bValue) << " by " << QString::number(aValue);
				}

				cycle += 1;

				break;

			case OP_XOR:
				// Exclusive-OR Stores bitwise EXCLUSIVE OR of B and A in B.

				setValue(instruction.argB, bValue ^ aValue, instruction.argTypeB);

				if (OPCODE_DEBUGGING) {
					qDebug() << "XOR: " << QString::number(bValue) << " by " << QString::number(aValue);
				}

				cycle += 1;

				break;

			case OP_SHR:
				// Shift Right Shifts B to the right by A bits.

				setValue(EX, (bValue << 16) >> aValue, REGISTER);
				setValue(instruction.argB, bValue >> aValue, instruction.argTypeB);

				if (OPCODE_DEBUGGING) {
					qDebug() << "SHR: " << QString::number(bValue) << " by " << QString::number(aValue);
				}

				cycle += 1;

				break;

			case OP_ASR:
				// Arithmetic Shift Right Shifts B to the right by A bits. B is treated as signed.

				setValue(EX, (getSigned(bValue) << 16) >> aValue, REGISTER);
				setValue(instruction.argB, getSigned(bValue) >> aValue, instruction.argTypeB);

				if (OPCODE_DEBUGGING) {
					qDebug() << "ASR: " << QString::number(bValue) << " by " << QString::number(aValue);
				}

				cycle += 1;

				break;

			case OP_SHL:
				// Shift Left Shifts B to the left by A bits.

				setValue(EX, (bValue << aValue) >> 16, REGISTER);
				setValue(instruction.argB, bValue << aValue, instruction.argTypeB);

				if (OPCODE_DEBUGGING) {
					qDebug() << "SHL: " << QString::number(bValue) << " by " << QString::number(aValue);
				}

				cycle += 1;

				break;

			case OP_IFB:
				// If Bits[/If Blank] Perform the next instruction if B&A is not zero. 
				// In other words, if B and A have any of the same bits set, perform the next instruction.

				if ((bValue & aValue) == 0) {
					skipTilNonIf();
				}

				if (OPCODE_DEBUGGING) {
					qDebug() << "IFB: " << QString::number(bValue) << " & " << QString::number(aValue) << " == 0";
				}

				cycle += 2;
				break;

			case OP_IFC:
				// If Clear Perform the next instruction if B&A is zero. 
				// In other words, if B and A have any of the same bits set, omit the next instruction.

				if (bValue & aValue) {
					skipTilNonIf();
				}

				if (OPCODE_DEBUGGING) {
					qDebug() << "IFC: " << QString::number(bValue) << " & " << QString::number(aValue);
				}

				cycle += 2;
				break;

			case OP_IFE:
				// If Equal Perform the next instruction if B is equal to A.

				if (bValue != aValue) {
					skipTilNonIf();
				}

				if (OPCODE_DEBUGGING) {
					qDebug() << "IFE: " << QString::number(bValue) << " == " << QString::number(aValue);
				}

				cycle += 2;
				break;

			case OP_IFN:
				// If Not Equal Perform the next instruction if B is not equal to A.

				if (bValue == aValue) {
					skipTilNonIf();
				}

				if (OPCODE_DEBUGGING) {
					qDebug() << "IFN: " << QString::number(bValue) << " != " << QString::number(aValue);
				}

				cycle += 2;
				break;

			case OP_IFG:
				// If Greater Perform the next instruction if B is strictly greater than A.

				if (bValue <= aValue) {
					skipTilNonIf();
				}

				if (OPCODE_DEBUGGING) {
					qDebug() << "IFG: " << QString::number(bValue) << " >= " << QString::number(aValue);
				}

				cycle += 2;
				break;

			case OP_IFA:
				// If After Perform the next instruction if B is strictly greater than A. A and B are treated as signed.

				if (getSigned(bValue) <= getSigned(aValue)) {
					skipTilNonIf();
				}

				if (OPCODE_DEBUGGING) {
					qDebug() << "IFA: " << QString::number(bValue) << " >= " << QString::number(aValue);
				}

				cycle += 2;
				break;

			case OP_IFL:
				// If Less Perform the next instruction if B is strictly less than A.

				if (bValue >= aValue) {
					skipTilNonIf();
				}

				if (OPCODE_DEBUGGING) {
					qDebug() << "IFL: " << QString::number(bValue) << " <= " << QString::number(aValue);
				}

				cycle += 2;
				break;

			case OP_IFU:
				// If Under Perform the next instruction if B is strictly less than A. A and B are treated as signed.

				if (getSigned(bValue) >= getSigned(aValue)) {
					skipTilNonIf();
				}

				if (OPCODE_DEBUGGING) {
					qDebug() << "IFU: " << QString::number(bValue) << " <= " << QString::number(aValue);
				}

				cycle += 2;
				break;

			case OP_ADX:
				{
					// Add EX Stores the value of A+B+EX in B.

					int value = bValue + aValue + registers.at(EX);

					if (value > MAX_VALUE) {
						setValue(EX, 1, REGISTER);
					} else {
						setValue(EX, 0, REGISTER);
					}

					setValue(instruction.argB, value, instruction.argTypeB);

					if (OPCODE_DEBUGGING) {
						qDebug() << "ADX: " << QString::number(bValue) << " + " << QString::number(aValue) << QString::number(registers.at(EX));
					}

					cycle += 3;
				}
				break;

			case OP_SBX: 
				{
					// Subtract [with] EX Stores the value of A−B+EX in B.

					int value = bValue + aValue + getSigned(registers.at(EX));

					if (value < 0) {
						setValue(EX, 0xffff, REGISTER);
					} else {
						setValue(EX, 0, REGISTER);
					}

					setValue(instruction.argB, value, instruction.argTypeB);

					if (OPCODE_DEBUGGING) {
						qDebug() << "SBX: " << QString::number(bValue) << " + " << QString::number(aValue) << QString::number(getSigned(registers.at(EX)));
					}

					cycle += 3;
				}
				break;

			case OP_STI:
				// Set-Increment Stores A in B, then increases I and J by 1.

				setValue(instruction.argB, aValue, instruction.argTypeB);

				setValue(I, registers.at(I) + 1, REGISTER);
				setValue(J, registers.at(J) + 1, REGISTER);

				if (OPCODE_DEBUGGING) {
					qDebug() << "STI: " << QString::number(aValue);
				}

				cycle += 2;
				break;

			case OP_STD:
				// Set-Decrement Stores A in B, then decreases I and J by 1.

				setValue(instruction.argB, aValue, instruction.argTypeB);

				setValue(I, registers.at(I) - 1, REGISTER);
				setValue(J, registers.at(J) - 1, REGISTER);

				if (OPCODE_DEBUGGING) {
					qDebug() << "STD: " << QString::number(aValue);
				}

				cycle += 2;
				break;
			case OP_JSR:
				// Jump Subroutine Stores the address of the next instruction on the stack and begins the subroutine located at A.

				setValue(PUSH, getValue(PC, REGISTER), MEMORY_OPERATION);
				setValue(PC, aValue, REGISTER);

				if (OPCODE_DEBUGGING) {
					qDebug() << "JSR: " << QString::number(aValue);
				}

				cycle += 3;
				break;

			case OP_INT:
				// Interrupt Triggers an interrupt from software with message A.

				interrupt(aValue);

				cycle += 4;
				break;

			case OP_IAG:
				// Interrupt Address Get Sets A to IA.

				setValue(instruction.argA, registers.at(IA), instruction.argTypeA);

				if (OPCODE_DEBUGGING) {
					qDebug() << "IAG: " << QString::number(registers.at(IA));
				}

				cycle += 1;
				break;

			case OP_IAS:
				// Interrupt Address Set Sets IA to A.

				setValue(IA, aValue, REGISTER);


				if (OPCODE_DEBUGGING) {
					qDebug() << "IAS: " << QString::number(aValue);
				}

				cycle += 1;
				break;

			case OP_RFI:
				// Return From Interrupt Disables interrupt queueing, pops A from the stack, then pops PC from the stack.

				triggerInterrupts = true;
				
				setValue(A, getValue(POP, MEMORY_OPERATION), REGISTER);
				setValue(PC, getValue(POP, MEMORY_OPERATION), REGISTER);

				returnedFromInterrupt = true;

				if (OPCODE_DEBUGGING) {
					qDebug() << "RFI: ";
				}

				cycle += 3;
				break;

			case OP_IAQ:
				// Interrupt Address Queue If A is nonzero, interrupts will be added to the queue instead of triggered.
				// If A is zero, interrupts will be triggered as normal again.

				if (aValue) {
					triggerInterrupts = false;
				} else {
					triggerInterrupts = true;
				}

				if (OPCODE_DEBUGGING) {
					qDebug() << "IAQ: " << QString::number(aValue);
				}
				cycle += 2;
				break;

			case OP_HWN:
				// Hardware Number Sets A to the number of hardware devices.

				setValue(instruction.argA, connectedDevices.size(), instruction.argTypeA);

				if (OPCODE_DEBUGGING) {
					qDebug() << "HWN: " << QString::number(connectedDevices.size());
				}

				cycle += 2;
				break;

			case OP_HWQ:
				{
					// Hardware Query Sets A, B, C, X and Y registers to information about hardware A.
					// A+(B<<16) is the 32-bit hardware ID. C is the hardware version. X+(Y<<16) is the 32-bit manufacturer code.

					if (aValue < connectedDevices.size()) {
						Device *device = connectedDevices.at(aValue);

						setValue(B, device->id >> 16, REGISTER);
						setValue(A, device->id & 0xffff, REGISTER);
						setValue(C, device->version, REGISTER);
						setValue(Y, device->manufacturer >> 16, REGISTER);
						setValue(X, device->id & 0xffff, REGISTER);
					} else {
						setValue(B, 0, REGISTER);
						setValue(A, 0, REGISTER);
						setValue(C, 0, REGISTER);
						setValue(Y, 0, REGISTER);
						setValue(X, 0, REGISTER);
					}

					cycle += 4;
				}
				break;

			case OP_HWI:
				{
					// Hardware Interrupt Sends an interrupt to hardware A.

					Device *device = connectedDevices.at(aValue);

					this->disconnect(SIGNAL(memoryUpdated(word_t)));

					connect(this, SIGNAL(memoryUpdated(word_t)), device, SLOT(memoryUpdated(word_t)));

					device->handleInterrupt(registers);

					cycle += 4;
				}
				break;
			}




			if (stepMode) {
				
				emit registersChanged(getRegisters());

				// May not be a good way of doing it.
				emit fullMemorySync(memory);

				//emit instructionChanged(instruction.rawInstruction);
			}


			if (stepMode) {
				// Skip next pass
				skippingCurrentPass = true;
			}

		} 
	}

	// Show the final memory state.
	emit fullMemorySync(memory);

	qDebug() << "Emulation Ended Sucessfully";
	emit emulationEnded(DCPU_SUCCESSFUL);

}

// Update and return the latest registers
registers_ptr Emulator::getRegisters()
{
	registers_ptr latestRegisters(new registers_t());

	latestRegisters->a = registers[A];
	latestRegisters->b = registers[B];
	latestRegisters->c = registers[C];
	latestRegisters->x = registers[X];
	latestRegisters->y = registers[Y];
	latestRegisters->z = registers[Z];
	latestRegisters->i = registers[I];
	latestRegisters->j = registers[J];
	latestRegisters->sp = registers[SP];
	latestRegisters->pc = registers[PC];
	latestRegisters->o = registers[EX];

	latestRegisters->ia = interruptAddress;


	return latestRegisters;
}



// Get an opcode from instruction
word_t Emulator::getOpcode(word_t instruction)
{
	return instruction & 0x1F;
}

argument_t Emulator::getArgument(word_t instruction, bool_t which)
{
	// First 6 bits for true, second 6 for false
	//return ((instruction >> 4) >> 6 * which) & 0x3F;

	//qDebug() << instruction;
	if (which == 0){
		// Argument A
		return (instruction >> 5) & 0x1F;
	} else {
		// Argument B
		return (instruction >> 10) & 0x3F;
	}

}




// How many words does instruction take
word_t Emulator::getInstructionLength(word_t instruction)
{
	if (getOpcode(instruction) == OP_NULL) {
		// 1 argument
		return 1 + Utils::usesNextWord(getArgument(instruction, 1));
	} else {
		return 1 + Utils::usesNextWord(getArgument(instruction, 0)) + Utils::usesNextWord(getArgument(instruction, 1));
	}
}

// Get offset from instruction for next word
word_t Emulator::getNextWordOffset(word_t instruction, bool_t which)
{
	if (getOpcode(instruction) == OP_NULL) {
		// 1 argument, 1 extra word
		return (which == 0) && Utils::usesNextWord(getArgument(instruction, 1));
	} else {
		if (!Utils::usesNextWord(getArgument(instruction, which))) {
			return 0;
		}

		return 1 + (which && Utils::usesNextWord(getArgument(instruction, 0)));
	}
}

bool Emulator::inStepMode() 
{
	return stepMode;
}

word_map Emulator::getMemory() {
	return memory;
}