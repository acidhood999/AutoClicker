#pragma once

#include <QtWidgets/QMainWindow>

#include <QPointer>
#include "ui_AutoClicker.h"
#include "Functional.h"

class AutoClicker : public QMainWindow
{
    Q_OBJECT

public:
    AutoClicker(QWidget *parent = nullptr);

    ~AutoClicker();

private:
    Ui::AutoClickerClass ui;
    QPointer<Functional> fun;
    

    void settingWindow();
};

