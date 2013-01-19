#include <QDialog>
#include <QDebug>
#include <QShortcut>
#include <QKeySequence>
#include <QStringListModel>
#include <QGridLayout>
#include <QSizePolicy>
#include <QTimer>

#include <QStandardItemModel>

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
	//editor->setFontPointSize(13);

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

	// Setup syntax highlighting
	highlighter = new Highlighter(editor->document());

	// Find Dialog
	findDialog = new FindDialog(this);
	findDialog->setModal(false);
	findDialog->setTextEdit(editor);

	// Find Replace Dialog
	findReplaceDialog = new FindReplaceDialog(this);
	findReplaceDialog->setModal(false);
	findReplaceDialog->setTextEdit(editor);

	assemblerRunning = false;
	emulatorRunning = false;
	inStepMode = false;

	phrases = new Phrases();

	setupConnections();

	// Load Config
	loadSettings();

	connect(ui->disassembly_list, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(disassembledRowChanged(QListWidgetItem *, QListWidgetItem *)));
	
	ui->disassembly_list->setContextMenuPolicy(Qt::ActionsContextMenu);

	
	QAction *act = new QAction(tr("Add Breakpoint"), ui->disassembly_list);
	connect(act, SIGNAL(triggered()), this, SLOT(addBreakpoint()));
	ui->disassembly_list->addAction(act);

	act = new QAction(tr("Remove Breakpoint"), ui->disassembly_list);
	connect(act, SIGNAL(triggered()), this, SLOT(removeBreakpoint()));
	ui->disassembly_list->addAction(act);

	//connect(ui->disassembly_list, SIGNAL(contextMenuEvent(QContextMenuEvent *)), this, SLOT(disassemblerContextEvent(QContextMenuEvent *)));

	// Run emulator in background.
	startEmulator();

}

DCPUDeveloper::~DCPUDeveloper()
{
	saveSettings();

	delete ui;
	delete editor;

	delete phrases;

	//delete memoryViewer;

	//delete emulator;

}

void DCPUDeveloper::setupConnections() 
{
	// Editor modified
	connect(editor, SIGNAL(textChanged()), this, SLOT(editorChanged()));

	// Add code completion 
	/*
	connect(highlighter, SIGNAL(addToCodeComplete(QString)), this, 
		SLOT(addToCodeComplete(QString)));
		*/

	// Update the code complete entries every 5 seconds
	QTimer *codeCompleteTimer = new QTimer(this);
	connect(codeCompleteTimer, SIGNAL(timeout()), this, SLOT(updateCodeComplete()));
	codeCompleteTimer->start(5000);

	// Memory viewer
	QTimer *timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()), memoryViewer, SLOT(animate()));

	timer->start(30);
}

void DCPUDeveloper::updateCodeComplete()
{
	QStringList codeList;

	QStringList splitStrings = editor->toPlainText().split(QRegExp("[\r\n]"));

	QStringList::iterator itr;

	QString currentLine;

	for (itr = splitStrings.begin(); itr != splitStrings.end(); ++itr) {
		currentLine = *itr;

		if (currentLine.contains(":")) {
			currentLine = currentLine.trimmed().replace(":", "");

			codeList.append(currentLine);
		}
	}

	completer->setModel(new QStringListModel(codeList, completer));

}

// Editor text changed
void DCPUDeveloper::editorChanged()
{
	int tabIndex = ui->editors_tabwidget->currentIndex();
	QString tabText = ui->editors_tabwidget->tabText(tabIndex);

	if (!tabText.contains("*")){
		tabText += "*";

		ui->editors_tabwidget->setTabText(tabIndex, tabText);
	}
}

void DCPUDeveloper::openFindDialog() {

}

