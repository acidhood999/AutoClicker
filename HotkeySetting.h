#pragma once

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QPointer>
#include <QLineEdit>

class SettingHotkey : public QDialog
{
	Q_OBJECT
public:
	SettingHotkey(QWidget* parent = nullptr);
	~SettingHotkey();
private:
	QPointer<QPushButton> btnChangeHot;
	QPointer<QPushButton> btnSave;
	QPointer<QPushButton> btnCancel;
	QPointer<QLineEdit> outHotkey;
	void initializationButtons()
	{
		btnChangeHot = new QPushButton("Start / Stop", this);
		btnSave = new QPushButton("Save",this);
		btnCancel = new QPushButton("Cancel",this);
		outHotkey = new QLineEdit(this);

	}
};