#include "Functional.h"

Functional::Functional(QWidget* parent) : QWidget(parent)
{
    clickSettings = std::make_unique<SettingsClicker>();

    mouseClick = new ClickLMR();// !!!!
    clickThread = new QThread(this);// !!!!

    mouseClick->moveToThread(clickThread);// !!!!

    connect(clickThread, &QThread::started, mouseClick, &ClickLMR::startClick);// !!!!
    connect(mouseClick, &ClickLMR::finished, clickThread, &QThread::quit);// !!!!
    connect(mouseClick, &ClickLMR::finished, this, &Functional::buttonsClickStop);// !!!!

    setupUi();
}

void Functional::setupUi()
{
    toolsContainer = new QGridLayout(this);

    QGroupBox* clickIntervalGroup = createClickIntervalGroup();
    QGroupBox* mouseButtonsSelectGroup = createMouseButtonsSelectGroup();
    QGroupBox* repeatClickGroup = createRepeatClickGroup();
    QGroupBox* buttonsGroup = createButtonsGroup();

    toolsContainer->addWidget(clickIntervalGroup, 0, 0, 1, 2);
    toolsContainer->addWidget(mouseButtonsSelectGroup, 1, 0, 1, 1);
    toolsContainer->addWidget(repeatClickGroup, 1, 1, 1, 1);
    toolsContainer->addWidget(buttonsGroup, 2, 0, 1, 2);



    QShortcut* startShortcut = new QShortcut(QKeySequence(Qt::Key_F5), this);
    connect(startShortcut, &QShortcut::activated, this, &Functional::buttonsClickStart);

    QShortcut* stopShortcut = new QShortcut(QKeySequence(Qt::Key_F6), this);
    connect(stopShortcut, &QShortcut::activated, this, &Functional::buttonsClickStop);

    connect(buttons[0], &QPushButton::clicked, this, &Functional::buttonsClickStart);
    connect(buttons[1], &QPushButton::clicked, this, &Functional::buttonsClickStop);


 


}

QGroupBox* Functional::createClickIntervalGroup()
{
    QGroupBox* group = new QGroupBox("Click Interval", this);
    QGridLayout* layout = new QGridLayout(group);

    initializationInterval();

    layout->addWidget(lines[0], 0, 0);
    layout->addWidget(new QLabel("h", this), 0, 1);
    layout->addWidget(lines[1], 0, 2);
    layout->addWidget(new QLabel("m", this), 0, 3);
    layout->addWidget(lines[2], 0, 4);
    layout->addWidget(new QLabel("s", this), 0, 5);
    layout->addWidget(lines[3], 0, 6);
    layout->addWidget(new QLabel("ms", this), 0, 7);

    return group;
}


void Functional::initializationInterval()
{

    lines.resize(4);
    for (int i = 0; i < lines.size();++i)
    {
        lines[i] = new QLineEdit(this);
        lines[i]->setText("0");
        lines[i]->setFocusPolicy(Qt::StrongFocus);
        lines[i]->installEventFilter(this);
        lines[i]->setAlignment(Qt::AlignRight);
        lines[i]->setContextMenuPolicy(Qt::NoContextMenu);
        lines[i]->setValidator(new QIntValidator(this));
    }
    lines[3]->setText("100");
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



void Functional::buttonsClickStart()
{ 
    if (clickThread->isRunning()) return;// !!!!

    clickSettings->ms_time = lines[3]->text().toULongLong() + (lines[2]->text().toLongLong() * 1000) + (lines[1]->text().toLongLong() * 60000) + (lines[0]->text().toLongLong() * 3600000);
    clickSettings->selectedKey = mouseButtonsSelect[0]->currentData().toString();
    clickSettings->controlClick = mouseButtonsSelect[1]->currentData().toBool();
    if (selectTimesBtn[0]->isChecked()) clickSettings->time_click = selectTimes->value();
    else clickSettings->time_click = -1;

    mouseClick->setSettings(*clickSettings);

    buttons[0]->setEnabled(false);
    buttons[1]->setEnabled(true);
 
    clickThread->start();// !!!!
}

void Functional::buttonsClickStop()
{  
   
    mouseClick->stop(); // !!!!

 
    buttons[0]->setEnabled(true);
    buttons[1]->setEnabled(false);
}



QGroupBox* Functional::createMouseButtonsSelectGroup()
{

    QGroupBox* group = new QGroupBox("Click Options", this);
    QGridLayout* layout = new QGridLayout(group);

    initializationMouseButtons();

    layout->addWidget(new QLabel("Mouse button:", this), 0, 0);
    layout->addWidget(mouseButtonsSelect[0], 0, 1);
    layout->addWidget(new QLabel("Click type:", this), 1, 0);
    layout->addWidget(mouseButtonsSelect[1], 1, 1);

    return group;
}

QGroupBox* Functional::createRepeatClickGroup()
{
    QGroupBox* group = new QGroupBox("Click Repeat", this);
    QGridLayout* layout = new QGridLayout(group);

    initializationTimesButtons();

    layout->addWidget(selectTimesBtn[0], 0, 0);
    layout->addWidget(selectTimes, 0, 1);
    layout->addWidget(new QLabel("times", this), 0, 2);
    layout->addWidget(selectTimesBtn[1], 1, 0);

    return group;
}

QGroupBox* Functional::createButtonsGroup()
{
    QGroupBox* group = new QGroupBox(this);
    QGridLayout* layout = new QGridLayout(group);

    initializationButtons();

    layout->addWidget(buttons[0], 0, 0);
    layout->addWidget(buttons[1], 0, 1);


    return group;
}

Functional::~Functional()
{
    if (clickThread->isRunning()) {// !!!!
        mouseClick->stop(); // !!!!
        clickThread->quit(); // !!!!
        clickThread->wait(); // !!!!
    }
    delete mouseClick;// !!!!
}


