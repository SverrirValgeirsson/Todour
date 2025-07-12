/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *btnDirBrowse;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *le_activeFont;
    QPushButton *pb_fontDlgActive;
    QPushButton *pb_colorDlgActive;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *le_InactiveFont;
    QPushButton *pb_fontDlgInactive;
    QPushButton *pb_colorDlgInactive;
    QHBoxLayout *horizontalLayout_5;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_7;
    QCheckBox *cb_due;
    QLabel *label_3;
    QSpinBox *sb_due_warning;
    QPushButton *pb_warningColor;
    QPushButton *pb_lateColor;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_5;
    QSpinBox *sb_fontSize;
    QCheckBox *cb_autorefresh;
    QHBoxLayout *horizontalLayout_12;
    QCheckBox *cb_showdates;
    QCheckBox *cb_dates;
    QHBoxLayout *horizontalLayout_13;
    QCheckBox *cb_threshold_inactive;
    QCheckBox *cb_separate;
    QCheckBox *cb_liveSearch;
    QCheckBox *cb_hotKey;
    QCheckBox *cb_deletedfile;
    QCheckBox *cb_threshold;
    QCheckBox *cb_threshold_label;
    QCheckBox *cb_due_as_threshold;
    QCheckBox *cb_tray_icon;
    QCheckBox *cb_CheckUpdates;
    QCheckBox *cb_removeDoublets;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_6;
    QComboBox *cb_default_threshold;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_7;
    QLineEdit *search_not_char;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_8;
    QCheckBox *cb_monday;
    QCheckBox *cb_tuesday;
    QCheckBox *cb_wednesday;
    QCheckBox *cb_thursday;
    QCheckBox *cb_friday;
    QCheckBox *cb_saturday;
    QCheckBox *cb_sunday;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_4;
    QComboBox *comb_priorities;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->resize(534, 782);
        SettingsDialog->setSizeGripEnabled(true);
        verticalLayout = new QVBoxLayout(SettingsDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(SettingsDialog);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 500, 774));
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(scrollAreaWidgetContents);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        btnDirBrowse = new QPushButton(scrollAreaWidgetContents);
        btnDirBrowse->setObjectName(QString::fromUtf8("btnDirBrowse"));

        horizontalLayout->addWidget(btnDirBrowse);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(scrollAreaWidgetContents);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit_2 = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        horizontalLayout_2->addWidget(lineEdit_2);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        groupBox_2 = new QGroupBox(scrollAreaWidgetContents);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_4 = new QHBoxLayout(groupBox_2);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        le_activeFont = new QLineEdit(groupBox_2);
        le_activeFont->setObjectName(QString::fromUtf8("le_activeFont"));
        le_activeFont->setEnabled(true);
        le_activeFont->setReadOnly(true);

        horizontalLayout_4->addWidget(le_activeFont);

        pb_fontDlgActive = new QPushButton(groupBox_2);
        pb_fontDlgActive->setObjectName(QString::fromUtf8("pb_fontDlgActive"));

        horizontalLayout_4->addWidget(pb_fontDlgActive);

        pb_colorDlgActive = new QPushButton(groupBox_2);
        pb_colorDlgActive->setObjectName(QString::fromUtf8("pb_colorDlgActive"));

        horizontalLayout_4->addWidget(pb_colorDlgActive);


        horizontalLayout_3->addWidget(groupBox_2);

        groupBox = new QGroupBox(scrollAreaWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_6 = new QHBoxLayout(groupBox);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        le_InactiveFont = new QLineEdit(groupBox);
        le_InactiveFont->setObjectName(QString::fromUtf8("le_InactiveFont"));
        le_InactiveFont->setReadOnly(true);

        horizontalLayout_6->addWidget(le_InactiveFont);

        pb_fontDlgInactive = new QPushButton(groupBox);
        pb_fontDlgInactive->setObjectName(QString::fromUtf8("pb_fontDlgInactive"));

        horizontalLayout_6->addWidget(pb_fontDlgInactive);

        pb_colorDlgInactive = new QPushButton(groupBox);
        pb_colorDlgInactive->setObjectName(QString::fromUtf8("pb_colorDlgInactive"));

        horizontalLayout_6->addWidget(pb_colorDlgInactive);


        horizontalLayout_3->addWidget(groupBox);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        groupBox_3 = new QGroupBox(scrollAreaWidgetContents);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        horizontalLayout_7 = new QHBoxLayout(groupBox_3);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        cb_due = new QCheckBox(groupBox_3);
        cb_due->setObjectName(QString::fromUtf8("cb_due"));

        horizontalLayout_7->addWidget(cb_due);

        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_7->addWidget(label_3);

        sb_due_warning = new QSpinBox(groupBox_3);
        sb_due_warning->setObjectName(QString::fromUtf8("sb_due_warning"));

        horizontalLayout_7->addWidget(sb_due_warning);

        pb_warningColor = new QPushButton(groupBox_3);
        pb_warningColor->setObjectName(QString::fromUtf8("pb_warningColor"));
        pb_warningColor->setAutoFillBackground(true);
        pb_warningColor->setFlat(true);

        horizontalLayout_7->addWidget(pb_warningColor);

        pb_lateColor = new QPushButton(groupBox_3);
        pb_lateColor->setObjectName(QString::fromUtf8("pb_lateColor"));
        pb_lateColor->setAutoFillBackground(true);
        pb_lateColor->setFlat(true);

        horizontalLayout_7->addWidget(pb_lateColor);


        horizontalLayout_5->addWidget(groupBox_3);


        verticalLayout_2->addLayout(horizontalLayout_5);

        groupBox_4 = new QGroupBox(scrollAreaWidgetContents);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        horizontalLayout_9 = new QHBoxLayout(groupBox_4);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_9->addWidget(label_5);

        sb_fontSize = new QSpinBox(groupBox_4);
        sb_fontSize->setObjectName(QString::fromUtf8("sb_fontSize"));

        horizontalLayout_9->addWidget(sb_fontSize);


        verticalLayout_2->addWidget(groupBox_4);

        cb_autorefresh = new QCheckBox(scrollAreaWidgetContents);
        cb_autorefresh->setObjectName(QString::fromUtf8("cb_autorefresh"));

        verticalLayout_2->addWidget(cb_autorefresh);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        cb_showdates = new QCheckBox(scrollAreaWidgetContents);
        cb_showdates->setObjectName(QString::fromUtf8("cb_showdates"));

        horizontalLayout_12->addWidget(cb_showdates);

        cb_dates = new QCheckBox(scrollAreaWidgetContents);
        cb_dates->setObjectName(QString::fromUtf8("cb_dates"));

        horizontalLayout_12->addWidget(cb_dates);


        verticalLayout_2->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        cb_threshold_inactive = new QCheckBox(scrollAreaWidgetContents);
        cb_threshold_inactive->setObjectName(QString::fromUtf8("cb_threshold_inactive"));

        horizontalLayout_13->addWidget(cb_threshold_inactive);

        cb_separate = new QCheckBox(scrollAreaWidgetContents);
        cb_separate->setObjectName(QString::fromUtf8("cb_separate"));
        cb_separate->setEnabled(true);

        horizontalLayout_13->addWidget(cb_separate);


        verticalLayout_2->addLayout(horizontalLayout_13);

        cb_liveSearch = new QCheckBox(scrollAreaWidgetContents);
        cb_liveSearch->setObjectName(QString::fromUtf8("cb_liveSearch"));

        verticalLayout_2->addWidget(cb_liveSearch);

        cb_hotKey = new QCheckBox(scrollAreaWidgetContents);
        cb_hotKey->setObjectName(QString::fromUtf8("cb_hotKey"));

        verticalLayout_2->addWidget(cb_hotKey);

        cb_deletedfile = new QCheckBox(scrollAreaWidgetContents);
        cb_deletedfile->setObjectName(QString::fromUtf8("cb_deletedfile"));

        verticalLayout_2->addWidget(cb_deletedfile);

        cb_threshold = new QCheckBox(scrollAreaWidgetContents);
        cb_threshold->setObjectName(QString::fromUtf8("cb_threshold"));

        verticalLayout_2->addWidget(cb_threshold);

        cb_threshold_label = new QCheckBox(scrollAreaWidgetContents);
        cb_threshold_label->setObjectName(QString::fromUtf8("cb_threshold_label"));

        verticalLayout_2->addWidget(cb_threshold_label);

        cb_due_as_threshold = new QCheckBox(scrollAreaWidgetContents);
        cb_due_as_threshold->setObjectName(QString::fromUtf8("cb_due_as_threshold"));

        verticalLayout_2->addWidget(cb_due_as_threshold);

        cb_tray_icon = new QCheckBox(scrollAreaWidgetContents);
        cb_tray_icon->setObjectName(QString::fromUtf8("cb_tray_icon"));

        verticalLayout_2->addWidget(cb_tray_icon);

        cb_CheckUpdates = new QCheckBox(scrollAreaWidgetContents);
        cb_CheckUpdates->setObjectName(QString::fromUtf8("cb_CheckUpdates"));

        verticalLayout_2->addWidget(cb_CheckUpdates);

        cb_removeDoublets = new QCheckBox(scrollAreaWidgetContents);
        cb_removeDoublets->setObjectName(QString::fromUtf8("cb_removeDoublets"));

        verticalLayout_2->addWidget(cb_removeDoublets);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_6 = new QLabel(scrollAreaWidgetContents);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_11->addWidget(label_6);

        cb_default_threshold = new QComboBox(scrollAreaWidgetContents);
        cb_default_threshold->addItem(QString());
        cb_default_threshold->addItem(QString());
        cb_default_threshold->setObjectName(QString::fromUtf8("cb_default_threshold"));

        horizontalLayout_11->addWidget(cb_default_threshold);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_11);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_7 = new QLabel(scrollAreaWidgetContents);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_10->addWidget(label_7);

        search_not_char = new QLineEdit(scrollAreaWidgetContents);
        search_not_char->setObjectName(QString::fromUtf8("search_not_char"));
        search_not_char->setMaxLength(1);
        search_not_char->setAlignment(Qt::AlignCenter);

        horizontalLayout_10->addWidget(search_not_char);

        horizontalSpacer = new QSpacerItem(80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_10);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        label_8 = new QLabel(scrollAreaWidgetContents);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_14->addWidget(label_8);

        cb_monday = new QCheckBox(scrollAreaWidgetContents);
        cb_monday->setObjectName(QString::fromUtf8("cb_monday"));

        horizontalLayout_14->addWidget(cb_monday);

        cb_tuesday = new QCheckBox(scrollAreaWidgetContents);
        cb_tuesday->setObjectName(QString::fromUtf8("cb_tuesday"));

        horizontalLayout_14->addWidget(cb_tuesday);

        cb_wednesday = new QCheckBox(scrollAreaWidgetContents);
        cb_wednesday->setObjectName(QString::fromUtf8("cb_wednesday"));

        horizontalLayout_14->addWidget(cb_wednesday);

        cb_thursday = new QCheckBox(scrollAreaWidgetContents);
        cb_thursday->setObjectName(QString::fromUtf8("cb_thursday"));

        horizontalLayout_14->addWidget(cb_thursday);

        cb_friday = new QCheckBox(scrollAreaWidgetContents);
        cb_friday->setObjectName(QString::fromUtf8("cb_friday"));

        horizontalLayout_14->addWidget(cb_friday);

        cb_saturday = new QCheckBox(scrollAreaWidgetContents);
        cb_saturday->setObjectName(QString::fromUtf8("cb_saturday"));

        horizontalLayout_14->addWidget(cb_saturday);

        cb_sunday = new QCheckBox(scrollAreaWidgetContents);
        cb_sunday->setObjectName(QString::fromUtf8("cb_sunday"));

        horizontalLayout_14->addWidget(cb_sunday);


        verticalLayout_2->addLayout(horizontalLayout_14);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_4 = new QLabel(scrollAreaWidgetContents);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_8->addWidget(label_4);

        comb_priorities = new QComboBox(scrollAreaWidgetContents);
        comb_priorities->addItem(QString());
        comb_priorities->addItem(QString());
        comb_priorities->addItem(QString());
        comb_priorities->setObjectName(QString::fromUtf8("comb_priorities"));

        horizontalLayout_8->addWidget(comb_priorities);


        verticalLayout_2->addLayout(horizontalLayout_8);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        buttonBox = new QDialogButtonBox(SettingsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(SettingsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SettingsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SettingsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QCoreApplication::translate("SettingsDialog", "Todour Settings", nullptr));
        label->setText(QCoreApplication::translate("SettingsDialog", "Directory", nullptr));
        btnDirBrowse->setText(QCoreApplication::translate("SettingsDialog", "...", nullptr));
        label_2->setText(QCoreApplication::translate("SettingsDialog", "Inactive", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_2->setToolTip(QCoreApplication::translate("SettingsDialog", "Keywords that mark an item as inactive. Seperated by ';'", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        lineEdit_2->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
        groupBox_2->setTitle(QCoreApplication::translate("SettingsDialog", "Active Font", nullptr));
        le_activeFont->setText(QCoreApplication::translate("SettingsDialog", "Active", nullptr));
        pb_fontDlgActive->setText(QCoreApplication::translate("SettingsDialog", "Font", nullptr));
        pb_colorDlgActive->setText(QCoreApplication::translate("SettingsDialog", "Color", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SettingsDialog", "Inactive Font", nullptr));
        le_InactiveFont->setText(QCoreApplication::translate("SettingsDialog", "Inactive", nullptr));
        pb_fontDlgInactive->setText(QCoreApplication::translate("SettingsDialog", "Font", nullptr));
        pb_colorDlgInactive->setText(QCoreApplication::translate("SettingsDialog", "Color", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("SettingsDialog", "Due ", nullptr));
        cb_due->setText(QCoreApplication::translate("SettingsDialog", "Enable due:", nullptr));
        label_3->setText(QCoreApplication::translate("SettingsDialog", "Warning:", nullptr));
        pb_warningColor->setText(QCoreApplication::translate("SettingsDialog", "Warning Color", nullptr));
        pb_lateColor->setText(QCoreApplication::translate("SettingsDialog", "Late Color", nullptr));
        groupBox_4->setTitle(QString());
        label_5->setText(QCoreApplication::translate("SettingsDialog", "<html><head/><body><p>Font size</p></body></html>", nullptr));
        cb_autorefresh->setText(QCoreApplication::translate("SettingsDialog", "Refresh automatically when files has changed", nullptr));
        cb_showdates->setText(QCoreApplication::translate("SettingsDialog", "Show dates", nullptr));
        cb_dates->setText(QCoreApplication::translate("SettingsDialog", "Add dates to lines", nullptr));
        cb_threshold_inactive->setText(QCoreApplication::translate("SettingsDialog", "Treat threshold as inactive", nullptr));
        cb_separate->setText(QCoreApplication::translate("SettingsDialog", "Sort all inactives last", nullptr));
        cb_liveSearch->setText(QCoreApplication::translate("SettingsDialog", "Live search", nullptr));
        cb_hotKey->setText(QCoreApplication::translate("SettingsDialog", "Ctrl+Alt+t as Todo hotkey", nullptr));
#if QT_CONFIG(tooltip)
        cb_deletedfile->setToolTip(QCoreApplication::translate("SettingsDialog", "Save deleted items in deleted.txt", nullptr));
#endif // QT_CONFIG(tooltip)
        cb_deletedfile->setText(QCoreApplication::translate("SettingsDialog", "Track delete", nullptr));
        cb_threshold->setText(QCoreApplication::translate("SettingsDialog", "Respect threshold dates (t:xxxx-xx-xx)", nullptr));
        cb_threshold_label->setText(QCoreApplication::translate("SettingsDialog", "Respect threshold contexts and projecs (t:+project or t:@context)", nullptr));
        cb_due_as_threshold->setText(QCoreApplication::translate("SettingsDialog", "Treat due as threshold", nullptr));
        cb_tray_icon->setText(QCoreApplication::translate("SettingsDialog", "Enable tray", nullptr));
        cb_CheckUpdates->setText(QCoreApplication::translate("SettingsDialog", "Check for updates (ca once per week and only at startup. No information is sent)", nullptr));
        cb_removeDoublets->setText(QCoreApplication::translate("SettingsDialog", "Remove Duplicate Lines", nullptr));
        label_6->setText(QCoreApplication::translate("SettingsDialog", "Default deadline/threshold to add when missing", nullptr));
        cb_default_threshold->setItemText(0, QCoreApplication::translate("SettingsDialog", "due:", nullptr));
        cb_default_threshold->setItemText(1, QCoreApplication::translate("SettingsDialog", "t:", nullptr));

#if QT_CONFIG(tooltip)
        cb_default_threshold->setToolTip(QCoreApplication::translate("SettingsDialog", "What to add to a rec: line without any due: or t:", nullptr));
#endif // QT_CONFIG(tooltip)
        label_7->setText(QCoreApplication::translate("SettingsDialog", "Symbol for \"NOT\" in search", nullptr));
        label_8->setText(QCoreApplication::translate("SettingsDialog", "Business days", nullptr));
        cb_monday->setText(QCoreApplication::translate("SettingsDialog", "Mon", nullptr));
        cb_tuesday->setText(QCoreApplication::translate("SettingsDialog", "Tue", nullptr));
        cb_wednesday->setText(QCoreApplication::translate("SettingsDialog", "Wed", nullptr));
        cb_thursday->setText(QCoreApplication::translate("SettingsDialog", "Thu", nullptr));
        cb_friday->setText(QCoreApplication::translate("SettingsDialog", "Fri", nullptr));
        cb_saturday->setText(QCoreApplication::translate("SettingsDialog", "Sat", nullptr));
        cb_sunday->setText(QCoreApplication::translate("SettingsDialog", "Sun", nullptr));
        label_4->setText(QCoreApplication::translate("SettingsDialog", "Priorities at checked", nullptr));
        comb_priorities->setItemText(0, QCoreApplication::translate("SettingsDialog", "Remove", nullptr));
        comb_priorities->setItemText(1, QCoreApplication::translate("SettingsDialog", "Keep (move to after date) ", nullptr));
        comb_priorities->setItemText(2, QCoreApplication::translate("SettingsDialog", "Add pri: tag", nullptr));

        comb_priorities->setCurrentText(QCoreApplication::translate("SettingsDialog", "Remove", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
