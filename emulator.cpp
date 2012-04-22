/**
DEmulator Emulator.
Written by James Whitwell, 2012.

Emulator emulation class
This code orginally based on dcpu-emu https://bitbucket.org/interfect/dcpu-emu

Started 7-Apr-2012 
*/

#include <iostream>
#include <stdio.h>
#include <QDebug>
#include <QFile>
#include <QByteArray>

#include "include/phrases.h"
#include "include/emulator.h"

Emulator::Emulator(QObject* parent) : QThread(parent), emulatorRunning(false)
{
	DEBUG = false;
	OPCODE_DEBUGGING = false;

	stepMode = false;

	memory = word_vector(MEMORY_LIMIT);
	registers = word_vector(NUM_REGISTERS);
	literals = word_vector(ARG_LITERAL_END - ARG_LITERAL_START);

	latestRegisters = registers_ptr(new registers_t());
	
	reset();
}

Emulator::~Emulator(void)
{
	wait();
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

void Emulator::toggleStepMode() 
{
	if (stepMode) {
		stepMode = false;
		skippingCurrentPass = false;
	} else {
		stepMode = true;
		skippingCurrentPass = true;
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
	//stepMode = false;

	// Setup literals
	//literals = new word_t[ARG_LITERAL_END - ARG_LITERAL_START];
	for (int i = 0; i < ARG_LITERAL_END - ARG_LITERAL_START; i++) {
		literals[i] = i;
	}

	//memory = new word_t[MEMORY_LIMIT];
	//memory = word_ptr(new word_t(MEMORY_LIMIT));

	// Init memory
	memory.empty();

	for (int i = 0; i < MEMORY_LIMIT; i++) {
		memory[i] = 0;
		//std::cout << &memory[i];
	}
	

	//registers = new word_t[NUM_REGISTERS];
	registers.empty();

	for (word_t i = 0; i < NUM_REGISTERS; i++) {
		registers[i] = 0;
	}

	//latestRegisters = new registers_t;

	latestRegisters->a = 0;
	latestRegisters->b = 0;
	latestRegisters->c = 0;
	latestRegisters->x = 0;
	latestRegisters->y = 0;
	latestRegisters->z = 0;
	latestRegisters->i = 0;
	latestRegisters->j = 0;
	latestRegisters->o = 0;
	latestRegisters->pc = 0;
	latestRegisters->sp = 0;

	// Colour table
	colourTable = new word_t[NUM_COLOURS];
	colourTable[0] = 0; // Black
	colourTable[1] = 4; // Blue
	colourTable[2] = 2; // Green
	colourTable[3] = 6; // Cyan
	colourTable[4] = 1; // Red
	colourTable[5] = 5; // Magenta
	colourTable[6] = 3; // Brown
	colourTable[7] = 7; // Light Gray
	colourTable[8] = 8; // Gray
	colourTable[9] = 12; // Light Blue
	colourTable[10] = 10; // Light Green
	colourTable[11] = 14; // Light Cyan
	colourTable[12] = 9; // Light Red
	colourTable[13] = 13; // Light Magenta
	colourTable[14] = 11; // Yellow
	colourTable[15] = 15; // White

	programCounter = 0;
	stackPointer = 0;
	overflow = 0;

	cycle = 0;
}

void Emulator::cleanupMemory()
{
	//delete literals;
	//delete memory;
	//delete registers;
	delete colourTable;
}

void Emulator::stopEmulator()
{
	emulatorRunning = false;
}


void Emulator::run()
{
	clearScreen();

	if (stepMode) {
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

		memory[i++] = currentWord;
	}

	program.close();


	bool videoDirty = false;

	// Start emulator loop, will continue until either finished or emulatorRunning is set to false
	while(emulatorRunning) {
		QMutexLocker lock(&mutex);

		if (!skippingCurrentPass) {

			

			word_t executingPC = programCounter;
			instruction_t instruction = memory[programCounter++];

			// Decode
			opcode_t opcode = getOpcode(instruction);
			nonbasicOpcode_t nonbasicOpcode;

			word_t* aLoc;
			word_t* bLoc;
			bool skipStore;

			if (opcode == OP_NONBASIC) {
				nonbasicOpcode = (nonbasicOpcode_t) getArgument(instruction, 0);
				aLoc = evaluateArgument(getArgument(instruction, 1));
				skipStore = 1;
			} else {
				aLoc = evaluateArgument(getArgument(instruction, 0));
				bLoc = evaluateArgument(getArgument(instruction, 1));
				skipStore = isConst(getArgument(instruction, 0));		// If literal
			}
			word_t result = 0;

			// Execute
			unsigned int resultWithCarry;		// Some opcodes use internal variable
			bool skipNext = 0;				// Skip the next instruction


			switch(opcode) {
			case OP_NONBASIC:
				skipStore = 1;

				switch(nonbasicOpcode) {
				case OP_JSR:
					// 0x01 JSR - pushes the address of next instruction onto stack.
					// Sets PC to A
					memory[--stackPointer] = programCounter;
					programCounter = *aLoc;
					cycle += 2;
					break;
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

				if (OPCODE_DEBUGGING) {
					setCursorPos(TERM_WIDTH + 6, 2);
					std::cout << "SET A to " << *bLoc;
				}
				break;

			case OP_ADD:
				// Add B to A, sets O
				result = *aLoc + *bLoc;
				overflow = (result < *aLoc || result < *bLoc);
				cycle += 2;

				if (OPCODE_DEBUGGING) {
					setCursorPos(TERM_WIDTH + 6, 2);
					std::cout << "ADD " << *bLoc << " to A";
				}
				break;

			case OP_SUB:
				// Subtracts B from a, sets O
				result = *aLoc - *bLoc;
				overflow = (result > *aLoc) ? 0xFFFF : 0;
				cycle += 2;

				if (OPCODE_DEBUGGING) {
					setCursorPos(TERM_WIDTH + 6, 2);
					std::cout << "SUB " << *bLoc << " FROM A";
				}
				break;

			case OP_MUL:
				// Multiple A by B, set O
				resultWithCarry = (unsigned int) *aLoc * (unsigned int) *bLoc;
				result = (word_t) (resultWithCarry & 0xFFFF);	// Low word
				overflow = (word_t) (resultWithCarry >> 16);	// High word
				cycle += 2;

				if (OPCODE_DEBUGGING) {
					setCursorPos(TERM_WIDTH + 6, 2);
					std::cout << "MUL A by " << *bLoc;
				}
				break;

			case OP_DIV:
				// Divide A by B, set O
				if (*bLoc != 0) {
					resultWithCarry = ((unsigned int) *aLoc << 16) / (unsigned int) *bLoc;
					result = (word_t) (resultWithCarry >> 16);		// High word
					overflow = (word_t) (resultWithCarry & 0xFFFF);	// Low word
				} else {
					result = 0;
					overflow = 0;
				}

				cycle += 3;

				if (OPCODE_DEBUGGING) {
					setCursorPos(TERM_WIDTH + 6, 2);
					std::cout << "DIV A by " << *bLoc;
				}
				break;

			case OP_MOD:
				// Remainder of A over B
				if (*bLoc != 0) {
					result = *aLoc % *bLoc;
				} else {
					result = 0;
				}

				cycle += 3;

				if (OPCODE_DEBUGGING) {
					setCursorPos(TERM_WIDTH + 6, 2);
					std::cout << "Remainder of A over  " << *bLoc;
				}
				break;

			case OP_SHL:
				// Shift A left B places, set O
				resultWithCarry = (unsigned int) *aLoc << *bLoc;
				result = (word_t) (resultWithCarry & 0xFFFF);
				overflow = (word_t) (resultWithCarry >> 16);
				cycle += 2;
				break;

			case OP_SHR:
				// Shift A right B places, set O
				resultWithCarry = (unsigned int) *aLoc >> *bLoc;
				result = (word_t) (resultWithCarry >> 16);
				overflow = (word_t) (resultWithCarry & 0xFFFF);
				cycle += 2;
				break;

			case OP_AND:
				// Binary AND of A and B
				result = *aLoc & *bLoc;
				cycle += 1;
				break;

			case OP_BOR:
				// Binary OR of A and B
				result = *aLoc | *bLoc;
				cycle += 1;
				break;

			case OP_XOR:
				// Binary XOR of A and B
				result = *aLoc ^ *bLoc;
				cycle += 1;
				break;

			case OP_IFE:
				// Skip next instruction if A != B
				skipStore = 1;
				skipNext = !!(*aLoc != *bLoc);
				cycle += (2 + skipNext);		// 2, +1 if skipped
				break;

			case OP_IFN:
				// Skip next instruction if A == B
				skipStore = 1;
				skipNext = !!(*aLoc == *bLoc);
				cycle += (2 + skipNext);
				break;

			case OP_IFG:
				// Skip next instruction if A <= B
				skipStore = 1;
				skipNext = !!(*aLoc <= *bLoc);
				cycle += (2 + skipNext);
				break;

			case OP_IFB:
				// Skip next instruction if (A & B) == 0
				skipStore = 1;
				skipNext = (!(*aLoc & *bLoc));
				cycle += (2 + skipNext);
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

			// TODO: Update video memory

			if (videoDirty) {
				//clearScreen();
				for (int i = 0; i < TERM_HEIGHT; i++) {
					for (int j = 0; j < TERM_WIDTH; j +=1) {
						word_t toPrint = memory[CONSOLE_START + i * TERM_WIDTH + j];

						setScreen(i, j, toPrint);
					}

					//std::cout << std::endl;
				}
				videoDirty = false;
			}

			if (stepMode) {

				setRegisters();
				// Sending the register updates should only be done in step mode, otherwise the 
				// GUI events will get overloaded.
				emit registersChanged(latestRegisters);

				// Skip next pass
				skippingCurrentPass = true;
			}
		}
	}

	cleanupMemory();

	emit emulationEnded(DCPU_SUCCESSFUL);
}

// Update and return the latest registers
void Emulator::setRegisters()
{
	//QMutexLocker locker(&mutex);

	//mutex.lock();

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
	latestRegisters->o = overflow;
	
	//mutex.unlock();
}

word_t* Emulator::evaluateArgument(argument_t argument)
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
		// Literal value 0-31 - does nothing on assign
		if (DEBUG) {
			std::cout << "literal " << argument - ARG_LITERAL_START << std::endl;
		}

		return &literals[argument - ARG_LITERAL_START];
	}

	// Single values
	switch(argument) {
	case ARG_POP:
		// Value at stack address, increments stack counter
		if (DEBUG) {
			std::cout << "POP" << std::endl;
		}

		return &memory[stackPointer++];
		break;

	case ARG_PEEK:
		// Value at stack address
		if (DEBUG) {
			std::cout << "PEEK" << std::endl;
		}

		return &memory[stackPointer];
		break;

	case ARG_PUSH:
		// Decreases stack address, returns value at stack address
		if (DEBUG) {
			std::cout << "PUSH" << std::endl;
		}

		return &memory[--stackPointer];
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

		return &programCounter;
		break;

	case ARG_O:
		// Overflow
		if (DEBUG) {
			std::cout << "overflow" << std::endl;
		}

		return &overflow;
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
			std::cout << memory[programCounter] << std::endl;
		}

		cycle++;
		return &memory[programCounter++];
		break;

	};
}

// Get an opcode from instruction
opcode_t Emulator::getOpcode(instruction_t instruction)
{
	return instruction & 0xF;
}

argument_t Emulator::getArgument(instruction_t instruction, bool_t which)
{
	// First 6 bits for true, second 6 for false
	return ((instruction >> 4) >> 6 * which) & 0x3F;
}

instruction_t Emulator::setOpcode(instruction_t instruction, opcode_t opcode)
{
	// Clear low 4 bits and OR in opcode
	return (instruction & 0xFFF0) | opcode; 
}

instruction_t Emulator::setArgument(instruction_t instruction, bool_t which, argument_t argument)
{
	if (!which) {
		// A argument
		return (instruction & 0xFC0F) | (((word_t) argument) << 4);
	} else {
		// B argument
		return (instruction & 0x03FF) | (((word_t) argument) << 10);		
	}
}

// Check if argument references next word
bool_t Emulator::usesNextWord(argument_t argument)
{
	return (argument >= ARG_REG_NEXTWORD_INDEX_START && argument < ARG_REG_NEXTWORD_INDEX_END)
		|| argument == ARG_NEXTWORD_INDEX
		|| argument == ARG_NEXTWORD;
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
		return 1 + usesNextWord(getArgument(instruction, 1));
	} else {
		return 1 + usesNextWord(getArgument(instruction, 0)) + usesNextWord(getArgument(instruction, 1));
	}
}

// Get offset from instruction for next word
word_t Emulator::getNextWordOffset(instruction_t instruction, bool_t which)
{
	if (getOpcode(instruction) == OP_NONBASIC) {
		// 1 argument, 1 extra word
		return (which == 0) && usesNextWord(getArgument(instruction, 1));
	} else {
		if (!usesNextWord(getArgument(instruction, which))) {
			return 0;
		}

		return 1 + (which && usesNextWord(getArgument(instruction, 0)));
	}
}

// Reverse the byte order
instruction_t Emulator::swapByteOrder(instruction_t instruction)
{
	return (instruction<<8) | (instruction>>8);
}

bool Emulator::inStepMode() 
{
	QMutexLocker locker(&mutex);

	return stepMode;
}

void Emulator::setScreen(word_t row, word_t column, word_t character)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	setCursorPos(column, row);

	/*
	word_t colourData = character >> 7;
	word_t foreColour = colourData >> 5;
	word_t backColour = (colourData >> 1) & 0xF;
	bool_t blinkBit = colourData & 0x1;
	*/
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
