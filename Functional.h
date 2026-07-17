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

class Functional : public QWidget
{
	Q_OBJECT
public:

	Functional(QWidget* parent);
	~Functional();	

private slots:

	void buttonsClickStart();
	void buttonsClickStop();

private:

	QPointer<QGridLayout> toolsContainer;

	QVector<QPointer<QLineEdit>> lines;
	QVector<QPointer<QPushButton>> buttons;
	QPointer<QSpinBox> selectTimes;
	QVector<QPointer<QRadioButton>> selectTimesBtn;
	QVector<QPointer<QComboBox>> mouseButtonsSelect;

	ClickLMR* mouseClick;
	QThread* clickThread;

	std::unique_ptr<SettingsClicker> clickSettings;
	


	QGroupBox* createClickIntervalGroup();
	QGroupBox* createMouseButtonsSelectGroup();
	QGroupBox* createRepeatClickGroup();
	QGroupBox* createButtonsGroup();

	void initializationInterval();
	void initializationButtons();
	void initializationMouseButtons();
	void initializationTimesButtons();

	void setupUi();


};