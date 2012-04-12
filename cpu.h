/**
DCPU Emulator.
Written by James Whitwell, 2012.

CPU emulation class
This code orginally based on dcpu-emu https://bitbucket.org/interfect/dcpu-emu

Started 7-Apr-2012 
*/
#pragma once

#include <iostream>
#include <Windows.h>

typedef unsigned short word_t;
typedef word_t instruction_t;

typedef unsigned char argument_t;
typedef unsigned char opcode_t;
typedef argument_t nonbasicOpcode_t;

typedef unsigned char bool_t;

static const int OP_NONBASIC = 0;
static const int OP_SET = 1;
static const int OP_ADD = 2;
static const int OP_SUB = 3;
static const int OP_MUL = 4;
static const int OP_DIV = 5;
static const int OP_MOD = 6;
static const int OP_SHL = 7;
static const int OP_SHR = 8;
static const int OP_AND = 9;
static const int OP_BOR = 10;
static const int OP_XOR = 11;
static const int OP_IFE = 12;
static const int OP_IFN = 13;
static const int OP_IFG = 14;
static const int OP_IFB = 15;

static const int OP_JSR = 1;

static const word_t ARG_REG_START = 0;
static const word_t ARG_REG_END = 8;
static const word_t ARG_REG_INDEX_START = 8;
static const word_t ARG_REG_INDEX_END = 16;
static const word_t ARG_REG_NEXTWORD_INDEX_START = 16;
static const word_t ARG_REG_NEXTWORD_INDEX_END = 24;
static const word_t ARG_POP = 24;
static const word_t ARG_PEEK = 25;
static const word_t ARG_PUSH = 26;
static const word_t ARG_SP = 27;
static const word_t ARG_PC = 28;
static const word_t ARG_O = 29;
static const word_t ARG_NEXTWORD_INDEX = 30;
static const word_t ARG_NEXTWORD = 31;
static const word_t ARG_LITERAL_START = 32;
static const word_t ARG_LITERAL_END = 64;

const long MEMORY_LIMIT = 0x10000;
const int NUM_REGISTERS = 8;

const int TERM_WIDTH = 32;
const int TERM_HEIGHT = 16;
const long CONSOLE_START = 0x8000;
const long CONSOLE_END = (CONSOLE_START + TERM_WIDTH + TERM_HEIGHT);

// Keyboard input
const long KEYBOARD_ADDRESS = 0x9000;
const int KEYBOARD_BUFFER_LENGTH = 1;

const int FRAMESKIP = 10;

const int NUM_COLOURS = 16;

class Cpu
{
private:
	bool DEBUG;
	bool OPCODE_DEBUGGING;

	bool STEP_MODE;
	bool RUNNING;

	word_t* evaluateArgument(argument_t argument);

	opcode_t getOpcode(instruction_t instruction);
	argument_t getArgument(instruction_t instruction, bool_t which);

	bool_t isConst(argument_t argument);
	word_t getInstructionLength(instruction_t instruction);
	word_t getNextWordOffset(instruction_t instruction, bool_t which);
public:
	Cpu(void);
	~Cpu(void);

	int run(std::string filename);

	static bool_t usesNextWord(argument_t argument);
	static instruction_t setOpcode(instruction_t instruction, opcode_t opcode);
	static instruction_t setArgument(instruction_t instruction, bool_t which, argument_t argument);

	void setScreen(word_t row, word_t column, word_t character);
	void setCursorPos(int x, int y);
	void clearScreen();
};

