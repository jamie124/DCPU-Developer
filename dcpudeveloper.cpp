#include <QDialog>
#include <QDebug>
#include <QShortcut>
#include <QKeySequence>
#include <QStringListModel>
#include <QGridLayout>
#include <QSizePolicy>
#include <QTimer>
//#include <QWeakPointer>

#include "include/dcpudeveloper.h"

#include "ui_dcpudeveloper.h"

DCPUDeveloper::DCPUDeveloper(QWidget *parent) :
QMainWindow(parent),
	ui(new Ui::DCPUDeveloper)
{
	ui->setupUi(this);
	setWindowTitle("DCPU Developer - " + VERSION_NUMBER);

	editor = new Editor;

	// Setup code completion
	completer = new QCompleter(this);
	completer->setModel(modelFromFile("wordcache.txt"));
	completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
	completer->setCompletionMode(QCompleter::PopupCompletion);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setWrapAround(true);

	editor->setCompleter(completer);
	editor->setFontPointSize(13);

	QGridLayout *editorLayout = new QGridLayout;

	editorLayout->addWidget(editor);
	ui->default_editor_tab->setLayout(editorLayout);

	memoryViewer = new MemoryViewer(&glHelper, this);

	QGridLayout *memoryLayout = new QGridLayout;

	memoryLayout->addWidget(memoryViewer);
	ui->memory_gb->setLayout(memoryLayout);

	memoryViewer->setFixedSize(500, 150);

	glHelper.setWindowSize(memoryViewer->width(), memoryViewer->height());

	// Set memory scrollbar max value
	ui->memory_scrollbar->setMaximum(glHelper.getTotalRows());
	ui->memory_scrollbar->setSingleStep(glHelper.getRowsPerWindow());

	QFile file(TEMP_FILENAME);

	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
	}

	QTextStream in(&file);
	editor->setText(in.readAll());

	file.close();

	// Setup syntax highlighting
	highlighter = new Highlighter(editor->document());

	assemblerRunning = false;
	emulatorRunning = false;
	inStepMode = false;

	//assembler = new Assembler();
	//emulator = new Emulator();
	phrases = new Phrases();

	setupConnections();
}

DCPUDeveloper::~DCPUDeveloper()
{
	delete ui;
	delete editor;

	delete phrases;

	delete memoryViewer;

	//emulator->stopEmulator();

	//delete emulator;

}

void DCPUDeveloper::setupConnections() 
{
	// Setup thread slots

	// Memory viewer
	QTimer *timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()), memoryViewer, SLOT(animate()));

	timer->start(30);
}


void DCPUDeveloper::addToCodeComplete(QString newEntry)
{
	QStringList codeList;

	if (!codeCompleteList.contains(newEntry)){
		codeCompleteList << newEntry;

		for (int i = 0; i < codeCompleteList.size(); i++) {
			codeList << codeCompleteList[i];
		}
	}
	completer->setModel(new QStringListModel(codeList, completer));
}

QAbstractItemModel* DCPUDeveloper::modelFromFile(const QString &filename)
{
	QFile file(filename);

	if (!file.open(QFile::ReadOnly)) {
		QStringList startingList;

		startingList << "Test";
		return new QStringListModel(startingList, completer);
	}

#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
	QStringList words;

	while (!file.atEnd()) {
		QByteArray line = file.readLine();

		if (!line.isEmpty()) {
			words << line.trimmed();
		}
	}

#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif
	return new QStringListModel(words, completer);

}

// Setup and new assembler thread and start it.
void DCPUDeveloper::createAndRunAssembler()
{
	ui->compile_button->setEnabled(false);

	assembler = new Assembler;

	connect(assembler, SIGNAL(sendAssemblerMessage(assembler_update_t*)), this,
		SLOT(assemblerUpdate(assembler_update_t*)));

	assembler->setFilename(TEMP_FILENAME.toStdString());

	assembler->startAssembler();
}

// Setup and new assembler thread and start it.
void DCPUDeveloper::createAndRunEmulator(QString binFile)
{
	//ui->run_button->setEnabled(false);

	emulator = new Emulator;

	//qRegisterMetaType<word_vector>();
	connect(emulator, SIGNAL(fullMemorySync(memory_array)), this, 
		SLOT(setFullMemoryBlock(memory_array)), Qt::QueuedConnection);
	connect(emulator, SIGNAL(registersChanged(registers_ptr)), this,
		SLOT(updateRegisters(registers_ptr)), Qt::BlockingQueuedConnection);
	connect(emulator, SIGNAL(emulationEnded(int)), this, SLOT(endEmulation(int)), Qt::QueuedConnection);

	emulator->setFilename(binFile);

	emulator->setStepMode(inStepMode);

	emulator->startEmulator();
}

