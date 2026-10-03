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


private slots:
	
	void changeHotKey();

private:
	QPointer<QPushButton> btnChangeHot;
	QPointer<QPushButton> btnSave;
	QPointer<QPushButton> btnCancel;
	QPointer<QLineEdit> outHotkey;
	
	DWORD saveVkCode = 0;
	bool ctrlPress = false;
	bool altPress = false;
	bool shiftPress = false;

	static LRESULT CALLBACK RebindHotkey(int nCodem, WPARAM wParam, LPARAM lParam);

	static HHOOK s_hHook;
	static SettingHotkey* s_instance;
	QEventLoop* m_loop = nullptr;

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