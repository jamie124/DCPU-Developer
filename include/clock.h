#ifndef _CLOCK_H
#define _CLOCK_H

#include <QDebug>

#include "device.h"

#include "emulator.h"

class Clock :
	public Device
{
	Q_OBJECT
public:
	Clock();
	~Clock();

	void handleInterrupt(word_vector registers);

private:
	void reset();

	int ticks, tickRate, interrupt;
	bool ticking;
};

#endif
