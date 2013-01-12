/**
LEM 1802 Monitor

DCPU Emulator.

Written by James Whitwell, 2012.
*/
#include "device.h"
#include "lem.h"

#include <QDebug>

#include <QTimer>

#include <iostream>
#include <Windows.h>

#include <QGridLayout>

#include "ui_lem_monitor.h"

Lem::Lem(Emulator *emu, QWidget *parent) : ui(new Ui::LemMonitor)
{
	ui->setupUi(this);

	qDebug() << "Initialising LEM1802.";

	//deviceName = "lem1802";

	id = 0x7349f615;
	version = 0x1802;
	manufacturer = 0x1c6c8b36;

	screenRamAddress = 0;

	emulator = emu;

	ramSize = emulator->getMemory().size();//ramSizeValue;

	lemViewer = new LemViewer(emulator, this);

	QGridLayout *lemLayout = new QGridLayout;

	lemLayout->addWidget(lemViewer);

	ui->lem_gb->setLayout(lemLayout);


	QTimer *timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()), lemViewer, SLOT(animate()));

	timer->start(30);
}


Lem::~Lem()
{
	delete ui;

	delete lemViewer;
}


void Lem::handleInterrupt(int a, int b) {
	qDebug() << "LEM1802 received interrupt";

	switch(a) {
	case 0:
		// MEM_MAP_SCREEN

		if (b > 0) {
			screenRamAddress = b;
		} else {
			screenRamAddress = ramSize;
		}

		lemViewer->setScreenAddress(screenRamAddress);

		lemViewer->drawScreen();

		break;

	case 1:
		// MEM_MAP_FONT
		qDebug() << "Map font";
		break;

	case 2:
		// MEM_MAP_PALLET

		qDebug() << "Map pallet";

		break;

	case 3:
		// SET_BORDER_COLOR

		qDebug() << "Set border";
		break;

	case 4:
		// MEM_DUMP_FONT

		qDebug() << "Dump font";
		break;

	case 5:
		// MEM_DUMP_PALLET

		qDebug() << "Dump pallet";
		break;

	};
}

void Lem::memoryUpdated(word_t value) {
	//qDebug() << "Memory updated: " << QString::number(value);

	lemViewer->updateChar(value - screenRamAddress);

}

/*
void Lem::drawScreen() {
	word_map memory = emulator->getMemory();

	//qDebug() << QString::number(screenRamAddress);
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLUMNS; c++) {
			//lemViewer->drawChar(c, r, memory[screenRamAddress + (r * COLUMNS) + r]);
		}
	}

	//std::cout << std::endl;

	//lemViewer->drawChar(0, 0, 4);
}
*/

void Lem::setScreen(word_t row, word_t column, word_t character)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	setCursorPos(column, row);

	char letter = (character & 0x7F);

	if (letter == '\0') {
		letter = ' ';
	}

	//SetConsoleTextAttribute(console, 6);

	std::cout << letter;
}

void Lem::setCursorPos(int x, int y)
{
	COORD pos  = {x, y};
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	//SetConsoleTextAttribute(console, 7);
	SetConsoleCursorPosition(console, pos);
}

void Lem::clearScreen()
{
	COORD topLeft  = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
	SetConsoleCursorPosition(console, topLeft);
}
