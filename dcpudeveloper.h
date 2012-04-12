#ifndef DCPUDEVELOPER_H
#define DCPUDEVELOPER_H

#include <QMainWindow>
#include <string>
#include "assembler.h"

static std::string TEMP_FILENAME = "dcpu_temp.dasm16";
namespace Ui {
class DCPUDeveloper;
}

class DCPUDeveloper : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit DCPUDeveloper(QWidget *parent = 0);
    ~DCPUDeveloper();
    
private slots:
    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_compile_clicked();

private:
    Ui::DCPUDeveloper *ui;

    Assembler *assembler;

    std::string currentFilename;

};

#endif // DCPUDEVELOPER_H
