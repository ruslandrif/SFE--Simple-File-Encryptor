#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_test_task.h"

class test_task : public QMainWindow
{
    Q_OBJECT

public:
    test_task(QWidget *parent = Q_NULLPTR);

private:
    Ui::test_taskClass ui;
};
