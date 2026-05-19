#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AutoClicker.h"

class AutoClicker : public QMainWindow
{
    Q_OBJECT

public:
    AutoClicker(QWidget *parent = nullptr);
    ~AutoClicker();

private:
    Ui::AutoClickerClass ui;
};

