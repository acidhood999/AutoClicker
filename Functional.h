#pragma once

#include <QWidget>
#include <QPushButton>
#include <QPointer>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QVector>
#include <QIntValidator>
#include <windows.h>								
#include <stdio.h>	 
#include <QtConcurrent>
#include <atomic>
#include <QFuture>
#include <QComboBox>
#include <unordered_map>
#include <vector>
#include <QShortcut>
#include <QSpinBox>
#include <QRadioButton>
#include <ClickLMR.h>
#include <memory>
#include <QThread>
#include "HotkeySetting.h"
#include <QMessageBox>
#include <QCheckBox>
#include <QHBoxLayout>


class Functional : public QWidget
{
	Q_OBJECT
public:

	Functional(QWidget* parent);
	~Functional();	

private slots:

	void buttonsClickStart();
	void buttonsClickStop();
	void toggleClick();

	void buttonsClickHotkeySett();

protected:
	bool nativeEvent(const QByteArray &event, void* message, qintptr* result) override;

private:

	QPointer<QGridLayout> toolsContainer;
	QPointer<QHBoxLayout> toolsOffsetH;

	QPointer<QLineEdit> lineMs;
	QPointer<QLineEdit> lineS;
	QPointer<QLineEdit> lineM;
	QPointer<QLineEdit> lineH;


	QPointer<QPushButton> btnStart;
	QPointer<QPushButton> btnStop;
	QPointer<QPushButton> btnHotkey;

	QPointer<QSpinBox> selectTimes;

	QPointer<QRadioButton> selectTimesBtnRepeat;
	QPointer<QRadioButton> selectTimesBtnRepeatUnStp;

	QPointer<QComboBox> mouseButtonsSelect;
	QPointer<QComboBox> mouseButtonsSelectClick;
	

	QPointer<QCheckBox> offsetRand;
	QPointer<QLineEdit> lineOffset;

	ClickLMR* mouseClick = nullptr;
	QThread* clickThread = nullptr;

	std::unique_ptr<SettingsClicker> clickSettings;
	
	SettingHotkey* dialog = nullptr;
	UINT vk = VK_F6;
	UINT currFsModifier = 0;

	QGroupBox* createClickIntervalGroup();
	QGroupBox* createMouseButtonsSelectGroup();
	QGroupBox* createRepeatClickGroup();
	QGroupBox* createButtonsGroup();

	void initializationInterval(QPointer<QLineEdit>& newLine);
	void initializationButtons();
	void initializationMouseButtons();
	void initializationTimesButtons();
	void initializationButtonsOffset();

	void setupUi();


	

};