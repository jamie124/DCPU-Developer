#ifndef _PHRASES_H
#define _PHRASES_H

#include <string>
#include <QString>
#include <QMap>

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

class Phrases
{

private:
	QMap<int, std::string> responses;

public:
	Phrases(void);
	~Phrases(void);

	QString getResponseMessage(int errorCode);
};

#endif