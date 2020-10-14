/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_6;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_3;
    QLineEdit *target1;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_4;
    QLineEdit *target2;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pushButton_5;
    QLineEdit *target3;
    QTableWidget *tableWidget;
    QLabel *Accesses;
    QTableWidget *tableWidgetResult;
    QLabel *text1;
    QLabel *Timer;
    QLabel *Message;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(887, 625);
        MainWindow->setAutoFillBackground(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(40, 10, 91, 41));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(40, 50, 91, 41));
        pushButton_6 = new QPushButton(centralwidget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(20, 460, 89, 25));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(220, 10, 541, 129));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton_3 = new QPushButton(layoutWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 213, 0); \n"
"color: rgb(0, 0, 0);"));

        verticalLayout->addWidget(pushButton_3);

        target1 = new QLineEdit(layoutWidget);
        target1->setObjectName(QString::fromUtf8("target1"));
        target1->setPlaceholderText(QString::fromUtf8("name to search"));

        verticalLayout->addWidget(target1);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pushButton_4 = new QPushButton(layoutWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setStyleSheet(QString::fromUtf8("QpushButton{\n"
"	background-color: #A9A9A9;\n"
"	border: 3px solid #696969;\n"
"}\n"
"QpushButton:pressed{\n"
"	background-color: #FFFAF0;\n"
"	border: 3px solid #696969;\n"
"}"));

        verticalLayout_2->addWidget(pushButton_4);

        target2 = new QLineEdit(layoutWidget);
        target2->setObjectName(QString::fromUtf8("target2"));

        verticalLayout_2->addWidget(target2);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        pushButton_5 = new QPushButton(layoutWidget);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setStyleSheet(QString::fromUtf8("QpushButton{\n"
"	background-color: #A9A9A9;\n"
"	border: 3px solid #696969;\n"
"}\n"
"QpushButton:pressed{\n"
"	background-color: #FFFAF0;\n"
"	border: 3px solid #696969;\n"
"}"));

        verticalLayout_3->addWidget(pushButton_5);

        target3 = new QLineEdit(layoutWidget);
        target3->setObjectName(QString::fromUtf8("target3"));

        verticalLayout_3->addWidget(target3);


        horizontalLayout->addLayout(verticalLayout_3);

        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(30, 190, 471, 261));
        tableWidget->setAutoFillBackground(false);
        Accesses = new QLabel(centralwidget);
        Accesses->setObjectName(QString::fromUtf8("Accesses"));
        Accesses->setGeometry(QRect(270, 470, 321, 31));
        tableWidgetResult = new QTableWidget(centralwidget);
        tableWidgetResult->setObjectName(QString::fromUtf8("tableWidgetResult"));
        tableWidgetResult->setGeometry(QRect(530, 240, 301, 101));
        tableWidgetResult->setAutoFillBackground(false);
        text1 = new QLabel(centralwidget);
        text1->setObjectName(QString::fromUtf8("text1"));
        text1->setGeometry(QRect(540, 200, 341, 41));
        Timer = new QLabel(centralwidget);
        Timer->setObjectName(QString::fromUtf8("Timer"));
        Timer->setGeometry(QRect(270, 500, 321, 31));
        Message = new QLabel(centralwidget);
        Message->setObjectName(QString::fromUtf8("Message"));
        Message->setGeometry(QRect(520, 350, 321, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 887, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "ISAM", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Sequential", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Search", nullptr));
        target1->setText(QString());
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Insert", nullptr));
        target2->setPlaceholderText(QCoreApplication::translate("MainWindow", "name,user,mail,pass", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        target3->setText(QString());
        target3->setPlaceholderText(QCoreApplication::translate("MainWindow", "name to delete", nullptr));
        Accesses->setText(QString());
        text1->setText(QCoreApplication::translate("MainWindow", "Result: ", nullptr));
        Timer->setText(QString());
        Message->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
