#ifndef _CONSTANTS_H
#define _CONSTANTS_H

// Return codes
static const enum { 
	// General
	SOURCE_FILE_MISSING, BIN_FILE_MISSING,
	// Emulation
	DCPU_SUCCESSFUL, DCPU_RESERVED_OPCODE,
	// Assembler
	ASSEMBLER_COMPILED, ASSEMBLER_FAILED, 
};

// Assembler errors
static const enum {
	ASSEMBLER_INVALID_REG, ASSEMBLER_UNRESOLVED_LABEL_A, ASSEMBLER_UNRESOLVED_LABEL_B
};

typedef struct {
	int errorCode;
	int lineNumber;
} assembler_error_t;

#endif