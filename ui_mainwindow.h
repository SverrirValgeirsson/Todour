/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSettings;
    QAction *actionAbout;
    QAction *actionCheck_for_updates;
    QAction *actionQuit;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionShow_All;
    QAction *actionStay_On_Top;
    QAction *actionManual;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QSpacerItem *horizontalSpacer;
    QCheckBox *cb_threshold_inactive;
    QPushButton *btn_Alphabetical;
    QLineEdit *lineEdit_2;
    QPushButton *context_lock;
    QHBoxLayout *horizontalLayout_3;
    QListView *lv_activetags;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *newVersionView;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lbl_newVersion;
    QLabel *lbl_latestVersion;
    QLabel *txtLatestVersion;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pb_closeVersionBar;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuEdit;
    QMenu *menuWindow;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(693, 420);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/todour.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setIconSize(QSize(24, 24));
        MainWindow->setToolButtonStyle(Qt::ToolButtonIconOnly);
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionCheck_for_updates = new QAction(MainWindow);
        actionCheck_for_updates->setObjectName(QString::fromUtf8("actionCheck_for_updates"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        actionShow_All = new QAction(MainWindow);
        actionShow_All->setObjectName(QString::fromUtf8("actionShow_All"));
        actionShow_All->setCheckable(true);
        actionStay_On_Top = new QAction(MainWindow);
        actionStay_On_Top->setObjectName(QString::fromUtf8("actionStay_On_Top"));
        actionStay_On_Top->setCheckable(true);
        actionManual = new QAction(MainWindow);
        actionManual->setObjectName(QString::fromUtf8("actionManual"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        horizontalLayout->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        horizontalLayout->addWidget(pushButton_4);

        horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cb_threshold_inactive = new QCheckBox(centralWidget);
        cb_threshold_inactive->setObjectName(QString::fromUtf8("cb_threshold_inactive"));

        horizontalLayout->addWidget(cb_threshold_inactive);

        btn_Alphabetical = new QPushButton(centralWidget);
        btn_Alphabetical->setObjectName(QString::fromUtf8("btn_Alphabetical"));
        btn_Alphabetical->setCheckable(true);

        horizontalLayout->addWidget(btn_Alphabetical);

        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setMinimumSize(QSize(0, 0));
        lineEdit_2->setBaseSize(QSize(100, 0));
        lineEdit_2->setStyleSheet(QString::fromUtf8("border: 1px solid lightgray;\n"
"border-radius: 5px;\n"
""));
        lineEdit_2->setClearButtonEnabled(true);

        horizontalLayout->addWidget(lineEdit_2);

        context_lock = new QPushButton(centralWidget);
        context_lock->setObjectName(QString::fromUtf8("context_lock"));
        context_lock->setCheckable(true);

        horizontalLayout->addWidget(context_lock);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lv_activetags = new QListView(centralWidget);
        lv_activetags->setObjectName(QString::fromUtf8("lv_activetags"));
        lv_activetags->setEnabled(true);

        horizontalLayout_3->addWidget(lv_activetags);

        tableView = new QTableView(centralWidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setContextMenuPolicy(Qt::CustomContextMenu);
        tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tableView->setAutoScroll(true);
        tableView->setAlternatingRowColors(false);
        tableView->setTextElideMode(Qt::ElideRight);
        tableView->setSortingEnabled(false);
        tableView->setWordWrap(false);
        tableView->verticalHeader()->setVisible(false);
        tableView->verticalHeader()->setHighlightSections(false);

        horizontalLayout_3->addWidget(tableView);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_2->addWidget(lineEdit);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout_2->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout_2);

        newVersionView = new QWidget(centralWidget);
        newVersionView->setObjectName(QString::fromUtf8("newVersionView"));
        horizontalLayout_4 = new QHBoxLayout(newVersionView);
        horizontalLayout_4->setSpacing(20);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        lbl_newVersion = new QLabel(newVersionView);
        lbl_newVersion->setObjectName(QString::fromUtf8("lbl_newVersion"));
        lbl_newVersion->setTextFormat(Qt::RichText);
        lbl_newVersion->setOpenExternalLinks(true);
        lbl_newVersion->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse);

        horizontalLayout_4->addWidget(lbl_newVersion);

        lbl_latestVersion = new QLabel(newVersionView);
        lbl_latestVersion->setObjectName(QString::fromUtf8("lbl_latestVersion"));

        horizontalLayout_4->addWidget(lbl_latestVersion);

        txtLatestVersion = new QLabel(newVersionView);
        txtLatestVersion->setObjectName(QString::fromUtf8("txtLatestVersion"));

        horizontalLayout_4->addWidget(txtLatestVersion);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        pb_closeVersionBar = new QPushButton(newVersionView);
        pb_closeVersionBar->setObjectName(QString::fromUtf8("pb_closeVersionBar"));

        horizontalLayout_4->addWidget(pb_closeVersionBar);


        verticalLayout->addWidget(newVersionView);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 693, 24));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuWindow = new QMenu(menuBar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        MainWindow->setMenuBar(menuBar);
        QWidget::setTabOrder(lineEdit, pushButton);
        QWidget::setTabOrder(pushButton, pushButton_2);
        QWidget::setTabOrder(pushButton_2, lineEdit_2);
        QWidget::setTabOrder(lineEdit_2, pushButton_3);
        QWidget::setTabOrder(pushButton_3, pushButton_4);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuWindow->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionSettings);
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionCheck_for_updates);
        menuHelp->addAction(actionManual);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuWindow->addAction(actionShow_All);
        menuWindow->addAction(actionStay_On_Top);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Todour", nullptr));
        actionSettings->setText(QCoreApplication::translate("MainWindow", "Settings..", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        actionCheck_for_updates->setText(QCoreApplication::translate("MainWindow", "Check for updates", nullptr));
        actionQuit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
        actionUndo->setText(QCoreApplication::translate("MainWindow", "Undo", nullptr));
#if QT_CONFIG(shortcut)
        actionUndo->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        actionRedo->setText(QCoreApplication::translate("MainWindow", "Redo", nullptr));
#if QT_CONFIG(shortcut)
        actionRedo->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionShow_All->setText(QCoreApplication::translate("MainWindow", "Show All", nullptr));
        actionStay_On_Top->setText(QCoreApplication::translate("MainWindow", "Stay On Top", nullptr));
        actionManual->setText(QCoreApplication::translate("MainWindow", "Manual..", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_3->setToolTip(QCoreApplication::translate("MainWindow", "Archive", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_3->setText(QString());
#if QT_CONFIG(tooltip)
        pushButton_4->setToolTip(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_4->setText(QString());
        cb_threshold_inactive->setText(QCoreApplication::translate("MainWindow", "Show Threshold", nullptr));
#if QT_CONFIG(tooltip)
        btn_Alphabetical->setToolTip(QCoreApplication::translate("MainWindow", "Toggle alpabetic sorting", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_Alphabetical->setText(QString());
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "search", nullptr));
#if QT_CONFIG(tooltip)
        context_lock->setToolTip(QCoreApplication::translate("MainWindow", "Lock the context", nullptr));
#endif // QT_CONFIG(tooltip)
        context_lock->setText(QString());
#if QT_CONFIG(tooltip)
        pushButton->setToolTip(QCoreApplication::translate("MainWindow", "Add item", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton->setText(QString());
#if QT_CONFIG(tooltip)
        pushButton_2->setToolTip(QCoreApplication::translate("MainWindow", "Remove item", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_2->setText(QString());
        lbl_newVersion->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p>New version is available at <a href=\"http://nerdur.com/todour-pl\"><span style=\" text-decoration: underline; color:#0000ff;\">https://nerdur.com/todour-pl</span></a></p></body></html>", nullptr));
        lbl_latestVersion->setText(QCoreApplication::translate("MainWindow", "You are running latest version", nullptr));
        txtLatestVersion->setText(QString());
        pb_closeVersionBar->setText(QString());
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        menuWindow->setTitle(QCoreApplication::translate("MainWindow", "Window", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
