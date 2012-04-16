#include <QDebug>
#include "dcpudeveloper.h"
#include "ui_dcpudeveloper.h"

DCPUDeveloper::DCPUDeveloper(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DCPUDeveloper)
{
    ui->setupUi(this);
    setWindowTitle("DCPU Developer - " + VERSION_NUMBER);

    QFile file(TEMP_FILENAME);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
    }

    QTextStream in(&file);
    ui->editor->setText(in.readAll());
    file.close();

	running = 0;

	assembler = new Assembler();
	emulator = new Emulator();
	phrases = new Phrases();

    // Setup thread slots
	// Assembler
	connect(assembler, SIGNAL(sendAssemblerMessage(assembler_error_t*)), this,
		SLOT(addAssemblerMessage(assembler_error_t*)));

	// Emulator
    connect(emulator, SIGNAL(registersChanged(registers_t*)), this,
		SLOT(updateRegisters(registers_t*)), Qt::DirectConnection);
	connect(emulator, SIGNAL(emulationEnded(int)), this, SLOT(endEmulation(int)));

}

DCPUDeveloper::~DCPUDeveloper()
{
    delete ui;
    
	assembler->stopEmulator();

	delete assembler;

	delete phrases;

	emulator->stopEmulator();

	delete emulator;

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
        ui->editor->setText(in.readAll());
        file.close();
    }
}

void DCPUDeveloper::on_actionExit_triggered()
{
    exit(1);
}

void DCPUDeveloper::on_compile_button_clicked()
{
    QFile file(TEMP_FILENAME);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream stream(&file);

    stream << ui->editor->toPlainText();

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

// Start or stop the emulation thread
void DCPUDeveloper::on_run_button_clicked()
{
    emulator->setFilename(TEMP_FILENAME.toStdString());

    if (running == 0){
		emulator->startEmulator();

        appendLogMessage("Emulator running.");

        running = 1;

        ui->run_button->setText("Stop");
    } else {
        ui->run_button->setText("Run");

		emulator->stopEmulator();

		running = 0;
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
	ui->register_pc->setValue(registers->pc);

}

void DCPUDeveloper::endEmulation(int endCode)
{
	appendLogMessage(phrases->getResponseMessage(endCode));
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
