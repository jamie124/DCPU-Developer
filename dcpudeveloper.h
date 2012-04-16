#ifndef DCPUDEVELOPER_H
#define DCPUDEVELOPER_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QThread>
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
    DCPUDeveloper(QWidget *parent = 0);
    ~DCPUDeveloper();
    
private slots:
    void on_actionOpen_triggered();

    void on_actionExit_triggered();

	// Emulator UI update requests
    void updateRegisters(registers_t* registers);
	void endEmulation(int endCode);

    void on_run_button_clicked();

    void on_compile_button_clicked();

	void on_toggle_step_button_clicked();

	void on_step_button_clicked();

private:
    Ui::DCPUDeveloper *ui;

    Assembler *assembler;

    Emulator *emulator;

    QString currentFilename;

    int running;

    void appendLogMessage(QString message);
};

#endif // DCPUDEVELOPER_H
