/********************************************************************************
** Form generated from reading UI file 'quickadddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUICKADDDIALOG_H
#define UI_QUICKADDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QuickAddDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *QuickAddDialog)
    {
        if (QuickAddDialog->objectName().isEmpty())
            QuickAddDialog->setObjectName(QString::fromUtf8("QuickAddDialog"));
        QuickAddDialog->resize(369, 77);
        verticalLayout = new QVBoxLayout(QuickAddDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lineEdit = new QLineEdit(QuickAddDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        buttonBox = new QDialogButtonBox(QuickAddDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(QuickAddDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), QuickAddDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), QuickAddDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(QuickAddDialog);
    } // setupUi

    void retranslateUi(QDialog *QuickAddDialog)
    {
        QuickAddDialog->setWindowTitle(QCoreApplication::translate("QuickAddDialog", "Add New Todo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QuickAddDialog: public Ui_QuickAddDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUICKADDDIALOG_H
