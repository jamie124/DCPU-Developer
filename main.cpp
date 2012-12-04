#include <QApplication>
#include "include/dcpudeveloper.h"
#include "include/emulator.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    //QApplication::setStyle(new QPlastiqueStyle);

	DCPUDeveloper w;
	w.show();

	return a.exec();
}