void DCPUDeveloper::on_actionOpen_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "",
		tr("Dasm16 Source File (*.dasm16)"));

	if (filename != "") {
		QFile file(filename);

		if (!file.open(QIODevice::ReadOnly)) {
			QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
		}

		QTextStream in(&file);
		editor->setText(in.readAll());
		file.close();
	}
}

void DCPUDeveloper::on_actionExit_triggered()
{
	exit(1);
}

void DCPUDeveloper::on_compile_button_clicked()
{
	resetMessages();

	QFile file(TEMP_FILENAME);
	file.open(QIODevice::WriteOnly | QIODevice::Text);

	QTextStream stream(&file);

	stream << editor->toPlainText();

	stream.flush();
	file.close();

	appendLogMessage("File saved, starting compile.");

	createAndRunAssembler();

}

void DCPUDeveloper::appendLogMessage(QString message)
{
	ui->messages->setText(ui->messages->toPlainText() +  message + "\n");
}

void DCPUDeveloper::resetMessages()
{
	ui->messages->clear();
}

// Start or stop the emulation thread
void DCPUDeveloper::on_run_button_clicked()
{
	resetMessages();

	//emulator->setFilename(COMPILED_TEMP_FILENAME);

	if (!emulatorRunning ){
		/*
		emulator->startEmulator();

		appendLogMessage("Emulator running.");

		emulatorRunning = true;
		*/
		createAndRunEmulator(COMPILED_TEMP_FILENAME);

		emulatorRunning = true;

		ui->run_button->setText("Stop");

	} else {
		emulator->stopEmulator();

		delete emulator;

		appendLogMessage("User stopped emulator");

		emulatorRunning = false;
	}
}

void DCPUDeveloper::assemblerUpdate(assembler_update_t* error)
{
	if (error->errorCode == ASSEMBLER_SUCESSFUL) {
		ui->run_button->setEnabled(true);
		ui->compile_button->setEnabled(true);

		assembler->stopAssembler();

		delete assembler;
	} else {
		ui->run_button->setEnabled(false);
	}

	appendLogMessage(phrases->getResponseMessage(error->errorCode) + (error->lineNumber > 0 ? " at line: " + QString::number(error->lineNumber) : ""));

	delete error;
}

void DCPUDeveloper::setFullMemoryBlock(memory_array memory)
{
	QMap<int, int> temp;

	for(int i = 0; i < MEMORY_LIMIT; i++) {
		temp[i] = memory[i];
	}

	delete memory;

	memoryViewer->setMemoryMap(temp);

}

void DCPUDeveloper::updateRegisters(registers_ptr registers)
{
	ui->register_a->setValue(registers->a);
	ui->register_b->setValue(registers->b);
	ui->register_c->setValue(registers->c);

	ui->register_x->setValue(registers->x);
	ui->register_y->setValue(registers->y);
	ui->register_z->setValue(registers->z);

	ui->register_i->setValue(registers->i);
	ui->register_j->setValue(registers->j);

	ui->register_pc->setValue(registers->pc);
	ui->register_sp->setValue(registers->sp);

	ui->register_o->setValue(registers->o);
}

// Set the max memory scrollbar value
void DCPUDeveloper::updateScrollbarValue(int value)
{
	ui->memory_scrollbar->setMaximum(value);
}

void DCPUDeveloper::endEmulation(int endCode)
{
	appendLogMessage(phrases->getResponseMessage(endCode));

	ui->run_button->setEnabled(true);
	ui->run_button->setText("Run");

	emulatorRunning = false;

	emulator->stopEmulator();

	delete emulator;
}

void DCPUDeveloper::on_toggle_step_button_clicked()
{
	if (!inStepMode) {
		inStepMode = true;

		ui->toggle_step_button->setText("Disable Step Mode");
		ui->step_button->setEnabled(true);
	} else {
		inStepMode = false;

		ui->toggle_step_button->setText("Enable Step Mode");
		ui->step_button->setEnabled(false);
	}

	if (emulatorRunning) {
		emulator->toggleStepMode();
	}
}

void DCPUDeveloper::on_step_button_clicked()
{
	if (emulatorRunning){
		emulator->step();
	}
}

void DCPUDeveloper::on_actionRun_triggered()
{

}

void DCPUDeveloper::on_actionSave_triggered()
{

}

void DCPUDeveloper::on_actionCut_triggered()
{

}

void DCPUDeveloper::on_actionCopy_triggered()
{

}

void DCPUDeveloper::on_actionPaste_triggered()
{

}

void DCPUDeveloper::on_actionSelect_All_triggered()
{

}

void DCPUDeveloper::on_actionNew_triggered()
{
	editor->clear();
}

void DCPUDeveloper::on_actionAbout_triggered()
{
	QMessageBox::about(this, "About", "DCPU Developer - " + VERSION_NUMBER);

}


void DCPUDeveloper::on_memory_scrollbar_valueChanged(int value)
{
	glHelper.setRowOffset(value);
}
