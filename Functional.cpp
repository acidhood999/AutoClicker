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


    QGroupBox* buttonsGroup = new QGroupBox(this);
    QGridLayout* buttonsLayout = new QGridLayout(buttonsGroup);

    initializationButtons(buttons);
    buttonsLayout->addWidget(buttons[0], 1, 0);
    buttonsLayout->addWidget(buttons[1],1, 1);









    toolsContainer->addWidget(clickIntervalGroup, 0, 0);
    toolsContainer->addWidget(buttonsGroup, 1, 0);

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

void Functional::ClickLMB()
{
    INPUT input;

    input.type = 0;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    input.mi.mouseData = 0;
    input.mi.time = 0;

    SendInput(1, &input, sizeof(INPUT));
    input.mi.dwFlags = 4;


    SendInput(1, &input, sizeof(INPUT));
}

void Functional::buttonsClickStart()
{

    buttons[0]->setEnabled(false);
    buttons[1]->setEnabled(true);

    unsigned long long ms_time = lines[3]->text().toLongLong() + (lines[2]->text().toLongLong() * 1000) + (lines[1]->text().toLongLong() * 60000) + (lines[0]->text().toLongLong() * 3600000);
    if (ms_time == 0) ms_time = 10;
    runCur = true;
   
    future = QtConcurrent::run([this,ms_time]()
    {
        while (runCur)
        {
            ClickLMB();
            QThread::msleep(ms_time);
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


