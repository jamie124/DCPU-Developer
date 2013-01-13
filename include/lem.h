/**
LEM 1802 Monitor

DCPU Emulator.

Written by James Whitwell, 2012.
*/
#ifndef _LEM_H
#define _LEM_H

#include <QMainWindow>

#include "lemviewer.h"

#include "device.h"

#include "constants.h"
#include "emulator.h"

namespace Ui {
	class LemMonitor;
}



class Lem : public Device
{
	Q_OBJECT
public:
	Lem(Emulator *emu, QWidget *parent = 0);
	~Lem();

	void handleInterrupt(word_vector registers);

public slots:
	void memoryUpdated(word_t value);

private:
	Ui::LemMonitor *ui;

	LemViewer *lemViewer;
	Emulator *emulator;

	//void drawScreen();

	void setScreen(word_t row, word_t column, word_t character);
	void setCursorPos(int x, int y);
	void clearScreen();

	long screenRamAddress;

	long ramSize;


};

#endif