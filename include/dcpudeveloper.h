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
#include <QScopedPointer>

#include <QListWidget>

#include <QSettings>

#include "phrases.h"
#include "assembler.h"
#include "emulator.h"
#include "highlighter.h"
#include "editor.h"
#include "glhelper.h"
#include "memoryviewer.h"
#include "utils.h"

#include "finddialog.h"
#include "findreplacedialog.h"

static QString VERSION_NUMBER = "1.0";
static QString TEMP_FILENAME = "dcpu_temp.dasm16";
//static QString COMPILED_TEMP_FILENAME = "dcpu_temp.bin";
static QString COMPILED_TEMP_FILENAME = "debug_dcpu_temp.bin";


Q_DECLARE_METATYPE(word_vector)

namespace Ui {
class DCPUDeveloper;
}

class QCompleter;
class Editor;

class FindDialog;
class FindReplaceDialog;

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
	void setFullMemoryBlock(word_map memory);
    void updateRegisters(registers_ptr registers);
	void emulatorInstructionChanged(word_t instruction);

	void endEmulation(int endCode);

	// Highlighter requests
	void addToCodeComplete(QString newEntry, bool removing);

	// Memory Viewer timer
	void updateScrollbarValue(int value);

	void disassembledRowChanged(QListWidgetItem *currentRow, QListWidgetItem * previousRow);

	void enableStepMode();
	void disableStepMode();

	void addBreakpoint();
	void removeBreakpoint();

	void editorChanged();

	void openFindDialog();
	void openFindReplaceDialog();

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

    void on_actionSave_As_triggered();

	void on_actionFind_File_triggered();

	void on_actionFormat_Code_triggered();
private:
    Ui::DCPUDeveloper *ui;

    Assembler *assembler;
    
	Emulator *emulator;
	//QSharedPointer<Emulator> emulator;

	Phrases *phrases;
	Highlighter *highlighter;

    GLHelper glHelper;

	QCompleter *completer;

	QList<QString> codeCompleteList;

	Editor *editor;
    MemoryViewer *memoryViewer;

	FindDialog *findDialog;
	FindReplaceDialog *findReplaceDialog;

    QString currentFilename;


    bool assemblerRunning, emulatorRunning;
	bool inStepMode;

	void resetMessages();
    void appendLogMessage(QString message);

	void setupConnections();

	QAbstractItemModel* modelFromFile(const QString &filename);

	// Assembler
	void createAndRunAssembler();

	// Emulation
	void startEmulator();
	void runProgram(QString binFile);

	//void createAndRunEmulator(QString binFile);

	void loadDisassemblyData();

	void saveSettings();
	void loadSettings();

	void setSelectedDisassembedInstruction(word_t instruction);

	void resetRegisters();
};

#endif // DCPUDEVELOPER_H
