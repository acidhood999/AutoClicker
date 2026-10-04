#include "Functional.h"

Functional::Functional(QWidget* parent) : QWidget(parent)
{
    clickSettings = std::make_unique<SettingsClicker>();
    
    setupUi();
    RegisterHotKey((HWND)this->winId(), 999, currFsModifier, vk);
}

void Functional::setupUi()
{
    toolsContainer = new QGridLayout(this);

    QGroupBox* clickIntervalGroup = createClickIntervalGroup();
    QGroupBox* mouseButtonsSelectGroup = createMouseButtonsSelectGroup();
    QGroupBox* repeatClickGroup = createRepeatClickGroup();
    QGroupBox* buttonsGroup = createButtonsGroup();

    buttonsGroup->setFixedHeight(150);
    buttonsGroup->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    toolsContainer->addWidget(clickIntervalGroup, 0, 0, 1, 2);
    toolsContainer->addWidget(mouseButtonsSelectGroup, 1, 0, 1, 1);
    toolsContainer->addWidget(repeatClickGroup, 1, 1, 1, 1);
    toolsContainer->addWidget(buttonsGroup, 2, 0, 1, 2);


    connect(btnStart, &QPushButton::clicked, this, &Functional::buttonsClickStart);
    connect(btnStop, &QPushButton::clicked, this, &Functional::buttonsClickStop);
    connect(btnHotkey, &QPushButton::clicked, this, &Functional::buttonsClickHotkeySett);
}

QGroupBox* Functional::createClickIntervalGroup()
{
    QGroupBox* group = new QGroupBox("Click Interval", this);
    QGridLayout* layout = new QGridLayout(group);

    initializationInterval(linesH);
    initializationInterval(linesM);
    initializationInterval(linesS);
    initializationInterval(linesMs);
    linesMs->setText("100");

    layout->addWidget(linesH, 0, 0);
    layout->addWidget(new QLabel("h", this), 0, 1);
    layout->addWidget(linesM, 0, 2);
    layout->addWidget(new QLabel("m", this), 0, 3);
    layout->addWidget(linesS, 0, 4);
    layout->addWidget(new QLabel("s", this), 0, 5);
    layout->addWidget(linesMs, 0, 6);
    layout->addWidget(new QLabel("ms", this), 0, 7);

    return group;
}


void Functional::initializationInterval(QPointer<QLineEdit>& newLine)
{
    newLine = new QLineEdit(this);
    newLine->setText("0");
    newLine->setFocusPolicy(Qt::StrongFocus);
    newLine->installEventFilter(this);
    newLine->setAlignment(Qt::AlignRight);
    newLine->setContextMenuPolicy(Qt::NoContextMenu);
    newLine->setValidator(new QIntValidator(this));
}

void Functional::initializationButtons()
{

    btnStart = new QPushButton(this);
    btnStop = new QPushButton(this);
    btnHotkey = new QPushButton(this);

    btnStart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    btnStop->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    btnHotkey->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    

    btnStart->setText("Start (" + vkToString(vk) + ")");
    btnStop->setText("Stop (" + vkToString(vk) + ")");
    btnStop->setEnabled(false);
    btnHotkey->setText("Hotkey setting");
}

void Functional::initializationMouseButtons()
{

    mouseButtonsSelect = new QComboBox(this);
    mouseButtonsSelectClick = new QComboBox(this);

    mouseButtonsSelect->addItem("Left", "Left");
    mouseButtonsSelect->addItem("Middle", "Middle");
    mouseButtonsSelect->addItem("Right", "Right");

    mouseButtonsSelectClick->addItem("Single", true);
    mouseButtonsSelectClick->addItem("Double", false);

   
}

void Functional::initializationTimesButtons()
{
   
    selectTimesBtnRepeat = new QRadioButton("Repeat", this);
    selectTimesBtnRepeatUnStp = new QRadioButton("Repeat until stopped", this);
    selectTimesBtnRepeatUnStp->setChecked(true);

    selectTimes = new QSpinBox(this);
    selectTimes->setMinimum(1);
    selectTimes->setValue(1);
}



