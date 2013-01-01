#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H

#include <QThread>
#include <QString>

#include "constants.h"
#include "utils.h"
#include "phrases.h"

const int MAX_CHARS = 1024;

typedef struct argumentStruct {
	argument_t argument;
	word_t nextWord;
	QString labelReference;		// If NULL, nextWord is valid, otherwise nextWord should point to this
	bool badArgument;
	int errorCode;
	int lineNumber;				// For debugging
} argumentStruct_t;

typedef struct assembledInstruction {
	QString label;
	word_t* data;
	int dataLength;
	word_t address;
	opcode_t opcode;
	argumentStruct_t a;
	argumentStruct_t b;
	struct assembledInstruction* next;
} assembledInstruction_t;

typedef struct {
	int errorCode;
	int lineNumber;
} assembler_update_t;

class Assembler : public QThread
{
	Q_OBJECT

signals:
	void sendAssemblerMessage(assembler_update_t*);

private:

	volatile bool assemblerRunning;
	volatile int lineNumber;

	QString sourceFilename;

	void assemblerError(int errorCode, int lineNumber);

	opcode_t opcodeFor(const QString command);
	nonbasicOpcode_t nonbasicOpcodeFor(QString command);

	int registerFor(char regName);
	argumentStruct_t argumentFor(QString arg);

	char* cleanString(char *rawLine);

	void removeComment(QString &input);

	int processLine(const QString currentLine, QString &data, QString &label, bool &functionOnNextLine, QString &command, QString &arg1, QString &arg2, bool containsLabel);

	int processCommand(QString &command, QString &data, word_t &address, QString &label, assembledInstruction_t *&head,  assembledInstruction_t *&tail, assembledInstruction_t *&instruction);
	void processArg1(QString &command, QString &arg, word_t &address, QString &label, assembledInstruction_t *&instruction);
	void processArg2(QString &command, QString &arg, word_t &address, QString &label, assembledInstruction_t *&instruction);

protected:
	void run();

public:
	explicit Assembler(QObject* parent = 0);
	~Assembler(void);

	void setFilename(const QString filename);

	void startAssembler();
	void stopAssembler();

};

#endif