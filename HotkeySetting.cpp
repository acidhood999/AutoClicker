#include "HotkeySetting.h"

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
	

}

SettingHotkey::~SettingHotkey()
{
}
