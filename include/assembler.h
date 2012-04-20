#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H

#include <QThread>
#include <QMutex>
#include "phrases.h"
#include "emulator.h"

const int MAX_CHARS = 1024;

typedef struct argumentStruct {
	argument_t argument;
	word_t nextWord;
	char* labelReference;		// If NULL, nextWord is valid, otherwise nextWord should point to this
	bool badArgument;
	int errorCode;
	int lineNumber;				// For debugging
} argumentStruct_t;

typedef struct assembledInstruction {
	char* label;
	word_t* data;
	word_t dataLength;
	word_t address;
	opcode_t opcode;
	argumentStruct_t a;
	argumentStruct_t b;
	struct assembledInstruction* next;
} assembledInstruction_t;

typedef struct {
	int errorCode;
	int lineNumber;
} assembler_error_t;

class Assembler : public QThread
{
	Q_OBJECT

signals:
	void sendAssemblerMessage(assembler_error_t*);

private:
	QMutex mutex;

	volatile bool assemblerRunning;
	volatile int lineNumber;

	std::string sourceFilename;

	void assemblerError(int errorCode, int lineNumber);

	opcode_t opcodeFor(char* command);
	nonbasicOpcode_t nonbasicOpcodeFor(char* command);
	int registerFor(char regName);
	argumentStruct_t argumentFor(char* arg);

	char* cleanString(char *rawLine);
	int processLine(char *currentLine, char *data, char *label, bool &functionOnNextLine, char *command, char *arg1, char *arg2, bool containsLabel);

	int processCommand(char* command, char* data, word_t &address, char* label, assembledInstruction_t *&head,  assembledInstruction_t *&tail, assembledInstruction_t *&instruction);
	void processArg1(char* command, char* arg, word_t &address, char* label, assembledInstruction_t *&instruction);
	void processArg2(char* command, char* arg, word_t &address, char* label, assembledInstruction_t *&instruction);

protected:
	void run();

public:
	explicit Assembler(QObject* parent = 0);
	~Assembler(void);

	void setFilename(std::string filename);

	void startEmulator();
	void stopEmulator();

};

#endif