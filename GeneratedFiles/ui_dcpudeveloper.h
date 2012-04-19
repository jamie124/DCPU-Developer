/********************************************************************************
** Form generated from reading UI file 'dcpudeveloper.ui'
**
** Created: Thu Apr 19 23:01:52 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DCPUDEVELOPER_H
#define UI_DCPUDEVELOPER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DCPUDeveloper
{
public:
    QAction *actionExit;
    QAction *actionNew;
    QAction *actionSave;
    QAction *actionOpen;
    QAction *actionAbout;
    QAction *actionOptions;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionSelect_All;
    QAction *actionFind_Selection;
    QAction *actionFind_File;
    QAction *actionRun;
    QWidget *centralWidget;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout;
    QGroupBox *messages_gb;
    QGridLayout *gridLayout_5;
    QTextEdit *messages;
    QGroupBox *program_control_gb;
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout_2;
    QLabel *cycle_label;
    QPushButton *compile_button;
    QPushButton *reset_button;
    QPushButton *view_screen_button;
    QPushButton *run_button;
    QPushButton *step_button;
    QPushButton *toggle_step_button;
    QGroupBox *registers_gb;
    QGridLayout *gridLayout_3;
    QLabel *label;
    QSpinBox *register_a;
    QLabel *label_5;
    QSpinBox *register_y;
    QLabel *label_9;
    QSpinBox *register_pc;
    QLabel *label_2;
    QSpinBox *register_b;
    QLabel *label_7;
    QSpinBox *register_z;
    QLabel *label_10;
    QSpinBox *register_sp;
    QLabel *label_3;
    QSpinBox *register_c;
    QLabel *label_6;
    QSpinBox *register_i;
    QLabel *label_11;
    QSpinBox *register_o;
    QLabel *label_4;
    QSpinBox *register_x;
    QLabel *label_8;
    QSpinBox *register_j;
    QLabel *test;
    QGroupBox *disassembly_gb;
    QGridLayout *gridLayout_4;
    QListView *disassembly;
    QGroupBox *editor_gb;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuTools;
    QMenu *menuEdit;
    QMenu *menuFind;
    QMenu *menuProgram;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DCPUDeveloper)
    {
        if (DCPUDeveloper->objectName().isEmpty())
            DCPUDeveloper->setObjectName(QString::fromUtf8("DCPUDeveloper"));
        DCPUDeveloper->resize(1146, 602);
        actionExit = new QAction(DCPUDeveloper);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionNew = new QAction(DCPUDeveloper);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionSave = new QAction(DCPUDeveloper);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionOpen = new QAction(DCPUDeveloper);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionAbout = new QAction(DCPUDeveloper);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionOptions = new QAction(DCPUDeveloper);
        actionOptions->setObjectName(QString::fromUtf8("actionOptions"));
        actionUndo = new QAction(DCPUDeveloper);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        actionRedo = new QAction(DCPUDeveloper);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        actionCut = new QAction(DCPUDeveloper);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionCopy = new QAction(DCPUDeveloper);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        actionPaste = new QAction(DCPUDeveloper);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionSelect_All = new QAction(DCPUDeveloper);
        actionSelect_All->setObjectName(QString::fromUtf8("actionSelect_All"));
        actionFind_Selection = new QAction(DCPUDeveloper);
        actionFind_Selection->setObjectName(QString::fromUtf8("actionFind_Selection"));
        actionFind_File = new QAction(DCPUDeveloper);
        actionFind_File->setObjectName(QString::fromUtf8("actionFind_File"));
        actionRun = new QAction(DCPUDeveloper);
        actionRun->setObjectName(QString::fromUtf8("actionRun"));
        centralWidget = new QWidget(DCPUDeveloper);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_6 = new QGridLayout(centralWidget);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        messages_gb = new QGroupBox(centralWidget);
        messages_gb->setObjectName(QString::fromUtf8("messages_gb"));
        messages_gb->setMinimumSize(QSize(0, 50));
        gridLayout_5 = new QGridLayout(messages_gb);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        messages = new QTextEdit(messages_gb);
        messages->setObjectName(QString::fromUtf8("messages"));

        gridLayout_5->addWidget(messages, 0, 0, 1, 1);


        gridLayout->addWidget(messages_gb, 1, 0, 1, 1);

        program_control_gb = new QGroupBox(centralWidget);
        program_control_gb->setObjectName(QString::fromUtf8("program_control_gb"));
        gridLayout_7 = new QGridLayout(program_control_gb);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        cycle_label = new QLabel(program_control_gb);
        cycle_label->setObjectName(QString::fromUtf8("cycle_label"));

        gridLayout_2->addWidget(cycle_label, 0, 0, 1, 1);

        compile_button = new QPushButton(program_control_gb);
        compile_button->setObjectName(QString::fromUtf8("compile_button"));

        gridLayout_2->addWidget(compile_button, 1, 0, 1, 1);

        reset_button = new QPushButton(program_control_gb);
        reset_button->setObjectName(QString::fromUtf8("reset_button"));
        reset_button->setEnabled(false);

        gridLayout_2->addWidget(reset_button, 3, 0, 1, 1);

        view_screen_button = new QPushButton(program_control_gb);
        view_screen_button->setObjectName(QString::fromUtf8("view_screen_button"));

        gridLayout_2->addWidget(view_screen_button, 4, 0, 1, 1);

        run_button = new QPushButton(program_control_gb);
        run_button->setObjectName(QString::fromUtf8("run_button"));
        run_button->setEnabled(false);

        gridLayout_2->addWidget(run_button, 1, 1, 1, 1);

        step_button = new QPushButton(program_control_gb);
        step_button->setObjectName(QString::fromUtf8("step_button"));
        step_button->setEnabled(false);

        gridLayout_2->addWidget(step_button, 4, 1, 1, 1);

        toggle_step_button = new QPushButton(program_control_gb);
        toggle_step_button->setObjectName(QString::fromUtf8("toggle_step_button"));
        toggle_step_button->setEnabled(false);

        gridLayout_2->addWidget(toggle_step_button, 3, 1, 1, 1);


        gridLayout_7->addLayout(gridLayout_2, 0, 0, 1, 1);


        gridLayout->addWidget(program_control_gb, 1, 1, 1, 1);

        registers_gb = new QGroupBox(centralWidget);
        registers_gb->setObjectName(QString::fromUtf8("registers_gb"));
        gridLayout_3 = new QGridLayout(registers_gb);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label = new QLabel(registers_gb);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        register_a = new QSpinBox(registers_gb);
        register_a->setObjectName(QString::fromUtf8("register_a"));
        register_a->setReadOnly(true);
        register_a->setMaximum(999);

        gridLayout_3->addWidget(register_a, 0, 1, 1, 1);

        label_5 = new QLabel(registers_gb);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_3->addWidget(label_5, 0, 2, 1, 1);

        register_y = new QSpinBox(registers_gb);
        register_y->setObjectName(QString::fromUtf8("register_y"));
        register_y->setMaximum(999);

        gridLayout_3->addWidget(register_y, 0, 3, 1, 1);

        label_9 = new QLabel(registers_gb);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_3->addWidget(label_9, 0, 4, 1, 1);

        register_pc = new QSpinBox(registers_gb);
        register_pc->setObjectName(QString::fromUtf8("register_pc"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(register_pc->sizePolicy().hasHeightForWidth());
        register_pc->setSizePolicy(sizePolicy);
        register_pc->setMaximum(999999999);

        gridLayout_3->addWidget(register_pc, 0, 5, 1, 1);

        label_2 = new QLabel(registers_gb);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 1, 0, 1, 1);

        register_b = new QSpinBox(registers_gb);
        register_b->setObjectName(QString::fromUtf8("register_b"));
        register_b->setMaximum(999);

        gridLayout_3->addWidget(register_b, 1, 1, 1, 1);

        label_7 = new QLabel(registers_gb);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_3->addWidget(label_7, 1, 2, 1, 1);

        register_z = new QSpinBox(registers_gb);
        register_z->setObjectName(QString::fromUtf8("register_z"));
        register_z->setMaximum(999);

        gridLayout_3->addWidget(register_z, 1, 3, 1, 1);

        label_10 = new QLabel(registers_gb);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_3->addWidget(label_10, 1, 4, 1, 1);

        register_sp = new QSpinBox(registers_gb);
        register_sp->setObjectName(QString::fromUtf8("register_sp"));
        register_sp->setMaximum(999999999);

        gridLayout_3->addWidget(register_sp, 1, 5, 1, 1);

        label_3 = new QLabel(registers_gb);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_3->addWidget(label_3, 2, 0, 1, 1);

        register_c = new QSpinBox(registers_gb);
        register_c->setObjectName(QString::fromUtf8("register_c"));
        register_c->setMaximum(999);

        gridLayout_3->addWidget(register_c, 2, 1, 1, 1);

        label_6 = new QLabel(registers_gb);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_3->addWidget(label_6, 2, 2, 1, 1);

        register_i = new QSpinBox(registers_gb);
        register_i->setObjectName(QString::fromUtf8("register_i"));
        register_i->setMaximum(999);

        gridLayout_3->addWidget(register_i, 2, 3, 1, 1);

        label_11 = new QLabel(registers_gb);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_3->addWidget(label_11, 2, 4, 1, 1);

        register_o = new QSpinBox(registers_gb);
        register_o->setObjectName(QString::fromUtf8("register_o"));
        register_o->setMaximum(999);

        gridLayout_3->addWidget(register_o, 2, 5, 1, 1);

        label_4 = new QLabel(registers_gb);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_3->addWidget(label_4, 3, 0, 1, 1);

        register_x = new QSpinBox(registers_gb);
        register_x->setObjectName(QString::fromUtf8("register_x"));
        register_x->setMaximum(999);

        gridLayout_3->addWidget(register_x, 3, 1, 1, 1);

        label_8 = new QLabel(registers_gb);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_3->addWidget(label_8, 3, 2, 1, 1);

        register_j = new QSpinBox(registers_gb);
        register_j->setObjectName(QString::fromUtf8("register_j"));
        register_j->setMaximum(999);

        gridLayout_3->addWidget(register_j, 3, 3, 1, 1);

        test = new QLabel(registers_gb);
        test->setObjectName(QString::fromUtf8("test"));

        gridLayout_3->addWidget(test, 3, 4, 1, 1);


        gridLayout->addWidget(registers_gb, 1, 2, 1, 1);

        disassembly_gb = new QGroupBox(centralWidget);
        disassembly_gb->setObjectName(QString::fromUtf8("disassembly_gb"));
        gridLayout_4 = new QGridLayout(disassembly_gb);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        disassembly = new QListView(disassembly_gb);
        disassembly->setObjectName(QString::fromUtf8("disassembly"));
        disassembly->setMinimumSize(QSize(300, 0));

        gridLayout_4->addWidget(disassembly, 0, 0, 1, 1);


        gridLayout->addWidget(disassembly_gb, 0, 0, 1, 1);

        editor_gb = new QGroupBox(centralWidget);
        editor_gb->setObjectName(QString::fromUtf8("editor_gb"));
        editor_gb->setMinimumSize(QSize(800, 500));

        gridLayout->addWidget(editor_gb, 0, 1, 1, 2);


        gridLayout_6->addLayout(gridLayout, 0, 0, 1, 1);

        DCPUDeveloper->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DCPUDeveloper);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1146, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuFind = new QMenu(menuEdit);
        menuFind->setObjectName(QString::fromUtf8("menuFind"));
        menuProgram = new QMenu(menuBar);
        menuProgram->setObjectName(QString::fromUtf8("menuProgram"));
        DCPUDeveloper->setMenuBar(menuBar);
        statusBar = new QStatusBar(DCPUDeveloper);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        DCPUDeveloper->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuProgram->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionAbout);
        menuTools->addAction(actionOptions);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionSelect_All);
        menuEdit->addSeparator();
        menuEdit->addAction(menuFind->menuAction());
        menuFind->addAction(actionFind_Selection);
        menuFind->addAction(actionFind_File);
        menuProgram->addAction(actionRun);

        retranslateUi(DCPUDeveloper);

        QMetaObject::connectSlotsByName(DCPUDeveloper);
    } // setupUi

    void retranslateUi(QMainWindow *DCPUDeveloper)
    {
        DCPUDeveloper->setWindowTitle(QApplication::translate("DCPUDeveloper", "DCPU Developer", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("DCPUDeveloper", "Exit", 0, QApplication::UnicodeUTF8));
        actionExit->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("DCPUDeveloper", "New", 0, QApplication::UnicodeUTF8));
        actionNew->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("DCPUDeveloper", "Save", 0, QApplication::UnicodeUTF8));
        actionSave->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("DCPUDeveloper", "Open", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("DCPUDeveloper", "About", 0, QApplication::UnicodeUTF8));
        actionOptions->setText(QApplication::translate("DCPUDeveloper", "Options", 0, QApplication::UnicodeUTF8));
        actionUndo->setText(QApplication::translate("DCPUDeveloper", "Undo", 0, QApplication::UnicodeUTF8));
        actionUndo->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        actionRedo->setText(QApplication::translate("DCPUDeveloper", "Redo", 0, QApplication::UnicodeUTF8));
        actionRedo->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+Y", 0, QApplication::UnicodeUTF8));
        actionCut->setText(QApplication::translate("DCPUDeveloper", "Cut", 0, QApplication::UnicodeUTF8));
        actionCut->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        actionCopy->setText(QApplication::translate("DCPUDeveloper", "Copy", 0, QApplication::UnicodeUTF8));
        actionCopy->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        actionPaste->setText(QApplication::translate("DCPUDeveloper", "Paste", 0, QApplication::UnicodeUTF8));
        actionPaste->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        actionSelect_All->setText(QApplication::translate("DCPUDeveloper", "Select All", 0, QApplication::UnicodeUTF8));
        actionSelect_All->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+A", 0, QApplication::UnicodeUTF8));
        actionFind_Selection->setText(QApplication::translate("DCPUDeveloper", "Find Selection", 0, QApplication::UnicodeUTF8));
        actionFind_File->setText(QApplication::translate("DCPUDeveloper", "Find in File", 0, QApplication::UnicodeUTF8));
        actionFind_File->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+F", 0, QApplication::UnicodeUTF8));
        actionRun->setText(QApplication::translate("DCPUDeveloper", "Run", 0, QApplication::UnicodeUTF8));
        actionRun->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        messages_gb->setTitle(QApplication::translate("DCPUDeveloper", "Messages", 0, QApplication::UnicodeUTF8));
        program_control_gb->setTitle(QApplication::translate("DCPUDeveloper", "Program Control", 0, QApplication::UnicodeUTF8));
        cycle_label->setText(QApplication::translate("DCPUDeveloper", "Cycle: 0", 0, QApplication::UnicodeUTF8));
        compile_button->setText(QApplication::translate("DCPUDeveloper", "Compile", 0, QApplication::UnicodeUTF8));
        reset_button->setText(QApplication::translate("DCPUDeveloper", "Reset", 0, QApplication::UnicodeUTF8));
        view_screen_button->setText(QApplication::translate("DCPUDeveloper", "View Screen", 0, QApplication::UnicodeUTF8));
        run_button->setText(QApplication::translate("DCPUDeveloper", "Run", 0, QApplication::UnicodeUTF8));
        step_button->setText(QApplication::translate("DCPUDeveloper", "Step", 0, QApplication::UnicodeUTF8));
        toggle_step_button->setText(QApplication::translate("DCPUDeveloper", "Enable Step Mode", 0, QApplication::UnicodeUTF8));
        registers_gb->setTitle(QApplication::translate("DCPUDeveloper", "Registers", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DCPUDeveloper", "A:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DCPUDeveloper", "Y:", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("DCPUDeveloper", "PC:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DCPUDeveloper", "B:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("DCPUDeveloper", "Z:", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("DCPUDeveloper", "SP:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DCPUDeveloper", "C:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DCPUDeveloper", "I:", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("DCPUDeveloper", "O:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DCPUDeveloper", "X:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("DCPUDeveloper", "J:", 0, QApplication::UnicodeUTF8));
        test->setText(QString());
        disassembly_gb->setTitle(QApplication::translate("DCPUDeveloper", "Disassembly", 0, QApplication::UnicodeUTF8));
        editor_gb->setTitle(QApplication::translate("DCPUDeveloper", "Editor", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("DCPUDeveloper", "File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("DCPUDeveloper", "Help", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("DCPUDeveloper", "Tools", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("DCPUDeveloper", "Edit", 0, QApplication::UnicodeUTF8));
        menuFind->setTitle(QApplication::translate("DCPUDeveloper", "Find", 0, QApplication::UnicodeUTF8));
        menuProgram->setTitle(QApplication::translate("DCPUDeveloper", "Program", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DCPUDeveloper: public Ui_DCPUDeveloper {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DCPUDEVELOPER_H
