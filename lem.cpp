/**
LEM 1802 Monitor

DCPU Emulator.

Written by James Whitwell, 2012.
*/
#include "device.h"
#include "lem.h"

#include <QDebug>

#include <iostream>
#include <Windows.h>

#include <QGridLayout>

#include "ui_lem_monitor.h"

Lem::Lem(long ramSizeValue, QWidget *parent) : ui(new Ui::LemMonitor)
{
	ui->setupUi(this);

	qDebug() << "Initialising LEM1802.";

	//deviceName = "lem1802";

	id = 0x7349f615;
	version = 0x1802;
	manufacturer = 0x1c6c8b36;

	screenRamAddress = 0;

	ramSize = ramSizeValue;

	lemViewer = new LemViewer(this);

	QGridLayout *lemLayout = new QGridLayout;

	lemLayout->addWidget(lemViewer);

	ui->lem_gb->setLayout(lemLayout);


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

		drawScreen();

		break;
		
	case 1:
		// MEM_MAP_FONT
		break;

	case 2:
		// MEM_MAP_PALLET
		break;

	case 3:
		// SET_BORDER_COLOR
		break;

	case 4:
		// MEM_DUMP_FONT
		break;

	case 5:
		// MEM_DUMP_PALLET
		break;

	};
}

void Lem::drawScreen() {
	qDebug() << "Drawing screen";

	clearScreen();
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			setScreen(i, j, 'a');
		}
	}

	qDebug() << "";
}

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
