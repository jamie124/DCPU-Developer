#ifndef _DCPUDEVELOPER_H
#define _DCPUDEVELOPER_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QThread>
#include <QCompleter>
#include <QList>

#include "include/phrases.h"
#include "include/assembler.h"
#include "include/emulator.h"
#include "include/highlighter.h"
#include "include/editor.h"

static QString VERSION_NUMBER = "0.2";
static QString TEMP_FILENAME = "dcpu_temp.dasm16";
static QString COMPILED_TEMP_FILENAME = "dcpu_temp.bin";

namespace Ui {
class DCPUDeveloper;
}

class QCompleter;
class Editor;

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

    void on_actionRun_triggered();

    void on_actionSave_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionSelect_All_triggered();

    void on_actionNew_triggered();

    void on_actionAbout_triggered();

private:
    Ui::DCPUDeveloper *ui;

    Assembler *assembler;
    Emulator *emulator;
	Phrases *phrases;
	Highlighter *highlighter;

	QCompleter *completer;

	QList<QString> codeCompleteList;

	Editor *editor;

    QString currentFilename;

    int running;

	void resetMessages();
    void appendLogMessage(QString message);

	void setupConnections();

	void addToCodeComplete(QString newEntry);

	QAbstractItemModel* modelFromFile(const QString &filename);
};

#endif // DCPUDEVELOPER_H