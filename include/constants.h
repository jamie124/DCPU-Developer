/**
Constants
Written by James Whitwell, 2012.

Started 7-Apr-2012 
*/

#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <QList>
#include <QHash>

typedef unsigned short word_t;
typedef unsigned char argument_t;
typedef unsigned char opcode_t;
typedef argument_t nonbasicOpcode_t;
typedef unsigned char bool_t;

typedef QList<word_t> word_vector;
typedef QHash<int, word_t> word_map;

// Registers 
static enum {
	A	= 0,
	B	= 1,
	C	= 2,
	X	= 3,
	Y	= 4,
	Z	= 5,
	I	= 6,
	J	= 7,
	SP	= 8,
	PC	= 9,
	EX	= 10,
	IA	= 11
	
} registers;

// Basic opcodes
static const enum {
	 OP_NULL		= 0x00,
	 OP_SET			= 0x01,
	 OP_ADD			= 0x02,
	 OP_SUB			= 0x03,

	 OP_MUL			= 0x04,
	 OP_MLI			= 0x05,
	 OP_DIV			= 0x06,
	 OP_DVI			= 0x07,

	 OP_MOD			= 0x08,
	 OP_MDI			= 0x09,

	 OP_AND			= 0x0a,
	 OP_BOR			= 0x0b,
	 OP_XOR			= 0x0c,
	 OP_SHR			= 0x0d,
	 OP_ASR			= 0x0e,
	 OP_SHL			= 0x0f,

	 OP_IFB			= 0x10,
	 OP_IFC			= 0x11,
	 OP_IFE			= 0x12,
	 OP_IFN			= 0x13,
	 OP_IFG			= 0x14,
	 OP_IFA			= 0x15,
	 OP_IFL			= 0x16,
	 OP_IFU			= 0x17,

	 OP_ADX			= 0x1a,
	 OP_SBX			= 0x1b,

	 OP_STI			= 0x1e,
	 OP_STD			= 0x1f
} opcodes;

// Special opcodes
static const enum {
	OP_JSR			= 0x20,

	OP_INT			= 0x100,
	OP_IAG			= 0x120,
	OP_IAS			= 0x140,
	OP_RFI			= 0x160,
	OP_IAQ			= 0x180,

	OP_HWN			= 0x200,
	OP_HWQ			= 0x220,
	OP_HWI			= 0x240,
};

// Return codes
static const enum { 
	// General
	SOURCE_FILE_MISSING				= 0x0, 
	BIN_FILE_MISSING				= 0x1,
	// Emulation
	DCPU_SUCCESSFUL					= 0x2, 
	DCPU_RESERVED_OPCODE			= 0x3, 
	DCPU_BAD_REGISTER_ACCESS		= 0x4, 
	DCPU_BAD_MEMORY_ACCESS			= 0x5, 
	DCPU_OVERFULL_INTERRUPT_QUEUE	= 0x6,
	// Assembler
	ASSEMBLER_SUCESSFUL				= 0x7, 
	ASSEMBLER_FAILED				= 0x8, 
};

// Assembler errors
static const enum {
	ASSEMBLER_EMPTY_ARG_STR			= 0x20, 
	ASSEMBLER_INVALID_LITERAL		= 0x21, 
	ASSEMBLER_INVALID_REG_NAME		= 0x22,
	ASSEMBLER_INVALID_REG			= 0x23, 
	ASSEMBLER_UNTERM_LABEL			= 0x24, 
	ASSEMBLER_UNRESOLVED_LABEL_A	= 0x25, 
	ASSEMBLER_UNRESOLVED_LABEL_B	= 0x26, 
	ASSEMBLER_EMPTY_DAT				= 0x27
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

static const word_t ARG_REG_START = 0x0;
static const word_t ARG_REG_END = 0x07;
static const word_t ARG_REG_INDEX_START = 0x08;
static const word_t ARG_REG_INDEX_END = 0x0f;
static const word_t ARG_REG_NEXTWORD_INDEX_START = 0x10;
static const word_t ARG_REG_NEXTWORD_INDEX_END = 0x17;
static const word_t ARG_PUSH_POP = 0x18;
static const word_t ARG_PEEK = 0x19;
static const word_t ARG_PICK = 0x1a;
static const word_t ARG_SP = 0x1b;
static const word_t ARG_PC = 0x1c;
static const word_t ARG_EX = 0x1d;
static const word_t ARG_NEXTWORD_INDEX = 0x1e;
static const word_t ARG_NEXTWORD = 0x1f;
static const word_t ARG_LITERAL_START = 0x20;
static const word_t ARG_LITERAL_END = 0x3f;

static const unsigned long MAX_SIZE = 0x10000;
static const word_t MAX_VALUE = 0xffff;

typedef struct {
	int errorCode;
	int lineNumber;
} assembler_error_t;

#endif
