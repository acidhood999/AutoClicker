#pragma once

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QPointer>
#include <QEventLoop>
#include <windows.h>
#include <QLineEdit>

class SettingHotkey : public QDialog
{
	Q_OBJECT
public:
	SettingHotkey(QWidget* parent = nullptr);
	~SettingHotkey();

	UINT getNumHotkey() { return saveVkCode; }

	QString vkToString(UINT vk)
	{
		static const std::unordered_map<UINT, QString> keyHot = {
			{VK_F1, "F1"}, {VK_F2, "F2"}, {VK_F3, "F3"}, {VK_F4, "F4"},
			{VK_F5, "F5"}, {VK_F6, "F6"}, {VK_F7, "F7"}, {VK_F8, "F8"},
			{VK_F9, "F9"}, {VK_F10, "F10"}, {VK_F11, "F11"}, {VK_F12, "F12"},
			{VK_SPACE, "Space"}, {VK_RETURN, "Enter"}, {VK_ESCAPE, "Esc"},
			{VK_TAB, "Tab"}, {VK_SHIFT, "Shift"}, {VK_CONTROL, "Ctrl"}
		};

		if ((vk >= 'A' && vk <= 'Z') || (vk >= '0' && vk <= '9')) return QString(1, (char)vk);

		auto it = keyHot.find(vk);
		if (it != keyHot.end()) return it->second;
		return "Unknown";
	}

private slots:
	
	void changeHotKey();


private:
	QPointer<QPushButton> btnChangeHot;
	QPointer<QPushButton> btnSave;
	QPointer<QPushButton> btnCancel;
	QPointer<QLineEdit> outHotkey;
	
	DWORD saveVkCode = VK_F6;
	bool ctrlPress = false;
	bool altPress = false;
	bool shiftPress = false;

	static LRESULT CALLBACK RebindHotkey(int nCodem, WPARAM wParam, LPARAM lParam);

	static HHOOK s_hHook;
	static SettingHotkey* s_instance;
	QEventLoop* m_loop = nullptr;

	QString newTextVk = "";

	void initializationButtons()
	{
		btnChangeHot = new QPushButton("Start / Stop", this);
		btnSave = new QPushButton("Save",this);
		btnCancel = new QPushButton("Cancel",this);
		outHotkey = new QLineEdit(this);
		outHotkey->setAlignment(Qt::AlignCenter);
		outHotkey->setText("F6");
		outHotkey->setReadOnly(true);
	}

	
};