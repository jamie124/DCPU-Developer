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
	DEBUG = true;
	OPCODE_DEBUGGING = true;

	stepMode = false;


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
			address = registers.at(value);
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
		return 'sp';
	case ARG_PC:
		return 'pc';
	case ARG_EX:
		return 'ex';

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

word_t Emulator::nextWord(bool isLiteral) {
	word_t word = getWord(registers[PC]);

	registers[PC] = (registers[PC] + 1) & 0xffff;

	cycle++;

	return word;
}

/*
word_t Emulator::nextWord(bool isLiteral) {
	word_t word = 0;
	
	if (isLiteral) {
		word = getValue(registers[PC], MEMORY);

	}

	registers[PC] = (registers[PC] + 1) & 0xffff;

	cycle++;

	return word;
}

*/

instruction_t Emulator::nextInstruction() {
	word_t word = nextWord();

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
}

void Emulator::setValue(word_t key, int value, arg_type argType) {
	switch (argType) {
	case REGISTER:
		if (DEBUG) {
			qDebug() << "Setting Register: " << QString::number(key) << " with value: " << QString::number(value);
		}
		
		if (key < registers.size()) {
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
		} else {
			// Program tried to write to an area of memory that doesn't exist.
			emit emulationEnded(DCPU_BAD_MEMORY_ACCESS);
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

		//memory[i++] = currentWord;

		memory[i++] = currentWord;

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

			aValue = bValue = result = 0;

			instruction = nextInstruction();

			//qDebug() << QString::number(instruction.argA);
			aValue = getValue(instruction.argA, instruction.argTypeA);

			if (instruction.hasB) {
				bValue = getValue(instruction.argB, instruction.argTypeB);
			}

			qDebug() << "A: " + QString::number(aValue) + ", B: " + QString::number(bValue);
			
			switch (instruction.opcode) {
			case OP_NULL:
				
				emulatorRunning = false;

				if (OPCODE_DEBUGGING) {
					qDebug() << "Null opcode.";
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
			}

			std::cout << std::endl;

			emit registersChanged(getRegisters());

			if (stepMode) {
				// May not be a good way of doing it.
				emit fullMemorySync(memory);
			}

			if (stepMode) {
				emit instructionChanged(instruction.rawInstruction);
			}

			/*
			word_t executingPC = registers.at(PC);

			//qDebug() << QString::number(registers.at(PC));

			word_t instruction = memory[registers.at(PC)++];



			//qDebug() << QString::number(instruction);

			// Decode
			opcode_t opcode = getOpcode(instruction);
			opcode_t nonbasicOpcode;

			word_t* aLoc;
			word_t* bLoc;
			bool skipStore;

			if (opcode == OP_NONBASIC) {
				nonbasicOpcode = (opcode_t) getArgument(instruction, 0);

				aLoc = evaluateArgument(getArgument(instruction, 1), false);

				skipStore = 1;

				if (DEBUG) {
					qDebug() << "NON BASIC: " << opcode << *aLoc;
				}
			} else {
				aLoc = evaluateArgument(getArgument(instruction, 0), true);
				bLoc = evaluateArgument(getArgument(instruction, 1), false);
				skipStore = isConst(getArgument(instruction, 0));		// If literal

				if (DEBUG) {
					qDebug() << "BASIC: " << opcode << *aLoc << *bLoc;
				}
			}


			word_t result = 0;

			// Execute
			unsigned int resultWithCarry;		// Some opcodes use internal variable
			bool skipNext = 0;				// Skip the next instruction
			int quo;

			switch(opcode) {
			case OP_NONBASIC:
				skipStore = 1;

				// Special Opcodes

				switch(nonbasicOpcode) {
				case OP_JSR:
					// 0x01 JSR - pushes the address of next instruction onto stack.
					// Sets PC to A
					memory[--stackPointer] = registers.at(PC);
					registers.at(PC) = *aLoc;
					cycle += 2;
					break;

				case OP_INT:
					// 0x08 INT
					cycle += 4;
					break;

				case OP_IAG:
					// 0x09 IAG
					cycle += 1;
					break;

				case OP_IAS:
					// 0x0a IAS
					cycle += 1;
					break;

				case OP_RFI:
					// 0x0b RFI
					cycle += 3;
					break;

				case OP_IAQ:
					// 0x0c IAQ
					cycle += 2;
					break;

				case OP_HWN:
					// 0x10 HWN
					registers[6] = connectedDevices.size();

					cycle += 2;
					break;

				case OP_HWQ:
					{
						// 0x11 HWQ
						//qDebug() << QString::number(*aLoc);

						Device *device = connectedDevices.at(*aLoc);

						if (device != NULL) {

							registers[B] = device->id >> 16;
							registers[A] = device->id & 0xffff;
							registers[C] = device->version;
							registers[Y] = device->manufacturer >> 16;
							registers[X] = device->manufacturer & 0xffff;
						} else {
							registers[B] = 0;
							registers[A] = 0;
							registers[C] = 0;
							registers[Y] = 0;
							registers[X] = 0;
						}


						cycle += 4;
						break;

					}
				case OP_HWI:
					{
						// 0x12
						Device *device = connectedDevices.at(*aLoc);

						device->handleInterrupt(registers[A], registers[B]);

						cycle += 4;
						break;
					}
				default:
					emit emulationEnded(DCPU_RESERVED_OPCODE);

					emulatorRunning = false;

					break;
				}
				break;

			case OP_SET:
				// Set value of A to B
				result = *bLoc;
				cycle += 1;

				break;

			case OP_ADD:
				// Add B to A, sets O
				result = divMod(*aLoc + *bLoc, &quo);
				ex = quo ? 1 : 0;
				cycle += 2;

				break;

			case OP_SUB:
				// Subtracts B from a, sets O
				result = divMod(*aLoc - *bLoc, &quo);
				ex = quo ? MAX_VALUE : 0;
				cycle += 2;

				break;

			case OP_MUL:
				// Multiple A by B, set O
				result = divMod(*aLoc * *bLoc, &quo);
				ex = quo % MAX_SIZE;
				cycle += 2;

				break;

			case OP_MLI:
				// Multiple A by B
				result = divMod((unsigned short)*aLoc * (unsigned short)*bLoc, &quo);
				ex = quo % MAX_SIZE;
				cycle += 2;

				break;

			case OP_DIV:
				// Divide A by B, set EX
				if (*bLoc != 0) {
					ex = ((*aLoc << 16) / *bLoc) % MAX_SIZE;
					result = (*aLoc / *bLoc) % MAX_SIZE;
				} else {
					result = 0;
					ex = 0;
				}

				cycle += 3;

				break;

			case OP_DVI:
				// Divide A by B, set EX. Signed
				if (*bLoc != 0) {
					ex = (((unsigned short)*aLoc << 16) / (unsigned short)*bLoc) % MAX_SIZE;
					result = (*aLoc / *bLoc) % MAX_SIZE;
				} else {
					result = 0;
					ex = 0;
				}

				cycle += 3;

				break;

			case OP_MOD:
				// Remainder of A over B
				if (*bLoc != 0) {
					result = (*aLoc % *bLoc) % MAX_SIZE;
				} else {
					result = 0;
				}

				cycle += 3;

				break;

			case OP_MDI:
				// Remainder of A over B. Signed
				if (*bLoc != 0) {
					result = ((unsigned short)*aLoc % (unsigned short)*bLoc) % MAX_SIZE;
				} else {
					result = 0;
				}

				cycle += 3;

				break;

			case OP_AND:
				// Binary AND of A and B
				result = (*aLoc & *bLoc) % MAX_SIZE;
				cycle += 1;
				break;

			case OP_BOR:
				// Binary OR of A and B
				result = (*aLoc | *bLoc) % MAX_SIZE;
				cycle += 1;
				break;

			case OP_XOR:
				// Binary XOR of A and B
				result = (*aLoc ^ *bLoc) % MAX_SIZE;
				cycle += 1;
				break;

			case OP_SHR:
				// Shift A right B places, set O
				ex = ((*aLoc << 16) >> *bLoc) % MAX_SIZE;
				result = (*aLoc >> *bLoc) % MAX_SIZE;

				cycle += 1;
				break;

			case OP_ASR:
				// Arithmetic Shift A right B places, set O
				ex = (((unsigned short)*aLoc << 16) >> *bLoc) % MAX_SIZE;
				result = ((unsigned short)*aLoc >> *bLoc) % MAX_SIZE;

				cycle += 1;
				break;

			case OP_SHL:
				// Shift A left B places, set O
				ex = ((*aLoc << *bLoc) >> 16) % MAX_SIZE;
				result = (*aLoc << *bLoc) % MAX_SIZE;

				cycle += 1;
				break;

			case OP_IFB:
				// Skip next instruction if (A & B) != 0
				skipStore = 1;
				skipNext = (*aLoc & *bLoc) != 0 ? 0 : 1;

				cycle += (2 + skipNext);
				break;

			case OP_IFC:
				// Skip next instruction if (A & B) == 0
				skipStore = 1;
				skipNext = (*aLoc & *bLoc) == 0 ? 0 : 1;

				cycle += (2 + skipNext);
				break;

			case OP_IFE:
				// Skip next instruction if A == B
				skipStore = 1;

				skipNext = (*aLoc == *bLoc) ? 0 : 1;

				//qDebug() << skipNext << argA << argB;

				cycle += (2 + skipNext);
				break;

			case OP_IFN:
				// Skip next instruction if A != B
				skipStore = 1;
				skipNext = (*aLoc != *bLoc) ? 0 : 1;

				cycle += (2 + skipNext);
				break;

			case OP_IFG:
				// Skip next instruction if A < B
				skipStore = 1;
				skipNext = (*aLoc > *bLoc) ? 0 : 1;

				cycle += (2 + skipNext);
				break;

			case OP_IFA:
				// Skip next instruction if A > B. Signed
				skipStore = 1;
				skipNext = ((unsigned short)*aLoc > (unsigned short)*bLoc) ? 0 : 1;

				cycle += (2 + skipNext);
				break;

			case OP_IFL:
				// Skip next instruction if A < B
				skipStore = 1;
				skipNext = (*aLoc < *bLoc) ? 0 : 1;

				cycle += (2 + skipNext);
				break;

			case OP_IFU:
				// Skip next instruction if A < B. Signed
				skipStore = 1;
				skipNext = ((unsigned short)*aLoc < (unsigned short)*bLoc) ? 0 : 1;

				cycle += (2 + skipNext);
				break;

			case OP_ADX:
				// Set A to A + B + EX
				skipStore = 1;
				result = divMod(*aLoc + *bLoc + ex, &quo);
				ex = quo ? 1 : 0;

				cycle += 3;
				break;

			case OP_SBX:
				// Set A to A - B + EX
				skipStore = 1;
				result = divMod(*aLoc - *bLoc + ex, &quo);
				ex = quo ? 1 : 0;

				cycle += 3;
				break;

			case OP_STI:
				// Set A to B then decrease I and J by 1
				result = *bLoc;
				registers[6] += 1;
				registers[7] += 1;

				cycle += 2;
				break;

			case OP_STD:
				// Set A to B then decrease I and J by 1
				result = *bLoc;
				registers[6] -= 1;
				registers[7] -= 1;

				cycle += 2;
				break;

			default:
				cycle += 1;
				break;
			}

			// Store result back in A, if it's not being skipped
			if (!skipStore) {
				// Halt?
				if (aLoc == &registers.at(PC) && result == executingPC
					&& ((opcode == OP_SET && getArgument(instruction, 1) == ARG_NEXTWORD)
					|| (opcode == OP_SUB && getArgument(instruction, 1) == ARG_LITERAL_START + 1))) {
						qDebug() << "SYSTEM HALTED!";

						emulatorRunning = false;
						break;
				}

				// Check if video needs to be updated
				if (aLoc >= &memory[CONSOLE_START] && aLoc < &memory[CONSOLE_END]) {
					videoDirty = true;
				}

				*aLoc = result;
			}

			// Skip next instruction if needed
			if (skipNext) {
				registers.at(PC) += getInstructionLength(memory[registers.at(PC)]);
			}

		

			// TODO: Add a way to toggle this
			//if (DEBUG) {
			emit registersChanged(getRegisters());
			//}

			*/
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

word_t* Emulator::evaluateArgument(argument_t argument, bool inA)
{
	/*
	if (argument >= ARG_REG_START && argument < ARG_REG_END) {
		// Register value
		word_t regNumber = argument - ARG_REG_START;

		if (DEBUG) {
			std::cout << "register " << regNumber << std::endl;
		}

		return &registers[regNumber];
	}

	if (argument >= ARG_REG_INDEX_START && argument < ARG_REG_INDEX_END) {
		// [register value] - Value at address in register
		word_t regNumber = argument - ARG_REG_INDEX_START;

		if (DEBUG) {
			std::cout << "[register " << regNumber << "]" << std::endl;
		}

		return &memory[registers[regNumber]];
	}

	if (argument >= ARG_REG_NEXTWORD_INDEX_START && argument < ARG_REG_NEXTWORD_INDEX_END) {
		// [next ram word + register value] - Memory address offset by register value
		word_t regNumber = argument - ARG_REG_NEXTWORD_INDEX_START;

		if (DEBUG) {
			std::cout << "[" << memory[registers.at(PC)] << " + register " << regNumber + "]" << std::endl;
		}

		cycle++;

		return &memory[registers[regNumber] + memory[registers.at(PC)++]];
	}

	if (argument >= ARG_LITERAL_START && argument < ARG_LITERAL_END) {
		if (argument == ARG_LITERAL_START) {
			return 0;
		} else {
			// Literal value 0-31 - does nothing on assign
			if (DEBUG) {
				std::cout << "literal " << argument - ARG_LITERAL_START << std::endl;
			}

			return &literals[argument - 0x21];
		}
	}

	// Single values
	switch(argument) {
	case ARG_PUSH_POP:
		// Value at stack address, increments stack counter
		if (inA){
			// Push

			if (DEBUG) {
				std::cout << "PUSH" << std::endl;
			}

			return &memory[--stackPointer];
		} else {
			// Pop

			if (DEBUG) {
				std::cout << "POP" << std::endl;
			}

			return &memory[stackPointer++];
		}
		break;

	case ARG_PEEK:
		// Value at stack address
		if (DEBUG) {
			std::cout << "PEEK" << std::endl;
		}

		return &memory[stackPointer + sizeof(word_t)];
		break;

	case ARG_PICK:
		// Value at stack address plus next word
		if (DEBUG) {
			std::cout << "PEEK" << std::endl;
		}

		return &memory[stackPointer];

		break;


	case ARG_SP:
		// Current stack pointer value
		if (DEBUG) {
			std::cout << "stack pointer" << std::endl;
		}

		return &stackPointer;
		break;

	case ARG_PC:
		// Program counter
		if (DEBUG) {
			std::cout << "program counter" << std::endl;
		}

		return &registers.at(PC);
		break;

	case ARG_EX:
		// Overflow
		if (DEBUG) {
			std::cout << "overflow" << std::endl;
		}

		return &ex;
		break;

	case ARG_NEXTWORD_INDEX:
		// Next word of ram
		if (DEBUG) {
			std::cout << "[" << memory[registers.at(PC)] << "]" << std::endl;
		}

		cycle++;
		return &memory[memory[registers.at(PC)++]];
		break;

	case ARG_NEXTWORD:
		// Next word of ram - literal
		if (DEBUG) {
			qDebug() << QString::number(memory[registers.at(PC)]);
		}

		cycle++;
		return &memory[registers.at(PC)++];
		break;

	};
	*/

	return 0;
}

// Get an opcode from instruction
opcode_t Emulator::getOpcode(word_t instruction)
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



// Is argument constant
bool_t Emulator::isConst(argument_t argument)
{
	return (argument >= ARG_LITERAL_START && argument < ARG_LITERAL_END)
		|| argument == ARG_NEXTWORD;
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