#pragma once

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QPointer>
#include <QEventLoop>
#include <windows.h>
#include <QLineEdit>
#include <QGroupBox>

QString vkToString(UINT vk);


class SettingHotkey : public QDialog
{
	Q_OBJECT
public:
	SettingHotkey(QWidget* parent = nullptr);
	~SettingHotkey();

	UINT getNumHotkey();
	UINT getFsModifier();

	QString getNumHotkeyString() { return newHotKetString; }
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

	QString newHotKetString = "";

	void initializationButtons()
	{
		btnChangeHot = new QPushButton("Start / Stop", this);
		btnSave = new QPushButton("Save",this);
		btnCancel = new QPushButton("Cancel",this);
		outHotkey = new QLineEdit(this);
		outHotkey->setAlignment(Qt::AlignCenter);
		outHotkey->setText("F6");
		outHotkey->setReadOnly(true);
		outHotkey->setFont(QFont("Segoe UI", 12, QFont::Bold));
		buttSettings();
	}

	void buttSettings()
	{
		btnChangeHot->setFixedHeight(50);
		btnChangeHot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		
		btnSave->setFixedHeight(40);
		btnCancel->setFixedHeight(40);

		btnSave->setFixedWidth(80);
		btnCancel->setFixedWidth(80);

		

		btnSave->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		btnCancel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

		outHotkey->setFixedHeight(50);
		outHotkey->setFixedWidth(130);
		outHotkey->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	}

	QGroupBox* createButtonsGroup();
	
};