/********************************************************************************
** Form generated from reading UI file 'FireForm.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIREFORM_H
#define UI_FIREFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FireForm
{
public:
    QFrame *frame_2;
    QLabel *label_10;
    QLabel *label_3;
    QPushButton *buttonPublish;
    QPushButton *buttonSubscribe;
    QLabel *label_5;
    QLineEdit *lineEditMessage;
    QSpinBox *spinBoxPort;
    QLineEdit *lineEditTopic;
    QLabel *label_9;
    QPushButton *buttonConnect;
    QPlainTextEdit *editLog;
    QPushButton *buttonUnsubscribe;
    QLineEdit *lineEditTopic_2;
    QLabel *label_12;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QRadioButton *radioButton_4;
    QComboBox *comboBoxHost;
    QFrame *line;
    QPushButton *buttonClear;
    QLabel *label_8;

    void setupUi(QWidget *FireForm)
    {
        if (FireForm->objectName().isEmpty())
            FireForm->setObjectName(QString::fromUtf8("FireForm"));
        FireForm->resize(1480, 615);
        FireForm->setStyleSheet(QString::fromUtf8(""));
        frame_2 = new QFrame(FireForm);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(10, 10, 1461, 591));
        QFont font;
        font.setFamily(QString::fromUtf8("Ubuntu Mono"));
        font.setBold(true);
        font.setWeight(75);
        frame_2->setFont(font);
        frame_2->setStyleSheet(QString::fromUtf8(""));
        frame_2->setFrameShape(QFrame::WinPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        label_10 = new QLabel(frame_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(840, 90, 61, 21));
        label_10->setFont(font);
        label_10->setFrameShape(QFrame::StyledPanel);
        label_10->setFrameShadow(QFrame::Sunken);
        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 10, 71, 21));
        label_3->setFont(font);
        label_3->setFrameShape(QFrame::StyledPanel);
        label_3->setFrameShadow(QFrame::Sunken);
        buttonPublish = new QPushButton(frame_2);
        buttonPublish->setObjectName(QString::fromUtf8("buttonPublish"));
        buttonPublish->setGeometry(QRect(1370, 110, 80, 25));
        buttonPublish->setStyleSheet(QString::fromUtf8(""));
        buttonSubscribe = new QPushButton(frame_2);
        buttonSubscribe->setObjectName(QString::fromUtf8("buttonSubscribe"));
        buttonSubscribe->setGeometry(QRect(420, 110, 81, 25));
        buttonSubscribe->setStyleSheet(QString::fromUtf8(""));
        label_5 = new QLabel(frame_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 40, 71, 21));
        label_5->setFont(font);
        label_5->setFrameShape(QFrame::StyledPanel);
        label_5->setFrameShadow(QFrame::Sunken);
        label_5->setLineWidth(2);
        lineEditMessage = new QLineEdit(frame_2);
        lineEditMessage->setObjectName(QString::fromUtf8("lineEditMessage"));
        lineEditMessage->setGeometry(QRect(840, 110, 521, 25));
        spinBoxPort = new QSpinBox(frame_2);
        spinBoxPort->setObjectName(QString::fromUtf8("spinBoxPort"));
        spinBoxPort->setGeometry(QRect(120, 40, 61, 26));
        spinBoxPort->setMaximum(9999);
        spinBoxPort->setValue(1883);
        lineEditTopic = new QLineEdit(frame_2);
        lineEditTopic->setObjectName(QString::fromUtf8("lineEditTopic"));
        lineEditTopic->setGeometry(QRect(10, 110, 401, 25));
        label_9 = new QLabel(frame_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 90, 61, 21));
        label_9->setFont(font);
        label_9->setFrameShape(QFrame::StyledPanel);
        label_9->setFrameShadow(QFrame::Sunken);
        buttonConnect = new QPushButton(frame_2);
        buttonConnect->setObjectName(QString::fromUtf8("buttonConnect"));
        buttonConnect->setGeometry(QRect(200, 40, 81, 25));
        buttonConnect->setStyleSheet(QString::fromUtf8(""));
        editLog = new QPlainTextEdit(frame_2);
        editLog->setObjectName(QString::fromUtf8("editLog"));
        editLog->setGeometry(QRect(10, 150, 1441, 411));
        editLog->setMaximumBlockCount(1000);
        buttonUnsubscribe = new QPushButton(frame_2);
        buttonUnsubscribe->setObjectName(QString::fromUtf8("buttonUnsubscribe"));
        buttonUnsubscribe->setGeometry(QRect(510, 110, 91, 25));
        buttonUnsubscribe->setStyleSheet(QString::fromUtf8(""));
        lineEditTopic_2 = new QLineEdit(frame_2);
        lineEditTopic_2->setObjectName(QString::fromUtf8("lineEditTopic_2"));
        lineEditTopic_2->setGeometry(QRect(840, 50, 521, 25));
        label_12 = new QLabel(frame_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(840, 30, 61, 21));
        label_12->setFont(font);
        label_12->setFrameShape(QFrame::StyledPanel);
        label_12->setFrameShadow(QFrame::Sunken);
        radioButton = new QRadioButton(frame_2);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setGeometry(QRect(910, 90, 41, 20));
        radioButton->setChecked(true);
        radioButton_2 = new QRadioButton(frame_2);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setGeometry(QRect(960, 90, 41, 20));
        radioButton_3 = new QRadioButton(frame_2);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));
        radioButton_3->setGeometry(QRect(1020, 90, 71, 20));
        radioButton_4 = new QRadioButton(frame_2);
        radioButton_4->setObjectName(QString::fromUtf8("radioButton_4"));
        radioButton_4->setGeometry(QRect(1110, 90, 91, 20));
        comboBoxHost = new QComboBox(frame_2);
        comboBoxHost->addItem(QString());
        comboBoxHost->addItem(QString());
        comboBoxHost->addItem(QString());
        comboBoxHost->setObjectName(QString::fromUtf8("comboBoxHost"));
        comboBoxHost->setGeometry(QRect(120, 10, 161, 21));
        comboBoxHost->setEditable(true);
        line = new QFrame(frame_2);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(650, 10, 20, 131));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        buttonClear = new QPushButton(frame_2);
        buttonClear->setObjectName(QString::fromUtf8("buttonClear"));
        buttonClear->setGeometry(QRect(1370, 560, 81, 25));
        buttonClear->setStyleSheet(QString::fromUtf8(""));
        label_10->raise();
        label_3->raise();
        buttonPublish->raise();
        buttonSubscribe->raise();
        label_5->raise();
        lineEditMessage->raise();
        spinBoxPort->raise();
        lineEditTopic->raise();
        label_9->raise();
        buttonConnect->raise();
        buttonUnsubscribe->raise();
        editLog->raise();
        lineEditTopic_2->raise();
        label_12->raise();
        radioButton->raise();
        radioButton_2->raise();
        radioButton_3->raise();
        radioButton_4->raise();
        comboBoxHost->raise();
        line->raise();
        buttonClear->raise();
        label_8 = new QLabel(FireForm);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(1510, 590, 72, 31));
        label_8->setFont(font);

        retranslateUi(FireForm);

        QMetaObject::connectSlotsByName(FireForm);
    } // setupUi

    void retranslateUi(QWidget *FireForm)
    {
        FireForm->setWindowTitle(QApplication::translate("FireForm", "FireForm", nullptr));
        label_10->setText(QApplication::translate("FireForm", "Send:", nullptr));
        label_3->setText(QApplication::translate("FireForm", "RemotHost:", nullptr));
        buttonPublish->setText(QApplication::translate("FireForm", "Publish", nullptr));
        buttonSubscribe->setText(QApplication::translate("FireForm", "Subscribe", nullptr));
        label_5->setText(QApplication::translate("FireForm", "Port:", nullptr));
        lineEditMessage->setText(QApplication::translate("FireForm", "ON|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18", nullptr));
        lineEditTopic->setText(QApplication::translate("FireForm", "FIRE/CQ-BS/2/#", nullptr));
        label_9->setText(QApplication::translate("FireForm", "Topic:", nullptr));
        buttonConnect->setText(QApplication::translate("FireForm", "Connect", nullptr));
        buttonUnsubscribe->setText(QApplication::translate("FireForm", "Unsubscribe", nullptr));
        lineEditTopic_2->setText(QApplication::translate("FireForm", "FIRE/CQ-BS/2/Cmd", nullptr));
        label_12->setText(QApplication::translate("FireForm", "Topic:", nullptr));
        radioButton->setText(QApplication::translate("FireForm", "ON", nullptr));
        radioButton_2->setText(QApplication::translate("FireForm", "OFF", nullptr));
        radioButton_3->setText(QApplication::translate("FireForm", "ClearPw", nullptr));
        radioButton_4->setText(QApplication::translate("FireForm", "ClearFaul", nullptr));
        comboBoxHost->setItemText(0, QApplication::translate("FireForm", "183.66.138.122", nullptr));
        comboBoxHost->setItemText(1, QApplication::translate("FireForm", "192.168.10.33", nullptr));
        comboBoxHost->setItemText(2, QApplication::translate("FireForm", "192.168.10.4", nullptr));

        buttonClear->setText(QApplication::translate("FireForm", "Clear", nullptr));
        label_8->setText(QApplication::translate("FireForm", "V00.02", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FireForm: public Ui_FireForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIREFORM_H
