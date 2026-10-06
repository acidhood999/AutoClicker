#include "AutoClicker.h"

AutoClicker::AutoClicker(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    settingWindow();
    fun = new Functional(this);
    setCentralWidget(fun);
}

void AutoClicker::settingWindow()
{
    setWindowTitle("Auto Clicker");
    setFixedSize(600,400);
}

AutoClicker::~AutoClicker() {}



