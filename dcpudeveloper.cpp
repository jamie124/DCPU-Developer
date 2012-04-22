#include <QDialog>
#include <QDebug>
#include <QShortcut>
#include <QKeySequence>
#include <QStringListModel>
#include <QGridLayout>
#include <QSizePolicy>
#include <QTimer>

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

	running = 0;

	assembler = new Assembler();
	emulator = new Emulator();
	phrases = new Phrases();

	setupConnections();
}

DCPUDeveloper::~DCPUDeveloper()
{
	delete ui;
	delete editor;
	//delete highlighter;

	assembler->stopEmulator();

	delete assembler;

	delete phrases;

	emulator->stopEmulator();

	delete emulator;

}

void DCPUDeveloper::setupConnections() 
{
	// Setup thread slots
	// Assembler
	connect(assembler, SIGNAL(sendAssemblerMessage(assembler_error_t*)), this,
		SLOT(addAssemblerMessage(assembler_error_t*)));

	// Emulator
	connect(emulator, SIGNAL(registersChanged(registers_t*)), this,
		SLOT(updateRegisters(registers_t*)), Qt::DirectConnection);
	connect(emulator, SIGNAL(emulationEnded(int)), this, SLOT(endEmulation(int)), Qt::QueuedConnection);

	// Memory viewer
	QTimer *timer = new QTimer(this);

	// TEMPORARY
	QMap<int, int> tempMap;

	tempMap[0] = 1;
	tempMap[1] = 2;
	tempMap[2] = 3;
	tempMap[3] = 4;
	tempMap[4] = 5;
	tempMap[5] = 6;
	tempMap[6] = 7;
	tempMap[7] = 8;
	tempMap[8] = 9;
	tempMap[9] = 10;
	tempMap[10] = 11;
	tempMap[11] = 12;
	tempMap[12] = 13;

	memoryViewer->setMemoryMap(tempMap);

	connect(timer, SIGNAL(timeout()), memoryViewer, SLOT(animate()));
	connect(timer, SIGNAL(setScrollbarValue(int)), memoryViewer, SLOT(updateScrollbarValue(int)));

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

	assembler->setFilename(TEMP_FILENAME.toStdString());

	assembler->startEmulator();
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

	emulator->setFilename(COMPILED_TEMP_FILENAME);

	if (running == 0){
		emulator->startEmulator();

		appendLogMessage("Emulator running.");

		running = 1;

		ui->run_button->setText("Stop");
	} 
}

void DCPUDeveloper::addAssemblerMessage(assembler_error_t* error)
{
	if (error->errorCode == ASSEMBLER_SUCESSFUL) {
		ui->run_button->setEnabled(true);
	} else {
		ui->run_button->setEnabled(false);
	}

	appendLogMessage(phrases->getResponseMessage(error->errorCode) + (error->lineNumber > 0 ? " at line: " + QString::number(error->lineNumber) : ""));

	delete error;
}

void DCPUDeveloper::updateRegisters(registers_t* registers)
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

	ui->run_button->setText("Run");

	running = 0;
}

void DCPUDeveloper::on_toggle_step_button_clicked()
{
	emulator->toggleStepMode();

	if (emulator->inStepMode()) {
		ui->toggle_step_button->setText("Disable Step Mode");
		ui->step_button->setEnabled(true);
	} else {
		ui->toggle_step_button->setText("Enable Step Mode");
		ui->step_button->setEnabled(false);
	}
}

void DCPUDeveloper::on_step_button_clicked()
{
	emulator->step();
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
