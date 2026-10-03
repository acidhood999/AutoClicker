#include "HotkeySetting.h"

HHOOK SettingHotkey::s_hHook = NULL;
std::unique_ptr<SettingHotkey> SettingHotkey::s_instance = nullptr;

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

LRESULT SettingHotkey::RebindHotket(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
	{
		KBDLLHOOKSTRUCT* pKey = (KBDLLHOOKSTRUCT*)lParam;
		DWORD vk = pKey->vkCode;
		if (vk != VK_LCONTROL && vk != VK_RCONTROL &&
			vk != VK_LSHIFT && vk != VK_RSHIFT &&
			vk != VK_LMENU && vk != VK_RMENU)
		{
			s_instance->saveVkCode = vk;
			s_instance->ctrlPress = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
			s_instance->altPress = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
			s_instance->shiftPress = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

			

			PostQuitMessage(0);
			return 1;
		}
	}

	return CallNextHookEx(s_hHook, nCode, wParam, lParam);
}



void SettingHotkey::changeHotKey()
{
	
}

SettingHotkey::~SettingHotkey()
{
}

