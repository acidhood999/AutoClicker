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


}
void Functional::buttonsClickStart()
{
    runCur = true;
        QtConcurrent::run([this]()
        {
                while (runCur)
                {
                    ClickLMB();
                }
        });
    
    

    
}
void Functional::buttonsClickStop()
{
    runCur = false;
   
}
Functional::~Functional()
{
}


