/********************************************************************************
** Form generated from reading UI file 'serialwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIALWINDOW_H
#define UI_SERIALWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SerialWindow
{
public:
    QFrame *frame_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QComboBox *comboBox_portName;
    QLabel *label_2;
    QLineEdit *lineEdit_Modtimeout;
    QComboBox *comboBox_buartRate;
    QLabel *label;
    QLabel *label_13;
    QLineEdit *lineEdit_ModDataBit;
    QLabel *label_43;
    QLineEdit *lineEdit_ModStopBit;
    QLabel *label_44;
    QWidget *tab_2;
    QLineEdit *lineEditHost_local;
    QPushButton *buttonConnect;
    QLabel *label_11;
    QLineEdit *lineEditHost;
    QPushButton *buttonConnect_local;
    QSpinBox *spinBoxPort_Remote;
    QSpinBox *spinBoxPort_local;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_6;
    QPushButton *buttonUnsubscribe;
    QLabel *label_9;
    QPushButton *buttonSubscribe;
    QLineEdit *lineEditTopic;
    QLineEdit *lineEdit_ProductID;
    QLabel *label_14;
    QLabel *label_26;
    QLineEdit *lineEdit_ProductNUM;
    QLabel *label_27;
    QLineEdit *lineEdit_ProductAREA;
    QLabel *label_28;
    QLineEdit *lineEdit_DevStart;
    QLabel *label_29;
    QLineEdit *lineEdit_DevEnd;
    QLabel *label_30;
    QLineEdit *lineEdit_FrameEach;
    QLabel *label_31;
    QLineEdit *lineEdit_TimeOut;
    QLabel *label_32;
    QLineEdit *lineEdit_ResendNum;
    QLabel *label_33;
    QLineEdit *lineEdit_PeriodTime;
    QLabel *label_34;
    QLabel *label_35;
    QLineEdit *lineEdit_RemoteUserN;
    QLineEdit *lineEdit_RemotePass;
    QLabel *label_36;
    QLabel *label_37;
    QLineEdit *lineEdit_LocalUserN;
    QLabel *label_38;
    QLineEdit *lineEdit_LocalPass;
    QLabel *label_39;
    QLabel *label_40;
    QLabel *label_41;
    QLabel *label_45;
    QLineEdit *lineEdit_ExtProductNUM;
    QLineEdit *lineEdit_ExtProductAREA;
    QLabel *label_46;
    QLineEdit *lineEdit_ExtProductID;
    QLabel *label_47;
    QWidget *tab_3;
    QCheckBox *DebugMode;
    QLabel *label_42;
    QPushButton *buttonSave;
    QPlainTextEdit *editLog;
    QLabel *label_16;
    QPushButton *buttonPublish;
    QLineEdit *lineEditTopic_2;
    QLabel *label_10;
    QLabel *label_12;
    QLineEdit *lineEditMessage;
    QFrame *line;
    QTextEdit *textEdit_read;
    QPushButton *pushButton_recvNum;
    QLabel *label_7;
    QPushButton *pushButton_emi_write;
    QPushButton *pushButton;
    QLabel *label_15;
    QRadioButton *radioButton_emi_2;
    QComboBox *comboBox_emi_write;
    QPushButton *pushButton_sendNum;
    QLineEdit *lineEdit_emi_write;
    QRadioButton *radioButton_emi;
    QLabel *label_4;
    QLabel *label_8;

    void setupUi(QWidget *SerialWindow)
    {
        if (SerialWindow->objectName().isEmpty())
            SerialWindow->setObjectName(QString::fromUtf8("SerialWindow"));
        SerialWindow->resize(1068, 637);
        SerialWindow->setStyleSheet(QString::fromUtf8("QMainWindow {\n"
"	background-image:url(://resource/resource/img/icon_color/Serialbackground .png);\n"
"}"));
        frame_2 = new QFrame(SerialWindow);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(10, 10, 1051, 611));
        QFont font;
        font.setFamily(QString::fromUtf8("Ubuntu Mono"));
        font.setBold(true);
        font.setWeight(75);
        frame_2->setFont(font);
        frame_2->setStyleSheet(QString::fromUtf8(""));
        frame_2->setFrameShape(QFrame::WinPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        tabWidget = new QTabWidget(frame_2);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(20, 10, 1021, 261));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        comboBox_portName = new QComboBox(tab);
        comboBox_portName->addItem(QString());
        comboBox_portName->addItem(QString());
        comboBox_portName->setObjectName(QString::fromUtf8("comboBox_portName"));
        comboBox_portName->setEnabled(true);
        comboBox_portName->setGeometry(QRect(70, 10, 111, 21));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Ubuntu Mono"));
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        comboBox_portName->setFont(font1);
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 40, 61, 21));
        label_2->setFont(font);
        label_2->setFrameShape(QFrame::StyledPanel);
        lineEdit_Modtimeout = new QLineEdit(tab);
        lineEdit_Modtimeout->setObjectName(QString::fromUtf8("lineEdit_Modtimeout"));
        lineEdit_Modtimeout->setGeometry(QRect(130, 130, 113, 22));
        comboBox_buartRate = new QComboBox(tab);
        comboBox_buartRate->addItem(QString());
        comboBox_buartRate->addItem(QString());
        comboBox_buartRate->addItem(QString());
        comboBox_buartRate->addItem(QString());
        comboBox_buartRate->setObjectName(QString::fromUtf8("comboBox_buartRate"));
        comboBox_buartRate->setEnabled(true);
        comboBox_buartRate->setGeometry(QRect(70, 40, 111, 21));
        comboBox_buartRate->setFont(font);
        comboBox_buartRate->setEditable(true);
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 61, 21));
        label->setFont(font);
        label->setFrameShape(QFrame::StyledPanel);
        label->setFrameShadow(QFrame::Sunken);
        label_13 = new QLabel(tab);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 130, 121, 21));
        label_13->setFont(font);
        label_13->setFrameShape(QFrame::StyledPanel);
        lineEdit_ModDataBit = new QLineEdit(tab);
        lineEdit_ModDataBit->setObjectName(QString::fromUtf8("lineEdit_ModDataBit"));
        lineEdit_ModDataBit->setGeometry(QRect(70, 70, 113, 22));
        label_43 = new QLabel(tab);
        label_43->setObjectName(QString::fromUtf8("label_43"));
        label_43->setGeometry(QRect(10, 70, 61, 21));
        label_43->setFont(font);
        label_43->setFrameShape(QFrame::StyledPanel);
        lineEdit_ModStopBit = new QLineEdit(tab);
        lineEdit_ModStopBit->setObjectName(QString::fromUtf8("lineEdit_ModStopBit"));
        lineEdit_ModStopBit->setGeometry(QRect(70, 100, 113, 22));
        label_44 = new QLabel(tab);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setGeometry(QRect(10, 100, 61, 21));
        label_44->setFont(font);
        label_44->setFrameShape(QFrame::StyledPanel);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        lineEditHost_local = new QLineEdit(tab_2);
        lineEditHost_local->setObjectName(QString::fromUtf8("lineEditHost_local"));
        lineEditHost_local->setGeometry(QRect(870, 100, 141, 21));
        lineEditHost_local->setFont(font);
        buttonConnect = new QPushButton(tab_2);
        buttonConnect->setObjectName(QString::fromUtf8("buttonConnect"));
        buttonConnect->setEnabled(false);
        buttonConnect->setGeometry(QRect(620, 130, 81, 21));
        buttonConnect->setStyleSheet(QString::fromUtf8(""));
        buttonConnect->setAutoDefault(false);
        buttonConnect->setFlat(false);
        label_11 = new QLabel(tab_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(790, 130, 71, 21));
        label_11->setFont(font);
        label_11->setFrameShape(QFrame::StyledPanel);
        label_11->setFrameShadow(QFrame::Sunken);
        label_11->setLineWidth(2);
        lineEditHost = new QLineEdit(tab_2);
        lineEditHost->setObjectName(QString::fromUtf8("lineEditHost"));
        lineEditHost->setGeometry(QRect(560, 100, 141, 21));
        lineEditHost->setFont(font);
        buttonConnect_local = new QPushButton(tab_2);
        buttonConnect_local->setObjectName(QString::fromUtf8("buttonConnect_local"));
        buttonConnect_local->setEnabled(false);
        buttonConnect_local->setGeometry(QRect(930, 130, 81, 21));
        buttonConnect_local->setStyleSheet(QString::fromUtf8(""));
        buttonConnect_local->setAutoDefault(false);
        buttonConnect_local->setFlat(false);
        spinBoxPort_Remote = new QSpinBox(tab_2);
        spinBoxPort_Remote->setObjectName(QString::fromUtf8("spinBoxPort_Remote"));
        spinBoxPort_Remote->setGeometry(QRect(560, 130, 51, 26));
        spinBoxPort_Remote->setMaximum(9999);
        spinBoxPort_Remote->setValue(1883);
        spinBoxPort_local = new QSpinBox(tab_2);
        spinBoxPort_local->setObjectName(QString::fromUtf8("spinBoxPort_local"));
        spinBoxPort_local->setGeometry(QRect(870, 130, 51, 26));
        spinBoxPort_local->setMaximum(9999);
        spinBoxPort_local->setValue(1883);
        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(480, 100, 71, 21));
        label_3->setFont(font);
        label_3->setFrameShape(QFrame::StyledPanel);
        label_3->setFrameShadow(QFrame::Sunken);
        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(480, 130, 71, 21));
        label_5->setFont(font);
        label_5->setFrameShape(QFrame::StyledPanel);
        label_5->setFrameShadow(QFrame::Sunken);
        label_5->setLineWidth(2);
        label_6 = new QLabel(tab_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(790, 100, 71, 21));
        label_6->setFont(font);
        label_6->setFrameShape(QFrame::StyledPanel);
        label_6->setFrameShadow(QFrame::Sunken);
        buttonUnsubscribe = new QPushButton(tab_2);
        buttonUnsubscribe->setObjectName(QString::fromUtf8("buttonUnsubscribe"));
        buttonUnsubscribe->setEnabled(false);
        buttonUnsubscribe->setGeometry(QRect(920, 200, 91, 25));
        buttonUnsubscribe->setStyleSheet(QString::fromUtf8(""));
        buttonUnsubscribe->setAutoDefault(false);
        buttonUnsubscribe->setFlat(false);
        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(480, 160, 61, 21));
        label_9->setFont(font);
        label_9->setFrameShape(QFrame::StyledPanel);
        label_9->setFrameShadow(QFrame::Sunken);
        buttonSubscribe = new QPushButton(tab_2);
        buttonSubscribe->setObjectName(QString::fromUtf8("buttonSubscribe"));
        buttonSubscribe->setEnabled(false);
        buttonSubscribe->setGeometry(QRect(830, 200, 91, 25));
        buttonSubscribe->setStyleSheet(QString::fromUtf8(""));
        buttonSubscribe->setAutoDefault(false);
        buttonSubscribe->setFlat(false);
        lineEditTopic = new QLineEdit(tab_2);
        lineEditTopic->setObjectName(QString::fromUtf8("lineEditTopic"));
        lineEditTopic->setEnabled(false);
        lineEditTopic->setGeometry(QRect(480, 180, 531, 25));
        lineEdit_ProductID = new QLineEdit(tab_2);
        lineEdit_ProductID->setObjectName(QString::fromUtf8("lineEdit_ProductID"));
        lineEdit_ProductID->setGeometry(QRect(110, 40, 81, 21));
        lineEdit_ProductID->setFont(font);
        label_14 = new QLabel(tab_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(20, 40, 81, 21));
        label_14->setFont(font);
        label_14->setFrameShape(QFrame::StyledPanel);
        label_14->setFrameShadow(QFrame::Sunken);
        label_26 = new QLabel(tab_2);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(20, 100, 81, 21));
        label_26->setFont(font);
        label_26->setFrameShape(QFrame::StyledPanel);
        label_26->setFrameShadow(QFrame::Sunken);
        lineEdit_ProductNUM = new QLineEdit(tab_2);
        lineEdit_ProductNUM->setObjectName(QString::fromUtf8("lineEdit_ProductNUM"));
        lineEdit_ProductNUM->setGeometry(QRect(110, 100, 81, 21));
        lineEdit_ProductNUM->setFont(font);
        label_27 = new QLabel(tab_2);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(20, 70, 81, 21));
        label_27->setFont(font);
        label_27->setFrameShape(QFrame::StyledPanel);
        label_27->setFrameShadow(QFrame::Sunken);
        lineEdit_ProductAREA = new QLineEdit(tab_2);
        lineEdit_ProductAREA->setObjectName(QString::fromUtf8("lineEdit_ProductAREA"));
        lineEdit_ProductAREA->setGeometry(QRect(110, 70, 81, 21));
        lineEdit_ProductAREA->setFont(font);
        label_28 = new QLabel(tab_2);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(20, 140, 81, 21));
        label_28->setFont(font);
        label_28->setFrameShape(QFrame::StyledPanel);
        label_28->setFrameShadow(QFrame::Sunken);
        lineEdit_DevStart = new QLineEdit(tab_2);
        lineEdit_DevStart->setObjectName(QString::fromUtf8("lineEdit_DevStart"));
        lineEdit_DevStart->setGeometry(QRect(110, 140, 61, 21));
        lineEdit_DevStart->setFont(font);
        label_29 = new QLabel(tab_2);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(190, 140, 21, 21));
        lineEdit_DevEnd = new QLineEdit(tab_2);
        lineEdit_DevEnd->setObjectName(QString::fromUtf8("lineEdit_DevEnd"));
        lineEdit_DevEnd->setGeometry(QRect(310, 140, 61, 21));
        lineEdit_DevEnd->setFont(font);
        label_30 = new QLabel(tab_2);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(220, 140, 81, 21));
        label_30->setFont(font);
        label_30->setFrameShape(QFrame::StyledPanel);
        label_30->setFrameShadow(QFrame::Sunken);
        lineEdit_FrameEach = new QLineEdit(tab_2);
        lineEdit_FrameEach->setObjectName(QString::fromUtf8("lineEdit_FrameEach"));
        lineEdit_FrameEach->setGeometry(QRect(110, 170, 61, 21));
        lineEdit_FrameEach->setFont(font);
        label_31 = new QLabel(tab_2);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(20, 170, 81, 21));
        label_31->setFont(font);
        label_31->setFrameShape(QFrame::StyledPanel);
        label_31->setFrameShadow(QFrame::Sunken);
        lineEdit_TimeOut = new QLineEdit(tab_2);
        lineEdit_TimeOut->setObjectName(QString::fromUtf8("lineEdit_TimeOut"));
        lineEdit_TimeOut->setGeometry(QRect(310, 170, 61, 21));
        lineEdit_TimeOut->setFont(font);
        label_32 = new QLabel(tab_2);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(220, 170, 81, 21));
        label_32->setFont(font);
        label_32->setFrameShape(QFrame::StyledPanel);
        label_32->setFrameShadow(QFrame::Sunken);
        lineEdit_ResendNum = new QLineEdit(tab_2);
        lineEdit_ResendNum->setObjectName(QString::fromUtf8("lineEdit_ResendNum"));
        lineEdit_ResendNum->setGeometry(QRect(110, 200, 61, 21));
        lineEdit_ResendNum->setFont(font);
        label_33 = new QLabel(tab_2);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(20, 200, 81, 21));
        label_33->setFont(font);
        label_33->setFrameShape(QFrame::StyledPanel);
        label_33->setFrameShadow(QFrame::Sunken);
        lineEdit_PeriodTime = new QLineEdit(tab_2);
        lineEdit_PeriodTime->setObjectName(QString::fromUtf8("lineEdit_PeriodTime"));
        lineEdit_PeriodTime->setGeometry(QRect(310, 200, 61, 21));
        lineEdit_PeriodTime->setFont(font);
        label_34 = new QLabel(tab_2);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(220, 200, 81, 21));
        label_34->setFont(font);
        label_34->setFrameShape(QFrame::StyledPanel);
        label_34->setFrameShadow(QFrame::Sunken);
        label_35 = new QLabel(tab_2);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setGeometry(QRect(480, 50, 81, 21));
        label_35->setFont(font);
        label_35->setFrameShape(QFrame::StyledPanel);
        label_35->setFrameShadow(QFrame::Sunken);
        lineEdit_RemoteUserN = new QLineEdit(tab_2);
        lineEdit_RemoteUserN->setObjectName(QString::fromUtf8("lineEdit_RemoteUserN"));
        lineEdit_RemoteUserN->setGeometry(QRect(570, 50, 61, 21));
        lineEdit_RemoteUserN->setFont(font);
        lineEdit_RemotePass = new QLineEdit(tab_2);
        lineEdit_RemotePass->setObjectName(QString::fromUtf8("lineEdit_RemotePass"));
        lineEdit_RemotePass->setGeometry(QRect(570, 70, 61, 21));
        lineEdit_RemotePass->setFont(font);
        lineEdit_RemotePass->setEchoMode(QLineEdit::Password);
        label_36 = new QLabel(tab_2);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setGeometry(QRect(480, 70, 81, 21));
        label_36->setFont(font);
        label_36->setFrameShape(QFrame::StyledPanel);
        label_36->setFrameShadow(QFrame::Sunken);
        label_37 = new QLabel(tab_2);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setGeometry(QRect(790, 70, 81, 21));
        label_37->setFont(font);
        label_37->setFrameShape(QFrame::StyledPanel);
        label_37->setFrameShadow(QFrame::Sunken);
        lineEdit_LocalUserN = new QLineEdit(tab_2);
        lineEdit_LocalUserN->setObjectName(QString::fromUtf8("lineEdit_LocalUserN"));
        lineEdit_LocalUserN->setGeometry(QRect(880, 50, 61, 21));
        lineEdit_LocalUserN->setFont(font);
        label_38 = new QLabel(tab_2);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        label_38->setGeometry(QRect(790, 50, 81, 21));
        label_38->setFont(font);
        label_38->setFrameShape(QFrame::StyledPanel);
        label_38->setFrameShadow(QFrame::Sunken);
        lineEdit_LocalPass = new QLineEdit(tab_2);
        lineEdit_LocalPass->setObjectName(QString::fromUtf8("lineEdit_LocalPass"));
        lineEdit_LocalPass->setGeometry(QRect(880, 70, 61, 21));
        lineEdit_LocalPass->setFont(font);
        lineEdit_LocalPass->setEchoMode(QLineEdit::Password);
        label_39 = new QLabel(tab_2);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setGeometry(QRect(520, 20, 111, 21));
        QFont font2;
        font2.setBold(true);
        font2.setUnderline(true);
        font2.setWeight(75);
        font2.setStrikeOut(false);
        label_39->setFont(font2);
        label_39->setAlignment(Qt::AlignCenter);
        label_40 = new QLabel(tab_2);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        label_40->setGeometry(QRect(800, 20, 111, 21));
        QFont font3;
        font3.setBold(true);
        font3.setUnderline(true);
        font3.setWeight(75);
        label_40->setFont(font3);
        label_40->setAlignment(Qt::AlignCenter);
        label_41 = new QLabel(tab_2);
        label_41->setObjectName(QString::fromUtf8("label_41"));
        label_41->setGeometry(QRect(80, 10, 111, 21));
        label_41->setFont(font2);
        label_41->setAlignment(Qt::AlignCenter);
        label_45 = new QLabel(tab_2);
        label_45->setObjectName(QString::fromUtf8("label_45"));
        label_45->setGeometry(QRect(210, 100, 101, 21));
        label_45->setFont(font);
        label_45->setFrameShape(QFrame::StyledPanel);
        label_45->setFrameShadow(QFrame::Sunken);
        lineEdit_ExtProductNUM = new QLineEdit(tab_2);
        lineEdit_ExtProductNUM->setObjectName(QString::fromUtf8("lineEdit_ExtProductNUM"));
        lineEdit_ExtProductNUM->setGeometry(QRect(320, 100, 81, 21));
        lineEdit_ExtProductNUM->setFont(font);
        lineEdit_ExtProductNUM->setDragEnabled(true);
        lineEdit_ExtProductAREA = new QLineEdit(tab_2);
        lineEdit_ExtProductAREA->setObjectName(QString::fromUtf8("lineEdit_ExtProductAREA"));
        lineEdit_ExtProductAREA->setGeometry(QRect(320, 70, 81, 21));
        lineEdit_ExtProductAREA->setFont(font);
        label_46 = new QLabel(tab_2);
        label_46->setObjectName(QString::fromUtf8("label_46"));
        label_46->setGeometry(QRect(210, 40, 101, 21));
        label_46->setFont(font);
        label_46->setFrameShape(QFrame::StyledPanel);
        label_46->setFrameShadow(QFrame::Sunken);
        lineEdit_ExtProductID = new QLineEdit(tab_2);
        lineEdit_ExtProductID->setObjectName(QString::fromUtf8("lineEdit_ExtProductID"));
        lineEdit_ExtProductID->setGeometry(QRect(320, 40, 81, 21));
        lineEdit_ExtProductID->setFont(font);
        label_47 = new QLabel(tab_2);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        label_47->setGeometry(QRect(210, 70, 101, 21));
        label_47->setFont(font);
        label_47->setFrameShape(QFrame::StyledPanel);
        label_47->setFrameShadow(QFrame::Sunken);
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        DebugMode = new QCheckBox(tab_3);
        DebugMode->setObjectName(QString::fromUtf8("DebugMode"));
        DebugMode->setGeometry(QRect(20, 60, 281, 20));
        label_42 = new QLabel(tab_3);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        label_42->setGeometry(QRect(20, 30, 251, 16));
        QFont font4;
        font4.setBold(true);
        font4.setWeight(75);
        label_42->setFont(font4);
        tabWidget->addTab(tab_3, QString());
        buttonSave = new QPushButton(frame_2);
        buttonSave->setObjectName(QString::fromUtf8("buttonSave"));
        buttonSave->setGeometry(QRect(460, 280, 80, 25));
        buttonSave->setStyleSheet(QString::fromUtf8(""));
        buttonSave->setAutoDefault(false);
        buttonSave->setFlat(false);
        editLog = new QPlainTextEdit(frame_2);
        editLog->setObjectName(QString::fromUtf8("editLog"));
        editLog->setGeometry(QRect(500, 460, 511, 141));
        editLog->setMaximumBlockCount(1000);
        label_16 = new QLabel(frame_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(500, 440, 71, 21));
        label_16->setFont(font);
        label_16->setFrameShape(QFrame::StyledPanel);
        label_16->setFrameShadow(QFrame::Sunken);
        buttonPublish = new QPushButton(frame_2);
        buttonPublish->setObjectName(QString::fromUtf8("buttonPublish"));
        buttonPublish->setGeometry(QRect(940, 400, 80, 25));
        buttonPublish->setStyleSheet(QString::fromUtf8(""));
        buttonPublish->setAutoDefault(false);
        buttonPublish->setFlat(false);
        lineEditTopic_2 = new QLineEdit(frame_2);
        lineEditTopic_2->setObjectName(QString::fromUtf8("lineEditTopic_2"));
        lineEditTopic_2->setGeometry(QRect(500, 350, 411, 25));
        label_10 = new QLabel(frame_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(500, 380, 61, 21));
        label_10->setFont(font);
        label_10->setFrameShape(QFrame::StyledPanel);
        label_10->setFrameShadow(QFrame::Sunken);
        label_12 = new QLabel(frame_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(500, 330, 61, 21));
        label_12->setFont(font);
        label_12->setFrameShape(QFrame::StyledPanel);
        label_12->setFrameShadow(QFrame::Sunken);
        lineEditMessage = new QLineEdit(frame_2);
        lineEditMessage->setObjectName(QString::fromUtf8("lineEditMessage"));
        lineEditMessage->setGeometry(QRect(500, 400, 411, 25));
        line = new QFrame(frame_2);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(10, 310, 1031, 20));
        QFont font5;
        font5.setBold(false);
        font5.setWeight(50);
        line->setFont(font5);
        line->setFrameShadow(QFrame::Sunken);
        line->setLineWidth(4);
        line->setMidLineWidth(1);
        line->setFrameShape(QFrame::HLine);
        textEdit_read = new QTextEdit(frame_2);
        textEdit_read->setObjectName(QString::fromUtf8("textEdit_read"));
        textEdit_read->setGeometry(QRect(10, 490, 461, 91));
        textEdit_read->setFont(font);
        pushButton_recvNum = new QPushButton(frame_2);
        pushButton_recvNum->setObjectName(QString::fromUtf8("pushButton_recvNum"));
        pushButton_recvNum->setGeometry(QRect(280, 470, 41, 20));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_recvNum->sizePolicy().hasHeightForWidth());
        pushButton_recvNum->setSizePolicy(sizePolicy);
        pushButton_recvNum->setFont(font);
        pushButton_recvNum->setAutoFillBackground(false);
        pushButton_recvNum->setStyleSheet(QString::fromUtf8(""));
        pushButton_recvNum->setInputMethodHints(Qt::ImhNone);
        pushButton_recvNum->setAutoDefault(false);
        pushButton_recvNum->setFlat(true);
        label_7 = new QLabel(frame_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(350, 470, 71, 21));
        label_7->setFont(font);
        label_7->setFrameShape(QFrame::Box);
        label_7->setFrameShadow(QFrame::Sunken);
        label_7->setTextFormat(Qt::RichText);
        label_7->setAlignment(Qt::AlignCenter);
        pushButton_emi_write = new QPushButton(frame_2);
        pushButton_emi_write->setObjectName(QString::fromUtf8("pushButton_emi_write"));
        pushButton_emi_write->setGeometry(QRect(420, 390, 51, 41));
        pushButton_emi_write->setFont(font);
        pushButton_emi_write->setStyleSheet(QString::fromUtf8(""));
        pushButton_emi_write->setAutoDefault(false);
        pushButton_emi_write->setFlat(false);
        pushButton = new QPushButton(frame_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(410, 580, 61, 21));
        pushButton->setFont(font);
        label_15 = new QLabel(frame_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(30, 350, 71, 21));
        label_15->setFont(font);
        label_15->setStyleSheet(QString::fromUtf8(""));
        label_15->setFrameShape(QFrame::StyledPanel);
        label_15->setFrameShadow(QFrame::Sunken);
        radioButton_emi_2 = new QRadioButton(frame_2);
        radioButton_emi_2->setObjectName(QString::fromUtf8("radioButton_emi_2"));
        radioButton_emi_2->setGeometry(QRect(10, 370, 112, 31));
        radioButton_emi_2->setFont(font);
        radioButton_emi_2->setStyleSheet(QString::fromUtf8("QCheckBox {\n"
"	padding:2px;\n"
"}"));
        comboBox_emi_write = new QComboBox(frame_2);
        comboBox_emi_write->setObjectName(QString::fromUtf8("comboBox_emi_write"));
        comboBox_emi_write->setGeometry(QRect(30, 420, 371, 31));
        comboBox_emi_write->setFont(font);
        comboBox_emi_write->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"	color:rgb(81,72,65);\n"
"	background: #ffffff;\n"
"}\n"
"QComboBox:editable {\n"
"	background: #ffffff;\n"
"	color: rgb(81,72,65);\n"
"	selection-color:rgb(81,72,65);\n"
"	selection-background-color: #ffffff;\n"
"}\n"
"QComboBox QAbstractItemView {\n"
"	color:rgb(81,72,65);	\n"
"	background: #ffffff;\n"
"	selection-color: #ffffff;\n"
"	selection-background-color: rgb(246, 134, 86);\n"
"}\n"
"QComboBox:!editable:on, QComboBox::drop-down:editable:on {\n"
"	color:  #1e1d23;	\n"
"	background: #ffffff;\n"
"}"));
        pushButton_sendNum = new QPushButton(frame_2);
        pushButton_sendNum->setObjectName(QString::fromUtf8("pushButton_sendNum"));
        pushButton_sendNum->setGeometry(QRect(430, 470, 41, 20));
        sizePolicy.setHeightForWidth(pushButton_sendNum->sizePolicy().hasHeightForWidth());
        pushButton_sendNum->setSizePolicy(sizePolicy);
        pushButton_sendNum->setFont(font);
        pushButton_sendNum->setAutoFillBackground(false);
        pushButton_sendNum->setStyleSheet(QString::fromUtf8(""));
        pushButton_sendNum->setInputMethodHints(Qt::ImhNone);
        pushButton_sendNum->setAutoDefault(false);
        pushButton_sendNum->setFlat(true);
        lineEdit_emi_write = new QLineEdit(frame_2);
        lineEdit_emi_write->setObjectName(QString::fromUtf8("lineEdit_emi_write"));
        lineEdit_emi_write->setGeometry(QRect(30, 370, 371, 31));
        lineEdit_emi_write->setFont(font);
        radioButton_emi = new QRadioButton(frame_2);
        radioButton_emi->setObjectName(QString::fromUtf8("radioButton_emi"));
        radioButton_emi->setGeometry(QRect(10, 400, 131, 23));
        radioButton_emi->setFont(font);
        radioButton_emi->setStyleSheet(QString::fromUtf8("QCheckBox {\n"
"	padding:2px;\n"
"}"));
        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(200, 470, 71, 21));
        label_4->setFont(font);
        label_4->setFrameShape(QFrame::Box);
        label_4->setFrameShadow(QFrame::Sunken);
        label_4->setTextFormat(Qt::RichText);
        label_4->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(SerialWindow);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(990, 620, 72, 21));
        label_8->setFont(font);
        QWidget::setTabOrder(comboBox_buartRate, textEdit_read);
        QWidget::setTabOrder(textEdit_read, comboBox_portName);
        QWidget::setTabOrder(comboBox_portName, comboBox_emi_write);
        QWidget::setTabOrder(comboBox_emi_write, radioButton_emi);
        QWidget::setTabOrder(radioButton_emi, lineEdit_emi_write);
        QWidget::setTabOrder(lineEdit_emi_write, pushButton_emi_write);
        QWidget::setTabOrder(pushButton_emi_write, radioButton_emi_2);

        retranslateUi(SerialWindow);

        tabWidget->setCurrentIndex(1);
        buttonConnect->setDefault(false);
        buttonConnect_local->setDefault(false);
        buttonUnsubscribe->setDefault(false);
        buttonSubscribe->setDefault(false);
        buttonSave->setDefault(false);
        buttonPublish->setDefault(false);
        pushButton_emi_write->setDefault(false);
        pushButton_sendNum->setDefault(false);


        QMetaObject::connectSlotsByName(SerialWindow);
    } // setupUi

    void retranslateUi(QWidget *SerialWindow)
    {
        SerialWindow->setWindowTitle(QApplication::translate("SerialWindow", "SerialWindow", nullptr));
        comboBox_portName->setItemText(0, QApplication::translate("SerialWindow", "/dev/ttyUSB0", nullptr));
        comboBox_portName->setItemText(1, QApplication::translate("SerialWindow", "/dev/tty5", nullptr));

        label_2->setText(QApplication::translate("SerialWindow", "Baurd", nullptr));
        lineEdit_Modtimeout->setText(QApplication::translate("SerialWindow", "500", nullptr));
        comboBox_buartRate->setItemText(0, QApplication::translate("SerialWindow", "9600", nullptr));
        comboBox_buartRate->setItemText(1, QApplication::translate("SerialWindow", "600", nullptr));
        comboBox_buartRate->setItemText(2, QApplication::translate("SerialWindow", "14400", nullptr));
        comboBox_buartRate->setItemText(3, QApplication::translate("SerialWindow", "115200", nullptr));

        label->setText(QApplication::translate("SerialWindow", "Port", nullptr));
        label_13->setText(QApplication::translate("SerialWindow", "Response TimeOut", nullptr));
        lineEdit_ModDataBit->setText(QApplication::translate("SerialWindow", "8", nullptr));
        label_43->setText(QApplication::translate("SerialWindow", "DataBit", nullptr));
        lineEdit_ModStopBit->setText(QApplication::translate("SerialWindow", "1", nullptr));
        label_44->setText(QApplication::translate("SerialWindow", "StopBit", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SerialWindow", "UartConfig", nullptr));
        lineEditHost_local->setText(QApplication::translate("SerialWindow", "localhost", nullptr));
        buttonConnect->setText(QApplication::translate("SerialWindow", "Connect", nullptr));
        label_11->setText(QApplication::translate("SerialWindow", "Port:", nullptr));
        lineEditHost->setText(QApplication::translate("SerialWindow", "183.66.138.122", nullptr));
        buttonConnect_local->setText(QApplication::translate("SerialWindow", "Connect", nullptr));
        label_3->setText(QApplication::translate("SerialWindow", "RemotHost:", nullptr));
        label_5->setText(QApplication::translate("SerialWindow", "Port:", nullptr));
        label_6->setText(QApplication::translate("SerialWindow", "LocalHost:", nullptr));
        buttonUnsubscribe->setText(QApplication::translate("SerialWindow", "Unsubscribe", nullptr));
        label_9->setText(QApplication::translate("SerialWindow", "Topic:", nullptr));
        buttonSubscribe->setText(QApplication::translate("SerialWindow", "Subscribe", nullptr));
        lineEditTopic->setText(QApplication::translate("SerialWindow", "FIRE/CQ-BS/5/#", nullptr));
        lineEdit_ProductID->setText(QApplication::translate("SerialWindow", "FIRE", nullptr));
        label_14->setText(QApplication::translate("SerialWindow", "ProductID:", nullptr));
        label_26->setText(QApplication::translate("SerialWindow", "ProductNUM:", nullptr));
        lineEdit_ProductNUM->setText(QApplication::translate("SerialWindow", "2", nullptr));
        label_27->setText(QApplication::translate("SerialWindow", "ProductAREA:", nullptr));
        lineEdit_ProductAREA->setText(QApplication::translate("SerialWindow", "CQ-BS", nullptr));
        label_28->setText(QApplication::translate("SerialWindow", "DevID Start:", nullptr));
        lineEdit_DevStart->setText(QApplication::translate("SerialWindow", "1", nullptr));
        label_29->setText(QApplication::translate("SerialWindow", "---", nullptr));
        lineEdit_DevEnd->setText(QApplication::translate("SerialWindow", "21", nullptr));
        label_30->setText(QApplication::translate("SerialWindow", "DevID End:", nullptr));
        lineEdit_FrameEach->setText(QApplication::translate("SerialWindow", "1024", nullptr));
        label_31->setText(QApplication::translate("SerialWindow", "FrameEach:", nullptr));
        lineEdit_TimeOut->setText(QApplication::translate("SerialWindow", "5", nullptr));
        label_32->setText(QApplication::translate("SerialWindow", "TimeOut(s):", nullptr));
        lineEdit_ResendNum->setText(QApplication::translate("SerialWindow", "3", nullptr));
        label_33->setText(QApplication::translate("SerialWindow", "ResendNum:", nullptr));
        lineEdit_PeriodTime->setText(QApplication::translate("SerialWindow", "8", nullptr));
        label_34->setText(QApplication::translate("SerialWindow", "PeriodT(S):", nullptr));
        label_35->setText(QApplication::translate("SerialWindow", "User:", nullptr));
        lineEdit_RemoteUserN->setText(QApplication::translate("SerialWindow", "admin", nullptr));
        lineEdit_RemotePass->setText(QApplication::translate("SerialWindow", "publish", nullptr));
        label_36->setText(QApplication::translate("SerialWindow", "Password:", nullptr));
        label_37->setText(QApplication::translate("SerialWindow", "Password:", nullptr));
        lineEdit_LocalUserN->setText(QApplication::translate("SerialWindow", "admin", nullptr));
        label_38->setText(QApplication::translate("SerialWindow", "User:", nullptr));
        lineEdit_LocalPass->setText(QApplication::translate("SerialWindow", "publish", nullptr));
        label_39->setText(QApplication::translate("SerialWindow", "Remote Mqtt", nullptr));
        label_40->setText(QApplication::translate("SerialWindow", "Local Mqtt", nullptr));
        label_41->setText(QApplication::translate("SerialWindow", "Public Mqtt", nullptr));
        label_45->setText(QApplication::translate("SerialWindow", "ExtProductNUM:", nullptr));
        lineEdit_ExtProductNUM->setText(QApplication::translate("SerialWindow", "2", nullptr));
        lineEdit_ExtProductAREA->setText(QApplication::translate("SerialWindow", "CQ-BS", nullptr));
        label_46->setText(QApplication::translate("SerialWindow", "ExtProductID:", nullptr));
        lineEdit_ExtProductID->setText(QApplication::translate("SerialWindow", "MCCB", nullptr));
        label_47->setText(QApplication::translate("SerialWindow", "ExtProductAREA:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("SerialWindow", "MqttConfig", nullptr));
        DebugMode->setText(QApplication::translate("SerialWindow", "Debug Mode(unchecked is Release Mode)", nullptr));
        label_42->setText(QApplication::translate("SerialWindow", "Set to Debug Mode or Release Mode:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("SerialWindow", "SysConfig", nullptr));
        buttonSave->setText(QApplication::translate("SerialWindow", "Save", nullptr));
        label_16->setText(QApplication::translate("SerialWindow", "Receive:", nullptr));
        buttonPublish->setText(QApplication::translate("SerialWindow", "Publish", nullptr));
        lineEditTopic_2->setText(QApplication::translate("SerialWindow", "FIRE/CQ-BS/5/Cmd", nullptr));
        label_10->setText(QApplication::translate("SerialWindow", "Send:", nullptr));
        label_12->setText(QApplication::translate("SerialWindow", "Topic:", nullptr));
        lineEditMessage->setText(QApplication::translate("SerialWindow", "ON|1|2|3|4|5|6|7|8|10|11|12|13|14|15|16|17|18", nullptr));
        pushButton_recvNum->setText(QApplication::translate("SerialWindow", "0", nullptr));
        label_7->setText(QApplication::translate("SerialWindow", "Send Num", nullptr));
        pushButton_emi_write->setText(QApplication::translate("SerialWindow", "Send", nullptr));
        pushButton->setText(QApplication::translate("SerialWindow", "Reload", nullptr));
        label_15->setText(QApplication::translate("SerialWindow", "Send:", nullptr));
        radioButton_emi_2->setText(QString());
        pushButton_sendNum->setText(QApplication::translate("SerialWindow", "0", nullptr));
        radioButton_emi->setText(QApplication::translate("SerialWindow", "Select Groups", nullptr));
        label_4->setText(QApplication::translate("SerialWindow", "Received", nullptr));
        label_8->setText(QApplication::translate("SerialWindow", "V00.02", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SerialWindow: public Ui_SerialWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIALWINDOW_H
