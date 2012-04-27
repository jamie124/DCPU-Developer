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
#include <QSharedPointer>
#include <QMetaType>

#include "phrases.h"
#include "assembler.h"
#include "emulator.h"
#include "highlighter.h"
#include "editor.h"
#include "glhelper.h"
#include "memoryviewer.h"

static QString VERSION_NUMBER = "0.2";
static QString TEMP_FILENAME = "dcpu_temp.dasm16";
static QString COMPILED_TEMP_FILENAME = "dcpu_temp.bin";

Q_DECLARE_METATYPE(word_vector)

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
	void assemblerUpdate(assembler_update_t* error);

	// Emulator UI update requests
	void setFullMemoryBlock(memory_array memory);
    void updateRegisters(registers_ptr registers);
	void endEmulation(int endCode);

	// Memory Viewer timer
	void updateScrollbarValue(int value);

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

    void on_memory_scrollbar_valueChanged(int value);

private:
    Ui::DCPUDeveloper *ui;

    Assembler *assembler;
    Emulator *emulator;
	Phrases *phrases;
	Highlighter *highlighter;

    GLHelper glHelper;

	QCompleter *completer;

	QList<QString> codeCompleteList;

	Editor *editor;
    MemoryViewer *memoryViewer;

    QString currentFilename;

    bool assemblerRunning, emulatorRunning;

	void resetMessages();
    void appendLogMessage(QString message);

	void setupConnections();

	void addToCodeComplete(QString newEntry);

	QAbstractItemModel* modelFromFile(const QString &filename);

	void createAndRunAssembler();
	void createAndRunEmulator(QString binFile);
};

#endif // DCPUDEVELOPER_H