void DCPUDeveloper::openFindReplaceDialog() {

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

void DCPUDeveloper::loadDisassemblyData() {
	QStandardItemModel *testData = new QStandardItemModel(this);

	QStandardItem *test = new QStandardItem();

	std::string debugFilename = Utils::replace(TEMP_FILENAME.toStdString(), "dasm16", "dbg");

	QFile debugFile(debugFilename.c_str());

	if (!debugFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "ERROR: Could not open debug file file ";
	}

	QTextStream debugInput(&debugFile);
	QString currentLine;
	int lineNumber = 0;
	int instruction = 0;

	QListWidgetItem *debugItem;

	int rowIndex = 0;

	ui->disassembly_list->clear();


	while (!debugInput.atEnd()) {

		currentLine = debugInput.readLine().trimmed();

		lineNumber = currentLine.left(currentLine.indexOf('|')).toInt();

		currentLine = currentLine.right(currentLine.length() - currentLine.indexOf('|') - 1);

		currentLine = currentLine.replace(":", "\t");

		debugItem = new QListWidgetItem;

		debugItem->setData(0, currentLine);
		debugItem->setData(1, lineNumber);

		ui->disassembly_list->insertItem(rowIndex++, debugItem);
	}


	ui->disassembly_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

// Setup and new assembler thread and start it.
void DCPUDeveloper::createAndRunAssembler()
{
	on_actionSave_triggered();

	ui->compile_button->setEnabled(false);

	assembler = new Assembler;

	connect(assembler, SIGNAL(sendAssemblerMessage(assembler_update_t*)), this,
		SLOT(assemblerUpdate(assembler_update_t*)));

	assembler->setFilename(TEMP_FILENAME);

	assembler->startAssembler();
}

void DCPUDeveloper::startEmulator() {
	emulator = new Emulator();

	qRegisterMetaType<word_t>("word_t");
	qRegisterMetaType<word_map>("word_map");
	
	connect(emulator, SIGNAL(fullMemorySync(word_map)), this, 
	SLOT(setFullMemoryBlock(word_map)), Qt::QueuedConnection);
	
	connect(emulator, SIGNAL(registersChanged(registers_ptr)), this,
		SLOT(updateRegisters(registers_ptr)), Qt::BlockingQueuedConnection);
	connect(emulator, SIGNAL(instructionChanged(word_t)), this, SLOT(emulatorInstructionChanged(word_t)), Qt::QueuedConnection);

	connect(emulator, SIGNAL(emulationEnded(int)), this, SLOT(endEmulation(int)), Qt::QueuedConnection);

	connect(emulator, SIGNAL(enableStepMode()), this, SLOT(enableStepMode()), Qt::QueuedConnection);

}

void DCPUDeveloper::runProgram(QString binFile) {
	emulator->setFilename(binFile);

	emulator->setStepMode(inStepMode);

	emulator->startEmulator();
}


void DCPUDeveloper::saveSettings()
{
	QSettings settings("./dcpu_developer.ini", QSettings::IniFormat);

	settings.beginGroup("files");
	settings.setValue("previous_file", currentFilename);
	settings.endGroup();
}

void DCPUDeveloper::loadSettings()
{
	QSettings settings("./dcpu_developer.ini", QSettings::IniFormat);

	settings.beginGroup("files");
	currentFilename = settings.value("previous_file").toString();
	settings.endGroup();

	if (currentFilename != "") {
		QFile file(currentFilename);

		if (!file.open(QIODevice::ReadOnly)) {
			QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
		}

		QTextStream in(&file);
		editor->setPlainText(in.readAll());

		file.close();

		ui->editors_tabwidget->setTabText(ui->editors_tabwidget->currentIndex(), currentFilename);
	}
}

void DCPUDeveloper::setSelectedDisassembedInstruction(word_t value) {
	// TODO: Add a map to prevent lookups


	bool ok;
	uint test = 0;

	word_t lineNumber = 0;
	word_t lineNumberDisassembled = value ^ 0xA000;

	QString currentLine = "";
	QListWidgetItem *item;

	int index = 0;
	for (int i = 0; i < ui->disassembly_list->count(); i++) {
		item = ui->disassembly_list->item(i);

		lineNumber = item->data(1).toInt();

		if (lineNumber == lineNumberDisassembled) {
			break;
		}

		index++;
	}

	if (index <= ui->disassembly_list->count()) {
		ui->disassembly_list->setCurrentRow(index);
	}
}

void DCPUDeveloper::resetRegisters() {
	ui->register_a->setValue(0);
	ui->register_b->setValue(0);
	ui->register_c->setValue(0);

	ui->register_x->setValue(0);
	ui->register_y->setValue(0);
	ui->register_z->setValue(0);

	ui->register_i->setValue(0);
	ui->register_j->setValue(0);

	ui->register_pc->setValue(0);
	ui->register_sp->setValue(0);
	ui->register_ia->setValue(0);

	ui->register_o->setValue(0);
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
		editor->setPlainText(in.readAll());
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

	if (!emulatorRunning ){

		resetRegisters();

		runProgram(COMPILED_TEMP_FILENAME);

		emulatorRunning = true;

		ui->run_button->setText("Stop");

	} else {
		emulator->stopEmulator();

		//delete emulator;

		appendLogMessage("User stopped program");

		emulatorRunning = false;
	}
}

void DCPUDeveloper::assemblerUpdate(assembler_update_t* error)
{
	if (error->errorCode == ASSEMBLER_SUCESSFUL) {
		ui->run_button->setEnabled(true);
		ui->compile_button->setEnabled(true);

		assembler->stopAssembler();


		loadDisassemblyData();



	} else {
		ui->run_button->setEnabled(false);
	}

	appendLogMessage(phrases->getResponseMessage(error->errorCode) + (error->lineNumber > 0 ? " at line: " + QString::number(error->lineNumber) : ""));

	delete error;
}

void DCPUDeveloper::setFullMemoryBlock(word_map memory)
{
	/*
	QMap<int, int> temp;

	for(int i = 0; i < RAM_SIZE; i++) {
		temp[i] = memory[i];
	}

	delete memory;
	*/


	memoryViewer->setMemoryMap(memory);

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
	ui->register_ia->setValue(registers->ia);

	ui->register_o->setValue(registers->o);
}

void DCPUDeveloper::emulatorInstructionChanged(word_t instruction) {
	//qDebug() << QString::number(instruction, 16);

	setSelectedDisassembedInstruction(instruction);
}

// Set the max memory scrollbar value
void DCPUDeveloper::updateScrollbarValue(int value)
{
	ui->memory_scrollbar->setMaximum(value);
}

void DCPUDeveloper::disassembledRowChanged(QListWidgetItem *currentRow, QListWidgetItem * previousRow) {
	if (currentRow != NULL) {
		editor->setLine(currentRow->data(1).toInt());
	}
}

void DCPUDeveloper::enableStepMode() {
	qDebug() << "Enabled step mode";

	on_toggle_step_button_clicked();
}

void DCPUDeveloper::disableStepMode() {
	qDebug() << "Disabled step mode";
}

void DCPUDeveloper::addBreakpoint() {
	int row = ui->disassembly_list->currentRow();

	if (row < 0) {
		return;
	}

	QListWidgetItem *item = ui->disassembly_list->item(row);

	item->setBackground(Qt::red);

	emulator->addBreakpoint(item->data(1).toInt());

	//item->setIcon(QIcon("icons/breakpoint.png"));
}

void DCPUDeveloper::removeBreakpoint() {
	int row = ui->disassembly_list->currentRow();

	if (row < 0) {
		return;
	}

	QListWidgetItem *item = ui->disassembly_list->item(row);

	item->setBackground(Qt::NoBrush);

	//item->setIcon(QIcon("icons/blank.png"));

	emulator->removeBreakpoint(item->data(1).toInt());
}

void DCPUDeveloper::endEmulation(int endCode)
{
	appendLogMessage(phrases->getResponseMessage(endCode));

	ui->run_button->setEnabled(true);
	ui->run_button->setText("Run");

	emulatorRunning = false;

	emulator->stopEmulator();

	//delete emulator;
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

void DCPUDeveloper::on_actionSave_triggered()
{
	if (currentFilename != "") {
		QFile file(currentFilename);
		if (!file.open(QIODevice::WriteOnly)) {
			qDebug() << "Could not save file";
		} else {
			QTextStream stream(&file);
			stream << editor->toPlainText();
			stream.flush();
			file.close();

			// Remove file modified indicator from tab
			ui->editors_tabwidget->setTabText(ui->editors_tabwidget->currentIndex(), currentFilename);
		}
	} else {
		// Call Save dialog
		on_actionSave_As_triggered();
	}
}

void DCPUDeveloper::on_actionSave_As_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
		tr("Dasm16 Source File (*.dasm16)"));

	if (fileName != "") {
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly)) {
			qDebug() << "Could not save file";
		} else {
			QTextStream stream(&file);
			stream << editor->toPlainText();
			stream.flush();
			file.close();

			ui->editors_tabwidget->setTabText(ui->editors_tabwidget->currentIndex(), fileName);

			currentFilename = fileName;
		}
	}
}

void  DCPUDeveloper::on_actionFind_File_triggered() {
	findDialog->show();
}

void DCPUDeveloper::on_actionFormat_Code_triggered() {
	editor->formatCode();	
}