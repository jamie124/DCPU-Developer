/********************************************************************************
** Form generated from reading UI file 'findreplacedialog.ui'
**
** Created: Sat Jan 19 22:07:42 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDREPLACEDIALOG_H
#define UI_FINDREPLACEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <findreplaceform.h>

QT_BEGIN_NAMESPACE

class Ui_FindReplaceDialog
{
public:
    QGridLayout *gridLayout;
    FindReplaceForm *findReplaceForm;

    void setupUi(QDialog *FindReplaceDialog)
    {
        if (FindReplaceDialog->objectName().isEmpty())
            FindReplaceDialog->setObjectName(QString::fromUtf8("FindReplaceDialog"));
        FindReplaceDialog->resize(342, 140);
        gridLayout = new QGridLayout(FindReplaceDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        findReplaceForm = new FindReplaceForm(FindReplaceDialog);
        findReplaceForm->setObjectName(QString::fromUtf8("findReplaceForm"));

        gridLayout->addWidget(findReplaceForm, 0, 0, 1, 1);


        retranslateUi(FindReplaceDialog);

        QMetaObject::connectSlotsByName(FindReplaceDialog);
    } // setupUi

    void retranslateUi(QDialog *FindReplaceDialog)
    {
        FindReplaceDialog->setWindowTitle(QApplication::translate("FindReplaceDialog", "Find/Replace", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FindReplaceDialog: public Ui_FindReplaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDREPLACEDIALOG_H
