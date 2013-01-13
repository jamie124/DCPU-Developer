#include "clock.h"


Clock::Clock()
{
	qDebug() << "Intialising Clock.";

	id = 0x12d0b402;
	version = 1;
	manufacturer = 0;

	reset();
}


Clock::~Clock()
{
}

void Clock::handleInterrupt(word_vector registers) {
	switch (registers.at(A)) {
	case 0:
		if (registers.at(B)) {
			ticks = 0;
			tickRate = registers.at(B);
		} else {
			tickRate = 0;
		}
		break;

	case 1:
		break;
	}
}

void Clock::reset() {
	ticks = 0;
	tickRate = 1;
	ticking = false;
	interrupt = 0;
}