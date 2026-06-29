#include "Functional.h"

Functional::Functional(QWidget* parent) : QWidget(parent)
{
    toolsContainer = new QGridLayout(this);

    QGroupBox* clickIntervalGroup = new QGroupBox("Click Interval", this);
    QGridLayout* clickIntervalLayout = new QGridLayout(clickIntervalGroup);

    initializationInterval(lines);

    clickIntervalLayout->addWidget(lines[0], 0, 0);
    clickIntervalLayout->addWidget(new QLabel("h", this), 0, 1);
    clickIntervalLayout->addWidget(lines[1], 0, 2);
    clickIntervalLayout->addWidget(new QLabel("m", this), 0, 3);
    clickIntervalLayout->addWidget(lines[2], 0, 4);
    clickIntervalLayout->addWidget(new QLabel("s", this), 0, 5);
    clickIntervalLayout->addWidget(lines[3], 0, 6);
    clickIntervalLayout->addWidget(new QLabel("ms", this), 0, 7);

    QGroupBox* mouseButtonsSelectGroup = new QGroupBox("Click Options", this);
    QGridLayout* mouseButtonsSelectLayout = new QGridLayout(mouseButtonsSelectGroup);

    initializationMouseButtons(mouseButtonsSelect);
    mouseButtonsSelectLayout->addWidget(mouseButtonsSelect[0], 0, 0);
    mouseButtonsSelectLayout->addWidget(mouseButtonsSelect[1], 1, 0);



    QGroupBox* buttonsGroup = new QGroupBox(this);
    QGridLayout* buttonsLayout = new QGridLayout(buttonsGroup);

    initializationButtons(buttons);
    buttonsLayout->addWidget(buttons[0], 0, 0);
    buttonsLayout->addWidget(buttons[1], 0, 1);









    toolsContainer->addWidget(clickIntervalGroup, 0, 0);
    toolsContainer->addWidget(mouseButtonsSelectGroup, 1, 0);
    toolsContainer->addWidget(buttonsGroup, 2, 0);

   

    connect(buttons[0], &QPushButton::clicked, this, &Functional::buttonsClickStart);
    connect(buttons[1], &QPushButton::clicked, this, &Functional::buttonsClickStop);

}

void Functional::initializationInterval(QVector<QPointer<QLineEdit>>& lines)
{
 
    lines.resize(4);
    for (int i = 0; i < lines.size();++i)
    {
        lines[i] = new QLineEdit(this);
        lines[i]->setFocusPolicy(Qt::StrongFocus);
        lines[i]->installEventFilter(this);
        lines[i]->setAlignment(Qt::AlignRight);
        lines[i]->setContextMenuPolicy(Qt::NoContextMenu);
        lines[i]->setValidator(new QIntValidator(this));
    }
}

void Functional::initializationButtons(QVector<QPointer<QPushButton>>& buttons)
{
    buttons.resize(2);
    for (int i = 0; i < buttons.size();++i) buttons[i] = new QPushButton(this);

    buttons[0]->setText("Start");
    buttons[1]->setText("Stop");
    buttons[1]->setEnabled(false);
}

void Functional::initializationMouseButtons(QVector<QPointer<QComboBox>>& mouseButtonsSelect)
{
    mouseButtonsSelect.resize(2);

    mouseButtonsSelect[0] = new QComboBox(this);
    mouseButtonsSelect[1] = new QComboBox(this);

    mouseButtonsSelect[0]->addItem("Left", "Left");
    mouseButtonsSelect[0]->addItem("Middle", "Middle");
    mouseButtonsSelect[0]->addItem("Right", "Right");

    mouseButtonsSelect[1]->addItem("Single", true);
    mouseButtonsSelect[1]->addItem("Double", false);

    mouseButtonsSelectName["Left"] = { MOUSEEVENTF_LEFTDOWN , MOUSEEVENTF_LEFTUP };
    mouseButtonsSelectName["Middle"] = { MOUSEEVENTF_MIDDLEDOWN , MOUSEEVENTF_MIDDLEUP };
    mouseButtonsSelectName["Right"] = { MOUSEEVENTF_RIGHTDOWN , MOUSEEVENTF_RIGHTUP };
}

void Functional::ClickLMB(const QString& key)
{
    INPUT input = {0};
    const auto& event = mouseButtonsSelectName.at(key);
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = event[0];

    SendInput(1, &input, sizeof(INPUT));

    QThread::msleep(5);
    input.mi.dwFlags = event[1];

    SendInput(1, &input, sizeof(INPUT));
}

void Functional::buttonsClickStart()
{
    if (runCur) return;
    buttons[0]->setEnabled(false);
    buttons[1]->setEnabled(true);


    unsigned long long ms_time = lines[3]->text().toLongLong() + (lines[2]->text().toLongLong() * 1000) + (lines[1]->text().toLongLong() * 60000) + (lines[0]->text().toLongLong() * 3600000);
    if (ms_time < 10) ms_time = 10;
    runCur = true;

    QString selectedKey = mouseButtonsSelect[0]->currentData().toString();
    bool controlClick = mouseButtonsSelect[1]->currentData().toBool();

    future = QtConcurrent::run([this,ms_time,selectedKey, controlClick]()
    {
        while (runCur)
        {
            ClickLMB(selectedKey);
            QThread::msleep(ms_time);
            if (!controlClick) ClickLMB(selectedKey);
        }    
    });

}
void Functional::buttonsClickStop()
{  
    runCur = false;
    buttons[0]->setEnabled(true);
    buttons[1]->setEnabled(false);
}

Functional::~Functional()
{
    runCur = false;
    if (future.isRunning()) future.waitForFinished();
}


