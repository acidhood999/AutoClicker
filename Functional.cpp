#include "Functional.h"

Functional::Functional(QWidget* parent) : QWidget(parent)
{
    toolsContainer = new QGridLayout(this);

    QGroupBox* clickIntervalGroup = new QGroupBox("Click Interval", this);
    QGridLayout* clickIntervalLayout = new QGridLayout(clickIntervalGroup);

    initializationInterval();

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

    initializationMouseButtons();
    mouseButtonsSelectLayout->addWidget(new QLabel("Mouse button:", this), 0, 0);
    mouseButtonsSelectLayout->addWidget(mouseButtonsSelect[0], 0, 1);
    mouseButtonsSelectLayout->addWidget(new QLabel("Click type:", this), 1, 0);
    mouseButtonsSelectLayout->addWidget(mouseButtonsSelect[1], 1, 1);

    QGroupBox* repeatClickGroup = new QGroupBox("Click Repeat", this);
    QGridLayout* repeatClickLayout = new QGridLayout(repeatClickGroup);

    initializationTimesButtons();
    repeatClickLayout->addWidget(selectTimesBtn[0], 0, 0);
    repeatClickLayout->addWidget(selectTimes, 0, 1);
    repeatClickLayout->addWidget(new QLabel("times", this), 0, 2);
    repeatClickLayout->addWidget(selectTimesBtn[1], 1, 0);

    QGroupBox* buttonsGroup = new QGroupBox(this);
    QGridLayout* buttonsLayout = new QGridLayout(buttonsGroup);

    initializationButtons();
    buttonsLayout->addWidget(buttons[0], 0, 0);
    buttonsLayout->addWidget(buttons[1], 0, 1);

    toolsContainer->addWidget(clickIntervalGroup, 0, 0, 1, 2);
    toolsContainer->addWidget(mouseButtonsSelectGroup, 1, 0, 1, 1);
    toolsContainer->addWidget(repeatClickGroup, 1, 1, 1, 1);
    toolsContainer->addWidget(buttonsGroup, 2, 0, 1, 2);

    

    connect(buttons[0], &QPushButton::clicked, this, &Functional::buttonsClickStart);
    connect(buttons[1], &QPushButton::clicked, this, &Functional::buttonsClickStop);


    QShortcut* startShortcut = new QShortcut(QKeySequence(Qt::Key_F5), this);
    connect(startShortcut, &QShortcut::activated , this, &Functional::buttonsClickStart);

    QShortcut* stopShortcut = new QShortcut(QKeySequence(Qt::Key_F6), this);
    connect(stopShortcut, &QShortcut::activated, this, &Functional::buttonsClickStop);


}

void Functional::initializationInterval()
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

void Functional::initializationButtons()
{
    buttons.resize(2);
    for (int i = 0; i < buttons.size();++i) buttons[i] = new QPushButton(this);

    buttons[0]->setText("Start");
    buttons[1]->setText("Stop");
    buttons[1]->setEnabled(false);
}

void Functional::initializationMouseButtons()
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

void Functional::initializationTimesButtons()
{
    selectTimesBtn.resize(2);
    selectTimesBtn[0] = new QRadioButton("Repeat", this);
    selectTimesBtn[1] = new QRadioButton("Repeat until sopped", this);
    selectTimesBtn[1]->setChecked(true);
    selectTimes = new QSpinBox(this);
    selectTimes->setMinimum(1);       
    selectTimes->setValue(1);
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

    int time_click = -1;
    if(selectTimesBtn[0]->isChecked()) time_click = selectTimes->value();

    future = QtConcurrent::run([this,ms_time,selectedKey, controlClick,time_click]() mutable
    {
        while (runCur)
        {
            if (time_click == 0) break;
            ClickLMB(selectedKey);
            QThread::msleep(ms_time);
            if (!controlClick) ClickLMB(selectedKey);
            if (time_click > 0) time_click--;
        }
        runCur = false;
        QMetaObject::invokeMethod(this, [this]() {
            buttons[0]->setEnabled(true);
            buttons[1]->setEnabled(false);
            });
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


