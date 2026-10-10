#include "HotkeySetting.h"

HHOOK SettingHotkey::s_hHook = NULL;
SettingHotkey* SettingHotkey::s_instance = nullptr;


QString vkToString(UINT vk)
{
	static const std::unordered_map<UINT, QString> keyHot = {
		{VK_F1, "F1"}, {VK_F2, "F2"}, {VK_F3, "F3"}, {VK_F4, "F4"},
		{VK_F5, "F5"}, {VK_F6, "F6"}, {VK_F7, "F7"}, {VK_F8, "F8"},
		{VK_F9, "F9"}, {VK_F10, "F10"}, {VK_F11, "F11"}, {VK_F12, "F12"},
		{VK_SPACE, "Space"}, {VK_RETURN, "Enter"}, {VK_ESCAPE, "Esc"},
		{VK_TAB, "Tab"}, {VK_SHIFT, "Shift"}, {VK_CONTROL, "Ctrl"},{VK_CAPITAL,"CapsLock"}
	};

	if ((vk >= 'A' && vk <= 'Z') || (vk >= '0' && vk <= '9')) return QString(1, (char)vk);

	auto it = keyHot.find(vk);
	if (it != keyHot.end()) return it->second;
	return "Unknown";
}


SettingHotkey::SettingHotkey(QWidget* parent) : QDialog(parent)
{
	setWindowTitle("Hotkey setting");
	resize(300, 100);

	setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

	QGridLayout* layout = new QGridLayout(this);

	QGroupBox* buttonsGroup = createButtonsGroup();
	layout->addWidget(buttonsGroup);

	connect(btnCancel,&QPushButton::clicked, this,&QDialog::close);
	connect(btnChangeHot, &QPushButton::clicked, this, &SettingHotkey::changeHotKey);
	connect(btnSave, &QPushButton::clicked, this, &QDialog::accept);
}

UINT SettingHotkey::getFsModifier()
{
	if (shiftPress) return MOD_SHIFT;
	if (ctrlPress) return MOD_CONTROL;
	if (altPress) return MOD_ALT;
	return 0;
}

UINT SettingHotkey::getNumHotkey() { return saveVkCode; }

QString SettingHotkey::getNumHotkeyString() { return newHotKeyString; }

void SettingHotkey::setTextButton(const QString& newHotKeyString) { outHotkey->setText(newHotKeyString); }

LRESULT SettingHotkey::RebindHotkey(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
	{
		KBDLLHOOKSTRUCT* pKey = (KBDLLHOOKSTRUCT*)lParam;
		DWORD vk = pKey->vkCode;
		if (s_instance)
		{
			if (vk == VK_LCONTROL || vk == VK_RCONTROL || vk == VK_CONTROL)
			{
				s_instance->ctrlPress = true;
				s_instance->outHotkey->setText("Ctrl + ");
				return 1;
			}
			else if (vk == VK_LSHIFT || vk == VK_RSHIFT || vk == VK_SHIFT)
			{
				s_instance->shiftPress = true;
				s_instance->outHotkey->setText("Shift + ");
				return 1;
			}
			else if (vk == VK_LMENU || vk == VK_RMENU || vk == VK_MENU)
			{
				s_instance->altPress = true;
				s_instance->outHotkey->setText("Alt + ");
				return 1;
			}
			else
			{
				s_instance->saveVkCode = vk;
				if (s_instance->m_loop) s_instance->m_loop->quit();
				return 1;
			}
		}

	}
	return CallNextHookEx(s_hHook, nCode, wParam, lParam);
}

QGroupBox* SettingHotkey::createButtonsGroup()
{
	QGroupBox* group = new QGroupBox(this);
	QGridLayout* layout = new QGridLayout(group);
	
	initializationButtons();

	layout->addWidget(btnChangeHot, 0, 0);
	layout->addWidget(outHotkey, 0, 1);
	layout->addWidget(btnSave, 1, 0, Qt::AlignCenter);
	layout->addWidget(btnCancel, 1, 1, Qt::AlignCenter);

	return group;
}



void SettingHotkey::changeHotKey()
{
	btnChangeHot->setEnabled(false);
	btnSave->setEnabled(false);
	btnCancel->setEnabled(false);

	newHotKeyString = "";
	shiftPress = false;
	ctrlPress = false;
	altPress = false;
	saveVkCode = 0;

	outHotkey->setText("Press hotkey");

	s_instance = this;
	s_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, RebindHotkey, GetModuleHandle(NULL), 0);

	if (s_hHook)
	{
		QEventLoop loop;
		m_loop = &loop;
		loop.exec();
		UnhookWindowsHookEx(s_hHook);
		s_hHook = NULL;
	}
	

	if (ctrlPress)  newHotKeyString = "Ctrl + ";
	if (altPress)   newHotKeyString = "Alt + ";
	if (shiftPress) newHotKeyString = "Shift + ";
	newHotKeyString += vkToString(saveVkCode);

	outHotkey->setText(newHotKeyString);

	btnChangeHot->setEnabled(true);
	btnSave->setEnabled(true);
	btnCancel->setEnabled(true);
}



SettingHotkey::~SettingHotkey()
{
}

