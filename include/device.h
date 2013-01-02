#ifndef _DEVICE_H
#define _DEVICE_H

#include <QMainWindow>

class Device : public QMainWindow
{
	Q_OBJECT

public:
	explicit Device(QWidget *parent = 0);
	 ~Device();

	QString deviceName;

	int id;
	int version;
	int manufacturer;

	virtual void handleInterrupt(int a, int b) = 0;
};

#endif