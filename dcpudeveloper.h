#ifndef _DCPUDEVELOPER_H
#define _DCPUDEVELOPER_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QThread>

#include "phrases.h"
#include "assembler.h"
#include "emulator.h"
#include "highlighter.h"


static QString VERSION_NUMBER = "0.1 DEV";
static QString TEMP_FILENAME = "dcpu_temp.dasm16";
static QString COMPILED_TEMP_FILENAME = "dcpu_temp.bin";

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
	// Assembler UI update requests
	void addAssemblerMessage(assembler_error_t* error);

	// Emulator UI update requests
    void updateRegisters(registers_t* registers);
	void endEmulation(int endCode);

    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_run_button_clicked();

    void on_compile_button_clicked();

	void on_toggle_step_button_clicked();

	void on_step_button_clicked();

private:
    Ui::DCPUDeveloper *ui;

    Assembler *assembler;
    Emulator *emulator;
	Phrases *phrases;

    QString currentFilename;

    int running;

	void resetMessages();

    void appendLogMessage(QString message);

	Highlighter *highlighter;
};

#endif // DCPUDEVELOPER_H