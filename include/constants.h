/**
Constants
Written by James Whitwell, 2012.

Started 7-Apr-2012 
*/

#ifndef _CONSTANTS_H
#define _CONSTANTS_H

typedef unsigned short word_t;
typedef word_t instruction_t;
typedef unsigned char argument_t;
typedef unsigned char opcode_t;
typedef argument_t nonbasicOpcode_t;
typedef unsigned char bool_t;

// Basic opcodes
static const enum {
	 OP_NONBASIC,
	 OP_SET,
	 OP_ADD,
	 OP_SUB,

	 OP_MUL,
	 OP_MLI,
	 OP_DIV,
	 OP_DVI,

	 OP_MOD,
	 OP_MDI,

	 OP_AND,
	 OP_BOR,
	 OP_XOR,
	 OP_SHR,
	 OP_ASR,
	 OP_SHL,

	 OP_IFB,
	 OP_IFC,
	 OP_IFE,
	 OP_IFN,
	 OP_IFG,
	 OP_IFA,
	 OP_IFL,
	 OP_IFU,

	 OP_ADX,
	 OP_SBX,

	 OP_STI,
	 OP_STD
};

// Special opcodes
static const enum {
	OP_JSR,

	OP_INT,
	OP_IAG,
	OP_IAS,
	OP_RFI,
	OP_IAQ,

	OP_HWN,
	OP_HWQ,
	OP_HWI
};

// Return codes
static const enum { 
	// General
	SOURCE_FILE_MISSING, BIN_FILE_MISSING,
	// Emulation
	DCPU_SUCCESSFUL, DCPU_RESERVED_OPCODE,
	// Assembler
	ASSEMBLER_SUCESSFUL, ASSEMBLER_FAILED, 
};

// Assembler errors
static const enum {
	ASSEMBLER_EMPTY_ARG_STR, ASSEMBLER_INVALID_LITERAL, ASSEMBLER_INVALID_REG_NAME,
	ASSEMBLER_INVALID_REG, ASSEMBLER_UNTERM_LABEL, ASSEMBLER_UNRESOLVED_LABEL_A, ASSEMBLER_UNRESOLVED_LABEL_B
};

// Registers
static const word_t REG_A = 0x0;
static const word_t REG_B = 0x1;
static const word_t REG_C = 0x2;
static const word_t REG_X = 0x3;
static const word_t REG_Y = 0x4;
static const word_t REG_Z = 0x5;
static const word_t REG_I = 0x6;
static const word_t REG_J = 0x7;

// Fake Registers
static const word_t PUSH = 0x18;
static const word_t POP = 0x18;
static const word_t PEEK = 0x19;

// Special registers
static const word_t REG_SP = 0x1b;
static const word_t REG_PC = 0x1c;
static const word_t REG_EX = 0x1d;
static const word_t REG_IA = 0xff;

static const word_t ARG_REG_START = 0;
static const word_t ARG_REG_END = 8;
static const word_t ARG_REG_INDEX_START = 8;
static const word_t ARG_REG_INDEX_END = 16;
static const word_t ARG_REG_NEXTWORD_INDEX_START = 16;
static const word_t ARG_REG_NEXTWORD_INDEX_END = 24;
static const word_t ARG_PUSH_POP = 24;
static const word_t ARG_PEEK = 25;
static const word_t ARG_PICK = 26;
static const word_t ARG_SP = 27;
static const word_t ARG_PC = 28;
static const word_t ARG_EX = 29;
static const word_t ARG_NEXTWORD_INDEX = 30;
static const word_t ARG_NEXTWORD = 31;
static const word_t ARG_LITERAL_START = 32;
static const word_t ARG_LITERAL_END = 64;

static const unsigned long MAX_SIZE = 0x10000;
static const word_t MAX_VALUE = 0xffff;

typedef struct {
	int errorCode;
	int lineNumber;
} assembler_error_t;

#endif