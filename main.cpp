#include <QApplication>
#include <QPlastiqueStyle>

#include "include/dcpudeveloper.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    QApplication::setStyle(new QPlastiqueStyle);

	DCPUDeveloper w;
	w.show();


	return a.exec();
}
