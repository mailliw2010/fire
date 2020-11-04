/********************************************************************************
** Form generated from reading UI file 'main_win.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_WIN_H
#define UI_MAIN_WIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include <qwebengineview.h>

QT_BEGIN_NAMESPACE

class Ui_View
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
    QToolBar *toolBar;

    void setupUi(QMainWindow *View)
    {
        if (View->objectName().isEmpty())
            View->setObjectName(QString::fromUtf8("View"));
        View->resize(936, 462);
        View->setMaximumSize(QSize(160000, 160000));
        View->setBaseSize(QSize(1024, 800));
        View->setStyleSheet(QString::fromUtf8("\n"
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
        actionQuit = new QAction(View);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionadd = new QAction(View);
        actionadd->setObjectName(QString::fromUtf8("actionadd"));
        actionSettings = new QAction(View);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        actionWebMode = new QAction(View);
        actionWebMode->setObjectName(QString::fromUtf8("actionWebMode"));
        actionCapture = new QAction(View);
        actionCapture->setObjectName(QString::fromUtf8("actionCapture"));
        actionTestButton = new QAction(View);
        actionTestButton->setObjectName(QString::fromUtf8("actionTestButton"));
        centralWidget = new QWidget(View);
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
        View->setCentralWidget(centralWidget);
        toolButton->raise();
        btnRefresh->raise();
        btnGo->raise();
        lineEdit->raise();
        m_webView->raise();
        menuBar = new QMenuBar(View);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 936, 18));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuTest = new QMenu(menuBar);
        menuTest->setObjectName(QString::fromUtf8("menuTest"));
        View->setMenuBar(menuBar);
        toolBar = new QToolBar(View);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        View->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTest->menuAction());
        menuFile->addAction(actionQuit);
        menuFile->addAction(actionSettings);
        menuTest->addAction(actionCapture);
        menuTest->addSeparator();
        menuTest->addSeparator();
        menuTest->addAction(actionTestButton);

        retranslateUi(View);

        QMetaObject::connectSlotsByName(View);
    } // setupUi

    void retranslateUi(QMainWindow *View)
    {
        View->setWindowTitle(QApplication::translate("View", "MainServer", nullptr));
        actionQuit->setText(QApplication::translate("View", "Quit", nullptr));
        actionadd->setText(QApplication::translate("View", "add", nullptr));
        actionSettings->setText(QApplication::translate("View", "Settings", nullptr));
        actionWebMode->setText(QApplication::translate("View", "WebMode", nullptr));
        actionCapture->setText(QApplication::translate("View", "Capture", nullptr));
        actionTestButton->setText(QApplication::translate("View", "TestButton", nullptr));
        toolButton->setText(QApplication::translate("View", "esc", nullptr));
        btnRefresh->setText(QApplication::translate("View", "Refresh", nullptr));
        lineEdit->setText(QApplication::translate("View", "http://localhost/index.html", nullptr));
        btnGo->setText(QApplication::translate("View", "Go", nullptr));
        menuFile->setTitle(QApplication::translate("View", "Configure", nullptr));
        menuTest->setTitle(QApplication::translate("View", "Test", nullptr));
        toolBar->setWindowTitle(QApplication::translate("View", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class View: public Ui_View {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WIN_H
