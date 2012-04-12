#include <QtGui/QApplication>
#include "dcpudeveloper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DCPUDeveloper w;
    w.show();
    
    return a.exec();
}