void Functional::buttonsClickStart()
{ 
    if (clickThread && clickThread->isRunning()) return;// !!!!

    clickSettings->ms_time = linesMs->text().toULongLong() + (linesS->text().toLongLong() * 1000) + (linesM->text().toLongLong() * 60000) + (linesH->text().toLongLong() * 3600000);
    clickSettings->selectedKey = mouseButtonsSelect->currentData().toString();
    clickSettings->controlClick = mouseButtonsSelectClick->currentData().toBool();
    if (selectTimesBtnRepeat->isChecked()) clickSettings->time_click = selectTimes->value();
    else clickSettings->time_click = -1;
 
    mouseClick = new ClickLMR();// !!!!
    clickThread = new QThread();// !!!!

    mouseClick->setSettings(*clickSettings);

    mouseClick->moveToThread(clickThread);// !!!!

    connect(clickThread, &QThread::started, mouseClick, &ClickLMR::startClick);// !!!!

    connect(mouseClick, &ClickLMR::finished, clickThread, &QThread::quit);// !!!!
    connect(mouseClick, &ClickLMR::finished, mouseClick, &QObject::deleteLater);// !!!!
    connect(clickThread, &QThread::finished, clickThread, &QObject::deleteLater);// !!!!
    

    connect(clickThread, &QThread::finished, this, [this]() {
        btnStart->setEnabled(true);
        btnStop->setEnabled(false);
    });

    connect(clickThread, &QThread::destroyed, this, [this]() {
        clickThread = nullptr;
        mouseClick = nullptr;
    });

    btnStart->setEnabled(false);
    btnStop->setEnabled(true);

    clickThread->start();// !!!!
}

void Functional::buttonsClickStop()
{  
   
    if (mouseClick && clickThread && clickThread->isRunning()) mouseClick->stop(); // !!!!
 
    btnStart->setEnabled(true);
    btnStop->setEnabled(false);
}



QGroupBox* Functional::createMouseButtonsSelectGroup()
{

    QGroupBox* group = new QGroupBox("Click Options", this);
    QGridLayout* layout = new QGridLayout(group);

    initializationMouseButtons();

    layout->addWidget(new QLabel("Mouse button:", this), 0, 0);
    layout->addWidget(mouseButtonsSelect, 0, 1);
    layout->addWidget(new QLabel("Click type:", this), 1, 0);
    layout->addWidget(mouseButtonsSelectClick, 1, 1);

    return group;
}

QGroupBox* Functional::createRepeatClickGroup()
{
    QGroupBox* group = new QGroupBox("Click Repeat", this);
    QGridLayout* layout = new QGridLayout(group);

    initializationTimesButtons();

    layout->addWidget(selectTimesBtnRepeat, 0, 0);
    layout->addWidget(selectTimes, 0, 1);
    layout->addWidget(new QLabel("times", this), 0, 2);
    layout->addWidget(selectTimesBtnRepeatUnStp, 1, 0);

    return group;
}

QGroupBox* Functional::createButtonsGroup()
{
    QGroupBox* group = new QGroupBox(this);
    QGridLayout* layout = new QGridLayout(group);

    initializationButtons();

    layout->addWidget(btnStart, 0, 0);
    layout->addWidget(btnStop, 0, 1);
    layout->addWidget(btnHotkey, 1, 0);
    //layout->addWidget(buttons[3], 1, 1);

    return group;
}

void Functional::toggleClick()
{
    if (clickThread && clickThread->isRunning()) buttonsClickStop();
    else buttonsClickStart();
}

void Functional::buttonsClickHotkeySett()
{
    dialog = new SettingHotkey(this);

    if (dialog->exec() == QDialog::Accepted)
    {
        UnregisterHotKey((HWND)this->winId(), 999);
        if (RegisterHotKey((HWND)this->winId(), 999, dialog->getFsModifier(), dialog->getNumHotkey()))
        {
            vk = dialog->getNumHotkey();
            currFsModifier = dialog->getFsModifier();
            btnStart->setText("Start (" + dialog->getNumHotkeyString() + ")");
            btnStop->setText("Stop (" + dialog->getNumHotkeyString() + ")");
        }
        else RegisterHotKey((HWND)this->winId(), 999, currFsModifier, vk);
    }

}

bool Functional::nativeEvent(const QByteArray& event, void* message, qintptr* result)
{
    Q_UNUSED(event);
    Q_UNUSED(result);

    MSG* msg = static_cast<MSG*>(message);
    if (msg->message == WM_HOTKEY) {
        if (msg->wParam == 999) {
            toggleClick(); 
            return true;   
        }
    }

    return QWidget::nativeEvent(event, message, result);
}

Functional::~Functional()
{
    UnregisterHotKey((HWND)this->winId(), 999);

    if (clickThread && clickThread->isRunning()) {// !!!!
        if (mouseClick)mouseClick->stop(); // !!!!
        clickThread->quit(); // !!!!
        clickThread->wait(); // !!!!
    }
}


