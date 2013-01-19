#include <QApplication>
#include <QPlastiqueStyle>
#include <QMacStyle>

#include "include/dcpudeveloper.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    QApplication::setStyle(new QMacStyle);

	DCPUDeveloper w;
	w.show();


	return a.exec();
}
