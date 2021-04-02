/********************************************************************************
** Form generated from reading UI file 'test_task.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEST_TASK_H
#define UI_TEST_TASK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_test_taskClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *test_taskClass)
    {
        if (test_taskClass->objectName().isEmpty())
            test_taskClass->setObjectName(QString::fromUtf8("test_taskClass"));
        test_taskClass->resize(600, 400);
        menuBar = new QMenuBar(test_taskClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        test_taskClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(test_taskClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        test_taskClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(test_taskClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        test_taskClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(test_taskClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        test_taskClass->setStatusBar(statusBar);

        retranslateUi(test_taskClass);

        QMetaObject::connectSlotsByName(test_taskClass);
    } // setupUi

    void retranslateUi(QMainWindow *test_taskClass)
    {
        test_taskClass->setWindowTitle(QCoreApplication::translate("test_taskClass", "test_task", nullptr));
    } // retranslateUi

};

namespace Ui {
    class test_taskClass: public Ui_test_taskClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEST_TASK_H
