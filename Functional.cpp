#include "Functional.h"

Functional::Functional(QWidget* parent) : QWidget(parent)
{
    clickSettings = std::make_unique<SettingsClicker>();
    
    setupUi();

    RegisterHotKey((HWND)this->winId(), 999, currFsModifier, vk);
}

void Functional::setupUi()
{
    mainLayout = new QHBoxLayout(this);
    toolsContainer = new QGridLayout();

    initializationClickNodeList();

    QGroupBox* clickNodeGroup = new QGroupBox("Nodes", this);
    QVBoxLayout* nodeLayout = new QVBoxLayout(clickNodeGroup);

    //nodeLayout->setContentsMargins(0, 0, 0, 0);
    //nodeLayout->setSpacing(0);                   
    //clickNodeList->setFrameShape(QFrame::NoFrame);

    nodeLayout->addWidget(clickNodeList);

    QGroupBox* clickIntervalGroup = createClickIntervalGroup();
    QGroupBox* mouseButtonsSelectGroup = createMouseButtonsSelectGroup();
    QGroupBox* repeatClickGroup = createRepeatClickGroup();
    QGroupBox* buttonsGroup = createButtonsGroup();
    QGroupBox* modeSelection = createModeSelection();

    buttonsGroup->setFixedHeight(100);
    buttonsGroup->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    clickIntervalGroup->setFixedHeight(100);
    clickIntervalGroup->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    toolsContainer->addWidget(clickIntervalGroup, 0, 0, 1, 2);
    toolsContainer->addWidget(mouseButtonsSelectGroup, 1, 0, 1, 1);
    toolsContainer->addWidget(repeatClickGroup, 1, 1, 1, 1);
    toolsContainer->addWidget(modeSelection, 2, 0, 1, 2);
    toolsContainer->addWidget(buttonsGroup, 3, 0, 1, 2);
    
    mainLayout->addLayout(toolsContainer);
    mainLayout->addWidget(clickNodeGroup);

    initializationConnect();
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

void Functional::initializationButtonsOffset()
{
    initializationInterval(lineOffset);

    toolsOffsetH = new QHBoxLayout();
    btnMsgInfo = new QPushButton("?",this);
    offsetRand = new QCheckBox(this);

    lineOffset->setText("50");
    lineOffset->setFixedWidth(80);
    btnMsgInfo->setFixedWidth(28);
    offsetRand->setFixedWidth(28);

    btnMsgInfo->setDefault(true);

    toolsOffsetH->addWidget(btnMsgInfo);
    toolsOffsetH->addWidget(new QLabel("Random Offset", this));
    toolsOffsetH->addWidget(offsetRand);
    toolsOffsetH->addWidget(lineOffset);
    toolsOffsetH->addWidget(new QLabel("ms", this));
    toolsOffsetH->addStretch();
}

void Functional::initializationClickNodeList()
{
    clickNodeList = new QListWidget(this);

    clickNodeList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    clickNodeList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    clickNodeList->setFixedWidth(100);
   
}

void Functional::initializationModeSelection()
{
    selectCurrLocationCursor = new QRadioButton("Current location", this);
    selectNodesCursor = new QRadioButton("Multi cursors", this);
    selectCurrLocationCursor->setChecked(true);
    selectNodesCursor->setChecked(false);
}

void Functional::initializationConnect()
{
    connect(btnStart, &QPushButton::clicked, this, &Functional::buttonsClickStart);
    connect(btnStop, &QPushButton::clicked, this, &Functional::buttonsClickStop);
    connect(btnHotkey, &QPushButton::clicked, this, &Functional::buttonsClickHotkeySett);
    connect(btnMsgInfo, &QPushButton::clicked, this, &Functional::infoClicked);
}



void Functional::buttonsClickStart()
{ 
    if (clickThread && clickThread->isRunning()) return;// !!!!

    clickSettings->ms_time = lineMs->text().toULongLong() + (lineS->text().toLongLong() * 1000) + (lineM->text().toLongLong() * 60000) + (lineH->text().toLongLong() * 3600000);
    clickSettings->selectedKey = mouseButtonsSelect->currentData().toString();
    clickSettings->controlClick = mouseButtonsSelectClick->currentData().toBool();
    if (selectTimesBtnRepeat->isChecked()) clickSettings->time_click = selectTimes->value();
    else clickSettings->time_click = -1;
    if (offsetRand->isChecked())clickSettings->ms_offset = lineOffset->text().toULongLong();
    else clickSettings->ms_offset = 0;
   
 
    mouseClick = new ClickLMR();// !!!!
    clickThread = new QThread();// !!!!

    mouseClick->moveToThread(clickThread);// !!!!

    SettingsClicker settingsCopy = *clickSettings;

    connect(clickThread, &QThread::started, mouseClick, [this, settingsCopy]()
    {
        mouseClick->startClick(settingsCopy);
    });// !!!!

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

void Functional::toggleClick()
{
    if (clickThread && clickThread->isRunning()) buttonsClickStop();
    else buttonsClickStart();
}

void Functional::infoClicked()
{
    QString msg = "If interval is set to 100 milliseconds and Random offset is set to 50, then the actual value of interval is a random number in the range of 50 to 150.";
    QMessageBox::information(
        this,
        "Info",
        msg
    );

}

void Functional::buttonsClickHotkeySett()
{
    SettingHotkey dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        UnregisterHotKey((HWND)this->winId(), 999);
        if (RegisterHotKey((HWND)this->winId(), 999, dialog.getFsModifier(), dialog.getNumHotkey()))
        {
            vk = dialog.getNumHotkey();
            currFsModifier = dialog.getFsModifier();
            btnStart->setText("Start (" + dialog.getNumHotkeyString() + ")");
            btnStop->setText("Stop (" + dialog.getNumHotkeyString() + ")");
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

QGroupBox* Functional::createButtonsGroup()
{
    initializationButtons();

    QGroupBox* group = new QGroupBox(this);
    QGridLayout* layout = new QGridLayout(group);

    layout->addWidget(btnStart, 0, 0);
    layout->addWidget(btnStop, 0, 1);
    layout->addWidget(btnHotkey, 1, 0, 1, 2);
    //layout->addWidget(buttons[3], 1, 1);

    return group;
}

QGroupBox* Functional::createModeSelection()
{
    initializationModeSelection();

    QGroupBox* group = new QGroupBox("Cursor mode",this);
    QGridLayout* layout = new QGridLayout(group);

    layout->addWidget(selectCurrLocationCursor, 0, 0);
    layout->addWidget(selectNodesCursor, 0, 1);

    return group;
}

QGroupBox* Functional::createRepeatClickGroup()
{
    initializationTimesButtons();

    QGroupBox* group = new QGroupBox("Click Repeat", this);
    QGridLayout* layout = new QGridLayout(group);

    layout->addWidget(selectTimesBtnRepeat, 0, 0);
    layout->addWidget(selectTimes, 0, 1);
    layout->addWidget(new QLabel("times", this), 0, 2);
    layout->addWidget(selectTimesBtnRepeatUnStp, 1, 0,1,2);


    return group;
}

QGroupBox* Functional::createMouseButtonsSelectGroup()
{
    initializationMouseButtons();

    QGroupBox* group = new QGroupBox("Click Options", this);
    QGridLayout* layout = new QGridLayout(group);

    layout->addWidget(new QLabel("Mouse button:", this), 0, 0);
    layout->addWidget(mouseButtonsSelect, 0, 1);
    layout->addWidget(new QLabel("Click type:", this), 1, 0);
    layout->addWidget(mouseButtonsSelectClick, 1, 1);

    return group;
}

QGroupBox* Functional::createClickIntervalGroup()
{
    initializationInterval(lineH);
    initializationInterval(lineM);
    initializationInterval(lineS);
    initializationInterval(lineMs);
    initializationButtonsOffset();


    QGroupBox* group = new QGroupBox("Click Interval", this);
    QGridLayout* layout = new QGridLayout(group);

    layout->addWidget(lineH, 0, 0);
    layout->addWidget(new QLabel("h", this), 0, 1);
    layout->addWidget(lineM, 0, 2);
    layout->addWidget(new QLabel("m", this), 0, 3);
    layout->addWidget(lineS, 0, 4);
    layout->addWidget(new QLabel("s", this), 0, 5);
    layout->addWidget(lineMs, 0, 6);
    layout->addWidget(new QLabel("ms", this), 0, 7);

    layout->addLayout(toolsOffsetH, 1, 0, 1, 8);

    return group;
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


