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


Emulator::Emulator(QObject* parent) : QThread(parent), emulatorRunning(false)
{
	DEBUG = true;
	OPCODE_DEBUGGING = false;

	stepMode = false;

	//memory = word_vector(RAM_SIZE);
	registers = word_vector(NUM_REGISTERS);
	//literals = word_vector(ARG_LITERAL_END - ARG_LITERAL_START);

	//reset();
}

Emulator::~Emulator(void)
{
	memory.clear();
	//memory.squeeze();
	registers.clear();
	registers.squeeze();
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
	memory[i] = 0;
	//memory[i] = 0;
	}
	*/



	registers.clear();

	for (word_t i = 0; i < NUM_REGISTERS; i++) {
		registers.append(0);
		//registers[i] = 0;
	}


	programCounter = 0;
	stackPointer = 0;
	interruptAddress = 0;

	ex = 0;
	currentOpcode = 0;

	cycle = 0;

	connectedDevices.clear();

	//QSharedPointer<Lem> lemDevice(new Lem());

	Lem *lemDevice = new Lem(this);
	lemDevice->show();

	connectedDevices.append(lemDevice);

	//connectedDevices.append(qobject_cast<Device*>(lemDevice.data()));

	//qDebug() << QString::number(connectedDevices.size());
}

void Emulator::stopEmulator()
{
	memory.clear();
	registers.clear();
	literals.clear();

	emulatorRunning = false;

	/*
	Device *currentDevice;

	for (int i = 0; i < connectedDevices.size(); i++) {
		currentDevice = connectedDevices.at(i);

		if (currentDevice->deviceName == "lem1802") {
			//currentDevice.
		}
	}
	*/

	//this->wait();
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

	//emit fullMemorySync(memoryDump);

	bool videoDirty = false;

	// Start emulator loop, will continue until either finished or emulatorRunning is set to false
	while(emulatorRunning) {

		if (skippingCurrentPass == false) {


			word_t executingPC = programCounter;

			//qDebug() << QString::number(programCounter);

			instruction_t instruction = memory[programCounter++];

			if (stepMode) {
				emit instructionChanged(instruction);
			}

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



			/*
			argument_t temp =  ((instruction >> 4) >> 6 * 0) & 0x3E;
			argument_t temp2 =  ((instruction >> 4) >> 6 * 1);// & 0x3E;
			qDebug() << "Opcode:" << opcode << "Instruction: " << instruction << "Arg A:" << temp << "Arg B: " << temp2;

			temp =  ((instruction >> 4) >> 6 * 0) & 0x3F;
			temp2 =  ((instruction >> 4) >> 6 * 1) & 0x3F;
			qDebug() << "Opcode:" << opcode << "Instruction: " << instruction << "Arg A:" << temp << "Arg B: " << temp2;
			*/

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
					memory[--stackPointer] = programCounter;
					programCounter = *aLoc;
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
						qDebug() << QString::number(*aLoc);

						Device *device = connectedDevices.at(*aLoc);

					if (device != NULL) {

						registers[1] = device->id >> 16;
						registers[0] = device->id & 0xffff;
						registers[2] = device->version;
						registers[4] = device->manufacturer >> 16;
						registers[3] = device->manufacturer & 0xffff;
					} else {
						registers[1] = 0;
						registers[0] = 0;
						registers[2] = 0;
						registers[4] = 0;
						registers[3] = 0;
					}


					cycle += 4;
					break;

					}
				case OP_HWI:
					{
					// 0x12
					Device *device = connectedDevices.at(*aLoc);
					
					device->handleInterrupt(registers[0], registers[1]);

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
				if (aLoc == &programCounter && result == executingPC
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
				programCounter += getInstructionLength(memory[programCounter]);
			}

			/*
			if (videoDirty) {

				clearScreen();
				for (int i = 0; i < TERM_HEIGHT; i++) {
					for (int j = 0; j < TERM_WIDTH; j +=1) {

						word_t toPrint = memory[CONSOLE_START + i * TERM_WIDTH + j];

						setScreen(i, j, toPrint);
					}

				}
				videoDirty = false;

			}
			*/

			// TODO: Add a way to toggle this
			emit registersChanged(getRegisters());

			if (stepMode) {
				// Skip next pass
				skippingCurrentPass = true;
			}
		} 
	}

	emit emulationEnded(DCPU_SUCCESSFUL);
}

// Update and return the latest registers
registers_ptr Emulator::getRegisters()
{
	registers_ptr latestRegisters(new registers_t());

	latestRegisters->a = registers[0];
	latestRegisters->b = registers[1];
	latestRegisters->c = registers[2];
	latestRegisters->x = registers[3];
	latestRegisters->y = registers[4];
	latestRegisters->z = registers[5];
	latestRegisters->i = registers[6];
	latestRegisters->j = registers[7];
	latestRegisters->pc = programCounter;
	latestRegisters->sp = stackPointer;
	latestRegisters->ia = interruptAddress;
	latestRegisters->o = ex;

	return latestRegisters;
}

word_t* Emulator::evaluateArgument(argument_t argument, bool inA)
{

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
			std::cout << "[" << memory[programCounter] << " + register " << regNumber + "]" << std::endl;
		}

		cycle++;

		return &memory[registers[regNumber] + memory[programCounter++]];
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

		/*
		case ARG_PUSH:
		// Decreases stack address, returns value at stack address
		if (DEBUG) {
		std::cout << "PUSH" << std::endl;
		}

		return &memory[--stackPointer];
		break;
		*/

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

		return &programCounter;
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
			std::cout << "[" << memory[programCounter] << "]" << std::endl;
		}

		cycle++;
		return &memory[memory[programCounter++]];
		break;

	case ARG_NEXTWORD:
		// Next word of ram - literal
		if (DEBUG) {
			qDebug() << QString::number(memory[programCounter]);
		}

		cycle++;
		return &memory[programCounter++];
		break;

	};

}

// Get an opcode from instruction
opcode_t Emulator::getOpcode(instruction_t instruction)
{
	return instruction & 0x1F;
}

argument_t Emulator::getArgument(instruction_t instruction, bool_t which)
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
word_t Emulator::getInstructionLength(instruction_t instruction)
{
	if (getOpcode(instruction) == OP_NONBASIC) {
		// 1 argument
		return 1 + Utils::usesNextWord(getArgument(instruction, 1));
	} else {
		return 1 + Utils::usesNextWord(getArgument(instruction, 0)) + Utils::usesNextWord(getArgument(instruction, 1));
	}
}

// Get offset from instruction for next word
word_t Emulator::getNextWordOffset(instruction_t instruction, bool_t which)
{
	if (getOpcode(instruction) == OP_NONBASIC) {
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

void Emulator::setScreen(word_t row, word_t column, word_t character)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	setCursorPos(column, row);

	char letter = (character & 0x7F);

	if (letter == '\0') {
		letter = ' ';
	}

	//SetConsoleTextAttribute(console, 6);

	std::cout << letter;
}

void Emulator::setCursorPos(int x, int y)
{
	COORD pos  = {x, y};
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	//SetConsoleTextAttribute(console, 7);
	SetConsoleCursorPosition(console, pos);
}

void Emulator::clearScreen()
{
	COORD topLeft  = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
	SetConsoleCursorPosition(console, topLeft);
}

word_map Emulator::getMemory() {
	return memory;
}