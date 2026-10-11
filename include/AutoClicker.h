#pragma once

#include <QtWidgets/QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include "include/Functional.h"
#include "ui_AutoClicker.h"

class AutoClicker : public QMainWindow
{
    Q_OBJECT

public:
    AutoClicker(QWidget *parent = nullptr);
    ~AutoClicker() = default;
protected:
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::AutoClickerClass ui;
    Functional* fun;
    QSystemTrayIcon* trayIcon;
    QMenu* trayMenu;

    void settingWindow();
    void setupTrayIcon();
};

