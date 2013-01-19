/**
DCPU Emulator.
Written by James Whitwell, 2012.

DCPU emulation class
This code was started as a port of dcpu-emu https://bitbucket.org/interfect/dcpu-emu

Started 7-Apr-2012 
*/
#ifndef _EMULATOR_H
#define _EMULATOR_H

#include <QThread>
#include <QString>
#include <QSharedPointer>
#include <QVector>

#include <QMap>
#include <QHash>
#include <QMutex>

#include <iostream>

#include "constants.h"
#include "utils.h"

// Devices
#include "device.h"

const long LITERAL_SIZE = 0x10000;
//const long MAX_VALUE = 65535;

const long RAM_SIZE = 0x10000;
const int NUM_REGISTERS = 12;

const int TERM_WIDTH = 32;
const int TERM_HEIGHT = 16;
const long CONSOLE_START = 0x8000;
const long CONSOLE_END = (CONSOLE_START + TERM_WIDTH + TERM_HEIGHT);

// Keyboard input
//const long KEYBOARD_ADDRESS = 0x9000;
//const int KEYBOARD_BUFFER_LENGTH = 1;

const int FRAMESKIP = 10;

const int NUM_COLOURS = 16;

typedef struct {
    int a, b, c, x, y, z, i, j, o;
    long pc, sp, ia;
	word_t opcode;
} registers_t;

// Pointer typedefs
typedef QSharedPointer<registers_t> registers_ptr;
//typedef QList<word_t> word_vector;
//typedef QHash<int, word_t> word_map;
typedef int* memory_array;

// Type of an argument.
typedef enum {
	REGISTER,
	LITERAL,
	MEMORY,
	MEMORY_OPERATION			// Special operation, Peek, Pick, etc.
	
} arg_type;

typedef struct {
	word_t rawInstruction;

	word_t opcode;
	bool hasB;

	int argA;
	arg_type argTypeA;

	int argB;
	arg_type argTypeB;

} instruction_t;


class Emulator : public QThread
{
    Q_OBJECT

signals:
	void fullMemorySync(word_map);
    void registersChanged(registers_ptr);
	void instructionChanged(word_t);

	void memoryUpdated(word_t);

	void emulationEnded(int);

	void enableStepMode();
	void disableStepMode();
private:

	bool DEBUG;
	bool OPCODE_DEBUGGING;

	volatile bool skippingCurrentPass;
    volatile bool stepMode;
    volatile bool emulatorRunning;

	volatile bool hitBreakpoint;

    QString compiledFilename;

	// Process instructions
	int getAddress(word_t value, arg_type &argType, bool a = false);

	word_t getWord(word_t value);
	word_t nextWord();
	instruction_t nextInstruction();

	word_t getSigned(word_t value);

	word_t roundTowardsZero(int value);

	// Get a value from memory or register
	word_t getValue(int key, arg_type argType);

	// Set a value to memory or register
	void setValue(word_t key, int value, arg_type argType);

	void skip();
	void skipTilNonIf();

	void interrupt(word_t value);

	void trigger(word_t value);

	// Deprecated shit
	word_t* evaluateArgument(argument_t argument, bool inA);

	word_t getOpcode(word_t instruction);
	argument_t getArgument(word_t instruction, bool_t which);

	bool_t isConst(argument_t argument);
	word_t getInstructionLength(word_t instruction);
	word_t getNextWordOffset(word_t instruction, bool_t which);

	registers_ptr getRegisters();

	void reset();

protected:
	void run();

public:
   explicit Emulator(QObject* parent = 0);
    ~Emulator();

    void setFilename(QString filename);

    void startEmulator();
	void stopEmulator();

	bool inStepMode();

	void setStepMode(bool stepMode);
	void toggleStepMode();

	void step();

	word_map getMemory();

	// Control breakpoints
	void addBreakpoint(int lineNumber);
	void removeBreakpoint(int lineNumber);

private:
	word_map memory;
	word_vector registers;
	word_map literals;

	word_vector interruptQueue;

	bool triggerInterrupts, returnedFromInterrupt;

	//word_t programCounter;
	word_t stackPointer;
	word_t ex;
	word_t interruptAddress;

	word_t cycle;
	word_t currentOpcode;

	//word_t keyboardPosition;
	QVector<Device*> connectedDevices;

	word_t lastInstruction;

	QVector<int> breakpoints;

	QMutex mutex;
};

#endif
