/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include <qwebengineview.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionadd;
    QAction *actionSettings;
    QAction *actionWebMode;
    QAction *actionCapture;
    QAction *actionTestButton;
    QWidget *centralWidget;
    QToolButton *toolButton;
    QPushButton *btnRefresh;
    QLineEdit *lineEdit;
    QPushButton *btnGo;
    QWebEngineView *m_webView;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTest;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(936, 462);
        MainWindow->setMaximumSize(QSize(160000, 160000));
        MainWindow->setBaseSize(QSize(1024, 800));
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"QMenuBar {\n"
"	color:rgb(223,219,210);\n"
"	background-color:rgb(52, 101, 164);\n"
"}\n"
"QMenuBar::item {\n"
"	padding-top:4px;\n"
"	padding-left:4px;\n"
"	padding-right:4px;\n"
"	color:rgb(223,219,210);\n"
"	background-color:rgb(52, 101, 164);\n"
"}\n"
"QMenuBar::item:selected {\n"
"	color:rgb(255,255,255);\n"
"	padding-top:2px;\n"
"	padding-left:2px;\n"
"	padding-right:2px;\n"
"	border-top-width:2px;\n"
"	border-left-width:2px;\n"
"	border-right-width:2px;\n"
"	border-top-right-radius:4px;\n"
"	border-top-left-radius:4px;\n"
"	border-style:solid;\n"
"//	background-color:rgb(65,64,59);\n"
"    background-color:rgb(52, 101, 164);\n"
"	border-top-color: rgb(47,47,44);\n"
"	border-right-color: qlineargradient(spread:pad, x1:0, y1:1, x2:1, y2:0, stop:0 rgba(90, 87, 78, 255), stop:1 rgba(47,47,44, 255));\n"
"	border-left-color:  qlineargradient(spread:pad, x1:1, y1:0, x2:0, y2:0, stop:0 rgba(90, 87, 78, 255), stop:1 rgba(47,47,44, 255));\n"
"}\n"
"QMenu {\n"
"	color:rgb(223,219,210);\n"
"	background-color:r"
                        "gb(65,64,59);\n"
"}\n"
"QMenu::item {\n"
"	color:rgb(223,219,210);\n"
"	padding-left:20px;\n"
"	padding-top:4px;\n"
"	padding-bottom:4px;\n"
"	padding-right:10px;\n"
"}\n"
"QMenu::item:selected {\n"
"	color:rgb(255,255,255);\n"
"	background-color: qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(225, 108, 54, 255), stop:1 rgba(246, 134, 86, 255));\n"
"	border-style:solid;\n"
"	border-width:3px;\n"
"	padding-left:17px;\n"
"	padding-top:4px;\n"
"	padding-bottom:4px;\n"
"	padding-right:7px;\n"
"	border-bottom-color:qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(175,85,48,255), stop:1 rgba(236,114,67, 255));\n"
"	border-top-color:qlineargradient(spread:pad, x1:0.5, y1:1, x2:0.5, y2:0, stop:0 rgba(253,156,113,255), stop:1 rgba(205,90,46, 255));\n"
"	border-right-color:qlineargradient(spread:pad, x1:0, y1:0.5, x2:1, y2:0.5, stop:0 rgba(253,156,113,255), stop:1 rgba(205,90,46, 255));\n"
"	border-left-color:qlineargradient(spread:pad, x1:1, y1:0.5, x2:0, y2:0.5, stop:0 rgba(253,"
                        "156,113,255), stop:1 rgba(205,90,46, 255));\n"
"}"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionadd = new QAction(MainWindow);
        actionadd->setObjectName(QString::fromUtf8("actionadd"));
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        actionWebMode = new QAction(MainWindow);
        actionWebMode->setObjectName(QString::fromUtf8("actionWebMode"));
        actionCapture = new QAction(MainWindow);
        actionCapture->setObjectName(QString::fromUtf8("actionCapture"));
        actionTestButton = new QAction(MainWindow);
        actionTestButton->setObjectName(QString::fromUtf8("actionTestButton"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        toolButton = new QToolButton(centralWidget);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setGeometry(QRect(870, 0, 26, 24));
        btnRefresh = new QPushButton(centralWidget);
        btnRefresh->setObjectName(QString::fromUtf8("btnRefresh"));
        btnRefresh->setGeometry(QRect(800, 0, 61, 25));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(190, 0, 551, 25));
        btnGo = new QPushButton(centralWidget);
        btnGo->setObjectName(QString::fromUtf8("btnGo"));
        btnGo->setGeometry(QRect(750, 0, 41, 25));
        m_webView = new QWebEngineView(centralWidget);
        m_webView->setObjectName(QString::fromUtf8("m_webView"));
        m_webView->setGeometry(QRect(0, 40, 931, 401));
        m_webView->setMaximumSize(QSize(160000, 160000));
        m_webView->setSizeIncrement(QSize(10, 10));
        m_webView->setBaseSize(QSize(1024, 0));
        MainWindow->setCentralWidget(centralWidget);
        toolButton->raise();
        btnRefresh->raise();
        btnGo->raise();
        lineEdit->raise();
        m_webView->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 936, 18));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuTest = new QMenu(menuBar);
        menuTest->setObjectName(QString::fromUtf8("menuTest"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTest->menuAction());
        menuFile->addAction(actionQuit);
        menuFile->addAction(actionSettings);
        menuTest->addAction(actionCapture);
        menuTest->addSeparator();
        menuTest->addSeparator();
        menuTest->addAction(actionTestButton);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", nullptr));
        actionadd->setText(QApplication::translate("MainWindow", "add", nullptr));
        actionSettings->setText(QApplication::translate("MainWindow", "Settings", nullptr));
        actionWebMode->setText(QApplication::translate("MainWindow", "WebMode", nullptr));
        actionCapture->setText(QApplication::translate("MainWindow", "Capture", nullptr));
        actionTestButton->setText(QApplication::translate("MainWindow", "TestButton", nullptr));
        toolButton->setText(QApplication::translate("MainWindow", "esc", nullptr));
        btnRefresh->setText(QApplication::translate("MainWindow", "Refresh", nullptr));
        lineEdit->setText(QApplication::translate("MainWindow", "http://localhost/index.html", nullptr));
        btnGo->setText(QApplication::translate("MainWindow", "Go", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "Configure", nullptr));
        menuTest->setTitle(QApplication::translate("MainWindow", "Test", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
