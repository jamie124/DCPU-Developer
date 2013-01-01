/********************************************************************************
** Form generated from reading UI file 'dcpudeveloper.ui'
**
** Created: Wed Nov 28 20:21:50 2012
**      by: Qt User Interface Compiler version 5.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DCPUDEVELOPER_H
#define UI_DCPUDEVELOPER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QColumnView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

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
    QAction *actionStep;
    QAction *actionEnable_Step_Mode;
    QAction *actionReset;
    QAction *actionCompile;
    QAction *actionSave_As;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *project_tabwidget;
    QWidget *project_tab;
    QGridLayout *gridLayout_9;
    QTreeView *project_tree;
    QWidget *disassembly_tab;
    QTabWidget *editors_tabwidget;
    QWidget *default_editor_tab;
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
    QTabWidget *build_debug_tabwidget;
    QWidget *messages_tab;
    QGridLayout *gridLayout_5;
    QTextEdit *messages;
    QWidget *errors_tab;
    QGridLayout *gridLayout_8;
    QColumnView *columnView;
    QWidget *registers_tab;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout;
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
    QGroupBox *memory_gb;
    QScrollBar *memory_scrollbar;
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
            DCPUDeveloper->setObjectName(QStringLiteral("DCPUDeveloper"));
        DCPUDeveloper->resize(1200, 700);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DCPUDeveloper->sizePolicy().hasHeightForWidth());
        DCPUDeveloper->setSizePolicy(sizePolicy);
        DCPUDeveloper->setMinimumSize(QSize(1200, 700));
        actionExit = new QAction(DCPUDeveloper);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionNew = new QAction(DCPUDeveloper);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionSave = new QAction(DCPUDeveloper);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionOpen = new QAction(DCPUDeveloper);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionAbout = new QAction(DCPUDeveloper);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionOptions = new QAction(DCPUDeveloper);
        actionOptions->setObjectName(QStringLiteral("actionOptions"));
        actionUndo = new QAction(DCPUDeveloper);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));
        actionRedo = new QAction(DCPUDeveloper);
        actionRedo->setObjectName(QStringLiteral("actionRedo"));
        actionCut = new QAction(DCPUDeveloper);
        actionCut->setObjectName(QStringLiteral("actionCut"));
        actionCopy = new QAction(DCPUDeveloper);
        actionCopy->setObjectName(QStringLiteral("actionCopy"));
        actionPaste = new QAction(DCPUDeveloper);
        actionPaste->setObjectName(QStringLiteral("actionPaste"));
        actionSelect_All = new QAction(DCPUDeveloper);
        actionSelect_All->setObjectName(QStringLiteral("actionSelect_All"));
        actionFind_Selection = new QAction(DCPUDeveloper);
        actionFind_Selection->setObjectName(QStringLiteral("actionFind_Selection"));
        actionFind_File = new QAction(DCPUDeveloper);
        actionFind_File->setObjectName(QStringLiteral("actionFind_File"));
        actionRun = new QAction(DCPUDeveloper);
        actionRun->setObjectName(QStringLiteral("actionRun"));
        actionStep = new QAction(DCPUDeveloper);
        actionStep->setObjectName(QStringLiteral("actionStep"));
        actionEnable_Step_Mode = new QAction(DCPUDeveloper);
        actionEnable_Step_Mode->setObjectName(QStringLiteral("actionEnable_Step_Mode"));
        actionReset = new QAction(DCPUDeveloper);
        actionReset->setObjectName(QStringLiteral("actionReset"));
        actionCompile = new QAction(DCPUDeveloper);
        actionCompile->setObjectName(QStringLiteral("actionCompile"));
        actionCompile->setShortcutContext(Qt::WidgetShortcut);
        actionSave_As = new QAction(DCPUDeveloper);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        centralWidget = new QWidget(DCPUDeveloper);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMinimumSize(QSize(1146, 659));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        project_tabwidget = new QTabWidget(centralWidget);
        project_tabwidget->setObjectName(QStringLiteral("project_tabwidget"));
        project_tabwidget->setMinimumSize(QSize(300, 0));
        project_tabwidget->setMaximumSize(QSize(300, 16777215));
        project_tabwidget->setTabPosition(QTabWidget::North);
        project_tab = new QWidget();
        project_tab->setObjectName(QStringLiteral("project_tab"));
        gridLayout_9 = new QGridLayout(project_tab);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        project_tree = new QTreeView(project_tab);
        project_tree->setObjectName(QStringLiteral("project_tree"));
        sizePolicy.setHeightForWidth(project_tree->sizePolicy().hasHeightForWidth());
        project_tree->setSizePolicy(sizePolicy);

        gridLayout_9->addWidget(project_tree, 0, 0, 1, 1);

        project_tabwidget->addTab(project_tab, QString());
        disassembly_tab = new QWidget();
        disassembly_tab->setObjectName(QStringLiteral("disassembly_tab"));
        project_tabwidget->addTab(disassembly_tab, QString());

        gridLayout->addWidget(project_tabwidget, 0, 0, 1, 1);

        editors_tabwidget = new QTabWidget(centralWidget);
        editors_tabwidget->setObjectName(QStringLiteral("editors_tabwidget"));
        default_editor_tab = new QWidget();
        default_editor_tab->setObjectName(QStringLiteral("default_editor_tab"));
        editors_tabwidget->addTab(default_editor_tab, QString());

        gridLayout->addWidget(editors_tabwidget, 0, 1, 1, 1);

        program_control_gb = new QGroupBox(centralWidget);
        program_control_gb->setObjectName(QStringLiteral("program_control_gb"));
        gridLayout_7 = new QGridLayout(program_control_gb);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        cycle_label = new QLabel(program_control_gb);
        cycle_label->setObjectName(QStringLiteral("cycle_label"));

        gridLayout_2->addWidget(cycle_label, 0, 0, 1, 1);

        compile_button = new QPushButton(program_control_gb);
        compile_button->setObjectName(QStringLiteral("compile_button"));

        gridLayout_2->addWidget(compile_button, 1, 0, 1, 1);

        reset_button = new QPushButton(program_control_gb);
        reset_button->setObjectName(QStringLiteral("reset_button"));
        reset_button->setEnabled(false);

        gridLayout_2->addWidget(reset_button, 3, 0, 1, 1);

        view_screen_button = new QPushButton(program_control_gb);
        view_screen_button->setObjectName(QStringLiteral("view_screen_button"));

        gridLayout_2->addWidget(view_screen_button, 4, 0, 1, 1);

        run_button = new QPushButton(program_control_gb);
        run_button->setObjectName(QStringLiteral("run_button"));
        run_button->setEnabled(false);

        gridLayout_2->addWidget(run_button, 1, 1, 1, 1);

        step_button = new QPushButton(program_control_gb);
        step_button->setObjectName(QStringLiteral("step_button"));
        step_button->setEnabled(false);

        gridLayout_2->addWidget(step_button, 4, 1, 1, 1);

        toggle_step_button = new QPushButton(program_control_gb);
        toggle_step_button->setObjectName(QStringLiteral("toggle_step_button"));
        toggle_step_button->setEnabled(true);

        gridLayout_2->addWidget(toggle_step_button, 3, 1, 1, 1);


        gridLayout_7->addLayout(gridLayout_2, 0, 0, 1, 1);


        gridLayout->addWidget(program_control_gb, 1, 0, 1, 1);

        build_debug_tabwidget = new QTabWidget(centralWidget);
        build_debug_tabwidget->setObjectName(QStringLiteral("build_debug_tabwidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(build_debug_tabwidget->sizePolicy().hasHeightForWidth());
        build_debug_tabwidget->setSizePolicy(sizePolicy1);
        build_debug_tabwidget->setMinimumSize(QSize(800, 160));
        build_debug_tabwidget->setMaximumSize(QSize(16777215, 200));
        build_debug_tabwidget->setTabPosition(QTabWidget::South);
        messages_tab = new QWidget();
        messages_tab->setObjectName(QStringLiteral("messages_tab"));
        gridLayout_5 = new QGridLayout(messages_tab);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        messages = new QTextEdit(messages_tab);
        messages->setObjectName(QStringLiteral("messages"));

        gridLayout_5->addWidget(messages, 0, 0, 1, 1);

        build_debug_tabwidget->addTab(messages_tab, QString());
        errors_tab = new QWidget();
        errors_tab->setObjectName(QStringLiteral("errors_tab"));
        gridLayout_8 = new QGridLayout(errors_tab);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        columnView = new QColumnView(errors_tab);
        columnView->setObjectName(QStringLiteral("columnView"));

        gridLayout_8->addWidget(columnView, 0, 0, 1, 1);

        build_debug_tabwidget->addTab(errors_tab, QString());
        registers_tab = new QWidget();
        registers_tab->setObjectName(QStringLiteral("registers_tab"));
        gridLayout_4 = new QGridLayout(registers_tab);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        registers_gb = new QGroupBox(registers_tab);
        registers_gb->setObjectName(QStringLiteral("registers_gb"));
        gridLayout_3 = new QGridLayout(registers_gb);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label = new QLabel(registers_gb);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        register_a = new QSpinBox(registers_gb);
        register_a->setObjectName(QStringLiteral("register_a"));
        register_a->setReadOnly(false);
        register_a->setMaximum(99999);

        gridLayout_3->addWidget(register_a, 0, 1, 1, 1);

        label_5 = new QLabel(registers_gb);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_3->addWidget(label_5, 0, 2, 1, 1);

        register_y = new QSpinBox(registers_gb);
        register_y->setObjectName(QStringLiteral("register_y"));
        register_y->setReadOnly(false);
        register_y->setMaximum(99999);

        gridLayout_3->addWidget(register_y, 0, 3, 1, 1);

        label_9 = new QLabel(registers_gb);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_3->addWidget(label_9, 0, 4, 1, 1);

        register_pc = new QSpinBox(registers_gb);
        register_pc->setObjectName(QStringLiteral("register_pc"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(register_pc->sizePolicy().hasHeightForWidth());
        register_pc->setSizePolicy(sizePolicy2);
        register_pc->setMaximum(999999999);

        gridLayout_3->addWidget(register_pc, 0, 5, 1, 1);

        label_2 = new QLabel(registers_gb);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_3->addWidget(label_2, 1, 0, 1, 1);

        register_b = new QSpinBox(registers_gb);
        register_b->setObjectName(QStringLiteral("register_b"));
        register_b->setReadOnly(false);
        register_b->setMaximum(99999);

        gridLayout_3->addWidget(register_b, 1, 1, 1, 1);

        label_7 = new QLabel(registers_gb);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_3->addWidget(label_7, 1, 2, 1, 1);

        register_z = new QSpinBox(registers_gb);
        register_z->setObjectName(QStringLiteral("register_z"));
        register_z->setReadOnly(false);
        register_z->setMaximum(99999);

        gridLayout_3->addWidget(register_z, 1, 3, 1, 1);

        label_10 = new QLabel(registers_gb);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_3->addWidget(label_10, 1, 4, 1, 1);

        register_sp = new QSpinBox(registers_gb);
        register_sp->setObjectName(QStringLiteral("register_sp"));
        register_sp->setMaximum(999999999);

        gridLayout_3->addWidget(register_sp, 1, 5, 1, 1);

        label_3 = new QLabel(registers_gb);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_3->addWidget(label_3, 2, 0, 1, 1);

        register_c = new QSpinBox(registers_gb);
        register_c->setObjectName(QStringLiteral("register_c"));
        register_c->setReadOnly(false);
        register_c->setMaximum(99999);

        gridLayout_3->addWidget(register_c, 2, 1, 1, 1);

        label_6 = new QLabel(registers_gb);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_3->addWidget(label_6, 2, 2, 1, 1);

        register_i = new QSpinBox(registers_gb);
        register_i->setObjectName(QStringLiteral("register_i"));
        register_i->setReadOnly(false);
        register_i->setMaximum(99999);

        gridLayout_3->addWidget(register_i, 2, 3, 1, 1);

        label_11 = new QLabel(registers_gb);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_3->addWidget(label_11, 2, 4, 1, 1);

        register_o = new QSpinBox(registers_gb);
        register_o->setObjectName(QStringLiteral("register_o"));
        register_o->setMaximum(999);

        gridLayout_3->addWidget(register_o, 2, 5, 1, 1);

        label_4 = new QLabel(registers_gb);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_3->addWidget(label_4, 3, 0, 1, 1);

        register_x = new QSpinBox(registers_gb);
        register_x->setObjectName(QStringLiteral("register_x"));
        register_x->setReadOnly(false);
        register_x->setMaximum(99999);

        gridLayout_3->addWidget(register_x, 3, 1, 1, 1);

        label_8 = new QLabel(registers_gb);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_3->addWidget(label_8, 3, 2, 1, 1);

        register_j = new QSpinBox(registers_gb);
        register_j->setObjectName(QStringLiteral("register_j"));
        register_j->setReadOnly(false);
        register_j->setMaximum(99999);

        gridLayout_3->addWidget(register_j, 3, 3, 1, 1);


        horizontalLayout->addWidget(registers_gb);

        memory_gb = new QGroupBox(registers_tab);
        memory_gb->setObjectName(QStringLiteral("memory_gb"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(memory_gb->sizePolicy().hasHeightForWidth());
        memory_gb->setSizePolicy(sizePolicy3);
        memory_gb->setMinimumSize(QSize(500, 150));

        horizontalLayout->addWidget(memory_gb);


        gridLayout_4->addLayout(horizontalLayout, 0, 0, 1, 1);

        memory_scrollbar = new QScrollBar(registers_tab);
        memory_scrollbar->setObjectName(QStringLiteral("memory_scrollbar"));
        memory_scrollbar->setMaximum(1);
        memory_scrollbar->setOrientation(Qt::Vertical);

        gridLayout_4->addWidget(memory_scrollbar, 0, 1, 1, 1);

        build_debug_tabwidget->addTab(registers_tab, QString());

        gridLayout->addWidget(build_debug_tabwidget, 1, 1, 1, 1);

        DCPUDeveloper->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DCPUDeveloper);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1200, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QStringLiteral("menuTools"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuFind = new QMenu(menuEdit);
        menuFind->setObjectName(QStringLiteral("menuFind"));
        menuProgram = new QMenu(menuBar);
        menuProgram->setObjectName(QStringLiteral("menuProgram"));
        DCPUDeveloper->setMenuBar(menuBar);
        statusBar = new QStatusBar(DCPUDeveloper);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DCPUDeveloper->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuProgram->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
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
        menuProgram->addAction(actionCompile);
        menuProgram->addSeparator();
        menuProgram->addAction(actionRun);
        menuProgram->addAction(actionReset);
        menuProgram->addSeparator();
        menuProgram->addAction(actionEnable_Step_Mode);
        menuProgram->addAction(actionStep);

        retranslateUi(DCPUDeveloper);

        project_tabwidget->setCurrentIndex(1);
        editors_tabwidget->setCurrentIndex(0);
        build_debug_tabwidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(DCPUDeveloper);
    } // setupUi

    void retranslateUi(QMainWindow *DCPUDeveloper)
    {
        DCPUDeveloper->setWindowTitle(QApplication::translate("DCPUDeveloper", "DCPU Developer", 0));
        actionExit->setText(QApplication::translate("DCPUDeveloper", "Exit", 0));
        actionExit->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+Q", 0));
        actionNew->setText(QApplication::translate("DCPUDeveloper", "New", 0));
        actionNew->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+N", 0));
        actionSave->setText(QApplication::translate("DCPUDeveloper", "Save", 0));
        actionSave->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+S", 0));
        actionOpen->setText(QApplication::translate("DCPUDeveloper", "Open", 0));
        actionOpen->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+O", 0));
        actionAbout->setText(QApplication::translate("DCPUDeveloper", "About", 0));
        actionOptions->setText(QApplication::translate("DCPUDeveloper", "Options", 0));
        actionUndo->setText(QApplication::translate("DCPUDeveloper", "Undo", 0));
        actionUndo->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+Z", 0));
        actionRedo->setText(QApplication::translate("DCPUDeveloper", "Redo", 0));
        actionRedo->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+Y", 0));
        actionCut->setText(QApplication::translate("DCPUDeveloper", "Cut", 0));
        actionCut->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+X", 0));
        actionCopy->setText(QApplication::translate("DCPUDeveloper", "Copy", 0));
        actionCopy->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+C", 0));
        actionPaste->setText(QApplication::translate("DCPUDeveloper", "Paste", 0));
        actionPaste->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+V", 0));
        actionSelect_All->setText(QApplication::translate("DCPUDeveloper", "Select All", 0));
        actionSelect_All->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+A", 0));
        actionFind_Selection->setText(QApplication::translate("DCPUDeveloper", "Find Selection", 0));
        actionFind_File->setText(QApplication::translate("DCPUDeveloper", "Find in File", 0));
        actionFind_File->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+F", 0));
        actionRun->setText(QApplication::translate("DCPUDeveloper", "Run", 0));
        actionRun->setShortcut(QApplication::translate("DCPUDeveloper", "F5", 0));
        actionStep->setText(QApplication::translate("DCPUDeveloper", "Step", 0));
        actionStep->setShortcut(QApplication::translate("DCPUDeveloper", "F7", 0));
        actionEnable_Step_Mode->setText(QApplication::translate("DCPUDeveloper", "Enable Step Mode", 0));
        actionEnable_Step_Mode->setShortcut(QApplication::translate("DCPUDeveloper", "F4", 0));
        actionReset->setText(QApplication::translate("DCPUDeveloper", "Reset", 0));
        actionReset->setShortcut(QApplication::translate("DCPUDeveloper", "F3", 0));
        actionCompile->setText(QApplication::translate("DCPUDeveloper", "Compile", 0));
        actionCompile->setShortcut(QApplication::translate("DCPUDeveloper", "F6", 0));
        actionSave_As->setText(QApplication::translate("DCPUDeveloper", "Save As", 0));
        actionSave_As->setShortcut(QApplication::translate("DCPUDeveloper", "Ctrl+Shift+S", 0));
        project_tabwidget->setTabText(project_tabwidget->indexOf(project_tab), QApplication::translate("DCPUDeveloper", "Project", 0));
        project_tabwidget->setTabText(project_tabwidget->indexOf(disassembly_tab), QApplication::translate("DCPUDeveloper", "Disassembly", 0));
        editors_tabwidget->setTabText(editors_tabwidget->indexOf(default_editor_tab), QApplication::translate("DCPUDeveloper", "Default", 0));
        program_control_gb->setTitle(QApplication::translate("DCPUDeveloper", "Program Control", 0));
        cycle_label->setText(QApplication::translate("DCPUDeveloper", "Cycle: 0", 0));
        compile_button->setText(QApplication::translate("DCPUDeveloper", "Compile", 0));
        reset_button->setText(QApplication::translate("DCPUDeveloper", "Reset", 0));
        view_screen_button->setText(QApplication::translate("DCPUDeveloper", "View Screen", 0));
        run_button->setText(QApplication::translate("DCPUDeveloper", "Run", 0));
        step_button->setText(QApplication::translate("DCPUDeveloper", "Step", 0));
        toggle_step_button->setText(QApplication::translate("DCPUDeveloper", "Enable Step Mode", 0));
        build_debug_tabwidget->setTabText(build_debug_tabwidget->indexOf(messages_tab), QApplication::translate("DCPUDeveloper", "Messages", 0));
        build_debug_tabwidget->setTabText(build_debug_tabwidget->indexOf(errors_tab), QApplication::translate("DCPUDeveloper", "Errors", 0));
        registers_gb->setTitle(QApplication::translate("DCPUDeveloper", "Registers", 0));
        label->setText(QApplication::translate("DCPUDeveloper", "A:", 0));
        label_5->setText(QApplication::translate("DCPUDeveloper", "Y:", 0));
        label_9->setText(QApplication::translate("DCPUDeveloper", "PC:", 0));
        label_2->setText(QApplication::translate("DCPUDeveloper", "B:", 0));
        label_7->setText(QApplication::translate("DCPUDeveloper", "Z:", 0));
        label_10->setText(QApplication::translate("DCPUDeveloper", "SP:", 0));
        label_3->setText(QApplication::translate("DCPUDeveloper", "C:", 0));
        label_6->setText(QApplication::translate("DCPUDeveloper", "I:", 0));
        label_11->setText(QApplication::translate("DCPUDeveloper", "O:", 0));
        label_4->setText(QApplication::translate("DCPUDeveloper", "X:", 0));
        label_8->setText(QApplication::translate("DCPUDeveloper", "J:", 0));
        memory_gb->setTitle(QApplication::translate("DCPUDeveloper", "Memory View", 0));
        build_debug_tabwidget->setTabText(build_debug_tabwidget->indexOf(registers_tab), QApplication::translate("DCPUDeveloper", "Debug Info", 0));
        menuFile->setTitle(QApplication::translate("DCPUDeveloper", "File", 0));
        menuHelp->setTitle(QApplication::translate("DCPUDeveloper", "Help", 0));
        menuTools->setTitle(QApplication::translate("DCPUDeveloper", "Tools", 0));
        menuEdit->setTitle(QApplication::translate("DCPUDeveloper", "Edit", 0));
        menuFind->setTitle(QApplication::translate("DCPUDeveloper", "Find", 0));
        menuProgram->setTitle(QApplication::translate("DCPUDeveloper", "Program", 0));
    } // retranslateUi

};

namespace Ui {
    class DCPUDeveloper: public Ui_DCPUDeveloper {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DCPUDEVELOPER_H
