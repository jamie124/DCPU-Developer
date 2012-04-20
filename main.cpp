#include <QtGui/QApplication>
#include "include/dcpudeveloper.h"
#include "include/emulator.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DCPUDeveloper w;
	w.show();

	return a.exec();
}
