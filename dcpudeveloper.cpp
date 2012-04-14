#include "dcpudeveloper.h"
#include "ui_dcpudeveloper.h"

DCPUDeveloper::DCPUDeveloper(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DCPUDeveloper)
{
    ui->setupUi(this);
    setWindowTitle("DCPU Developer - " + VERSION_NUMBER);

    assembler = new Assembler();

    QFile file(TEMP_FILENAME);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
    }

    QTextStream in(&file);
    ui->editor->setText(in.readAll());
    file.close();

    // Emulator isn't running by default
    emulatorRunning = false;

    // Setup thread slots
    QObject::connect(&emulator, SIGNAL(registersChanged(registers_t*)), this,
                     SLOT(updateRegisters(registers_t*)));
}

DCPUDeveloper::~DCPUDeveloper()
{
    delete ui;

    delete assembler;

    emulator.quit();
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

void DCPUDeveloper::on_compile_clicked()
{
    QFile file(TEMP_FILENAME);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream stream(&file);

    stream << ui->editor->toPlainText();

    stream.flush();
    file.close();


    appendLogMessage("File saved, starting compile.");

    assembler->compile(TEMP_FILENAME.toStdString());
}

void DCPUDeveloper::appendLogMessage(QString message)
{
    ui->messages->setText(ui->messages->toPlainText() +  message + "\n");
}

// Start or stop the emulation thread
void DCPUDeveloper::on_run_clicked()
{
    emulator.setFilename(TEMP_FILENAME.toStdString());

    if (!emulatorRunning){
        emulator.start();

        emulatorRunning = true;

        ui->run_button->setText("Stop");
    } else {
        emulator.quit();

        emulatorRunning = false;

        ui->run_button->setText("Run");
    }
}

void DCPUDeveloper::updateRegisters(registers_t* registers)
{
    ui->test->setText(QString::number(registers->a));
}
