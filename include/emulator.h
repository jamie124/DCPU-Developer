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

#include <iostream>

//#include <Windows.h>

#include "constants.h"
#include "utils.h"

// Devices
#include "device.h"
//#include "lem.h"


const long RAM_SIZE = 0x10000;
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

typedef struct {
    int a, b, c, x, y, z, i, j, o;
    long pc, sp, ia;
	word_t opcode;
} registers_t;

// Pointer typedefs
typedef QSharedPointer<registers_t> registers_ptr;
typedef QVector<word_t> word_vector;
typedef QMap<int, word_t> word_map;
typedef int* memory_array;

class Emulator : public QThread
{
    Q_OBJECT

signals:
	void fullMemorySync(memory_array);
    void registersChanged(registers_ptr);
	void instructionChanged(word_t);

	void emulationEnded(int);

private:

	bool DEBUG;
	bool OPCODE_DEBUGGING;

	volatile bool skippingCurrentPass;
    volatile bool stepMode;
    volatile bool emulatorRunning;

    QString compiledFilename;

	word_t* evaluateArgument(argument_t argument, bool inA);

	opcode_t getOpcode(instruction_t instruction);
	argument_t getArgument(instruction_t instruction, bool_t which);

	bool_t isConst(argument_t argument);
	word_t getInstructionLength(instruction_t instruction);
	word_t getNextWordOffset(instruction_t instruction, bool_t which);

	registers_ptr getRegisters();

	void reset();

protected:
	void run();

public:
   explicit Emulator(QObject* parent = 0);
    ~Emulator(void);

    void setFilename(QString filename);

    void startEmulator();
	void stopEmulator();

	bool inStepMode();

	void setStepMode(bool stepMode);
	void toggleStepMode();

	void step();

	void setScreen(word_t row, word_t column, word_t character);
	void setCursorPos(int x, int y);
	void clearScreen();

	word_map getMemory();
private:
	//word_t* memory;
	word_map memory;
	word_vector registers;
	word_map literals;
	//word_t* colourTable;

	word_t programCounter;
	word_t stackPointer;
	word_t ex;
	word_t interruptAddress;

	word_t cycle;
	word_t currentOpcode;

	//word_t keyboardPosition;
	QVector<Device*> connectedDevices;
};

#endif
