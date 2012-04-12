#include "dcpudeveloper.h"
#include "ui_dcpudeveloper.h"

DCPUDeveloper::DCPUDeveloper(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DCPUDeveloper)
{
    ui->setupUi(this);

    assembler = new Assembler();
}

DCPUDeveloper::~DCPUDeveloper()
{
    delete ui;

    delete assembler;
}

void DCPUDeveloper::on_actionOpen_triggered()
{

}

void DCPUDeveloper::on_actionExit_triggered()
{
    exit(1);
}

void DCPUDeveloper::on_compile_clicked()
{
    QFile file()
    assembler->compile(currentFilename);
}
