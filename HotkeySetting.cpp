#include "HotkeySetting.h"

HHOOK SettingHotkey::s_hHook = NULL;
SettingHotkey* SettingHotkey::s_instance = nullptr;

SettingHotkey::SettingHotkey(QWidget* parent) : QDialog(parent)
{
	setWindowTitle("Hotkey setting");
	resize(300, 200);

	setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

	QGridLayout* layout = new QGridLayout(this);
	initializationButtons();



	layout->addWidget(btnChangeHot, 0, 0);
	layout->addWidget(outHotkey, 0, 1);
	layout->addWidget(btnSave, 1, 0);
	layout->addWidget(btnCancel, 1, 1);

	connect(btnCancel,&QPushButton::clicked, this,&QDialog::close);

	connect(btnChangeHot, &QPushButton::clicked, this, &SettingHotkey::changeHotKey);

}

LRESULT SettingHotkey::RebindHotkey(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
	{
		KBDLLHOOKSTRUCT* pKey = (KBDLLHOOKSTRUCT*)lParam;
		DWORD vk = pKey->vkCode;
		if (vk != VK_LCONTROL && vk != VK_RCONTROL &&
			vk != VK_LSHIFT && vk != VK_RSHIFT &&
			vk != VK_LMENU && vk != VK_RMENU)
		{
			if (s_instance)
			{
				s_instance->saveVkCode = vk;
				s_instance->ctrlPress = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
				s_instance->altPress = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
				s_instance->shiftPress = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

				if (s_instance->m_loop) s_instance->m_loop->quit();
			}
			return 1;
		}
	}
	return CallNextHookEx(s_hHook, nCode, wParam, lParam);
}



void SettingHotkey::changeHotKey()
{
	btnChangeHot->setEnabled(false);
	btnSave->setEnabled(false);
	btnCancel->setEnabled(false);

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

	QString text = "";
	if (ctrlPress)  text += "Ctrl + ";
	if (altPress)   text += "Alt + ";
	if (shiftPress) text += "Shift + ";
	text += QString("VK_%1").arg(saveVkCode);
	outHotkey->setText(text);

	btnChangeHot->setEnabled(true);
	btnSave->setEnabled(true);
	btnCancel->setEnabled(true);
}

SettingHotkey::~SettingHotkey()
{
}

