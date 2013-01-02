/********************************************************************************
** Form generated from reading UI file 'lem_monitor.ui'
**
** Created: Wed Jan 2 21:10:42 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEM_MONITOR_H
#define UI_LEM_MONITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LemMonitor
{
public:
    QGroupBox *lem_gb;

    void setupUi(QWidget *LemMonitor)
    {
        if (LemMonitor->objectName().isEmpty())
            LemMonitor->setObjectName(QString::fromUtf8("LemMonitor"));
        LemMonitor->resize(400, 300);
        lem_gb = new QGroupBox(LemMonitor);
        lem_gb->setObjectName(QString::fromUtf8("lem_gb"));
        lem_gb->setGeometry(QRect(0, -10, 401, 311));

        retranslateUi(LemMonitor);

        QMetaObject::connectSlotsByName(LemMonitor);
    } // setupUi

    void retranslateUi(QWidget *LemMonitor)
    {
        LemMonitor->setWindowTitle(QApplication::translate("LemMonitor", "LEM 1802", 0, QApplication::UnicodeUTF8));
        lem_gb->setTitle(QApplication::translate("LemMonitor", "GroupBox", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LemMonitor: public Ui_LemMonitor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEM_MONITOR_H
