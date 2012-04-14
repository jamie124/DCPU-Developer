#ifndef DCPUDEVELOPER_H
#define DCPUDEVELOPER_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include "assembler.h"
#include "emulator.h"

static QString VERSION_NUMBER = "0.1 DEV";
static QString TEMP_FILENAME = "dcpu_temp.dasm16";

namespace Ui {
class DCPUDeveloper;
}

class DCPUDeveloper : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit DCPUDeveloper(QWidget *parent = 0);
    ~DCPUDeveloper();
    
private slots:
    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_compile_clicked();

    void on_run_clicked();

    void updateRegisters(registers_t* registers);

private:
    Ui::DCPUDeveloper *ui;

    Assembler *assembler;
    Emulator emulator;

    QString currentFilename;

    bool emulatorRunning;

    void appendLogMessage(QString message);

};

#endif // DCPUDEVELOPER_H
