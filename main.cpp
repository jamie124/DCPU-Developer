#include <QApplication>

#ifdef _WIN32
#include <QPlastiqueStyle>
#elif __APPLE__
#include <QMacStyle>
#endif

#include "include/dcpudeveloper.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

#ifdef _WIN32
	QApplication::setStyle(new QPlastiqueStyle);
#elif __APPLE__
	QApplication::setStyle(new QMacStyle);
#endif


	DCPUDeveloper w;
	w.show();


	return a.exec();
}
