#pragma once

#include <QWidget>
#include <QPushButton>
#include <QPointer>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>
#include <windows.h>								
#include <stdio.h>	 
#include <QComboBox>
#include <QShortcut>
#include <QSpinBox>
#include <QRadioButton>
#include <QThread>
#include <QMessageBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QList>
#include <QListWidget>
#include <QSettings>
#include <QDir>
#include "include\PositionPicker.h"
#include "include\NodeMarker.h"
#include "include\ClickLMR.h"
#include "include\HotkeySetting.h"

class Functional : public QWidget
{
	Q_OBJECT
public:

	Functional(QWidget* parent);
	void hideMarkersForTray();
	void restoreMarkersFromTray();


	void setMarkersVisible(bool visible);
	QString getNodeItemText(int num, int x, int y) const;
	QSettings getSettings() const;

	~Functional();	

private slots:

	void buttonsClickStart();
	void buttonsClickStop();
	void toggleClick();
	void infoClicked();
	void buttonsClickHotkeySett();
	void buttonsClickReset(); 
	void selectPixelPosition();
	void addNodeButtonClicked();
	void delNodeButtonClicked();

protected:
	bool nativeEvent(const QByteArray &event, void* message, qintptr* result) override;

private:
	QPointer<QHBoxLayout> mainLayout;
	QPointer<QGridLayout> toolsContainer;
	QPointer<QHBoxLayout> toolsOffsetH;
	QPointer<QHBoxLayout> addAndDel;

	QPointer<QLineEdit> lineMs;
	QPointer<QLineEdit> lineS;
	QPointer<QLineEdit> lineM;
	QPointer<QLineEdit> lineH;

	QPointer<QLineEdit> lineX;
	QPointer<QLineEdit> lineY;

	QPointer<QPushButton> btnStart;
	QPointer<QPushButton> btnStop;
	QPointer<QPushButton> btnHotkey;
	QPointer<QPushButton> btnReset;
	QPointer<QPushButton> btnPosition;
	QPointer<QPushButton> btnAddNode;
	QPointer<QPushButton> btnDelNode;
	QPointer<QPushButton> btnMsgInfo;

	QPointer<QSpinBox> selectTimes;

	QPointer<QRadioButton> selectTimesBtnRepeat;
	QPointer<QRadioButton> selectTimesBtnRepeatUnStp;
	QPointer<QRadioButton> selectCurrLocationCursor;
	QPointer<QRadioButton> selectNodesCursor;
	QPointer<QRadioButton> selectNodeСursor;

	QPointer<QComboBox> mouseButtonsSelect;
	QPointer<QComboBox> mouseButtonsSelectClick;
	
	QPointer<QCheckBox> offsetRand;
	QPointer<QLineEdit> lineOffset;

	ClickLMR* mouseClick = nullptr;
	QThread* clickThread = nullptr;

	std::unique_ptr<SettingsClicker> clickSettings;
	
	UINT vk = VK_F6;
	UINT currFsModifier = 0;
	QString saveName = "F6";

	QPointer<QGroupBox> clickNodeGroup; 
	QPointer<QListWidget> clickNodeList;
	QList<NodeMarker*> nodeMarkers;

	QPointer <QScreen> currentScreen;
	QRect screenGeometry;

	QGroupBox* createClickIntervalGroup();
	QGroupBox* createMouseButtonsSelectGroup();
	QGroupBox* createRepeatClickGroup();
	QGroupBox* createButtonsGroup();
	QGroupBox* createModeSelection();

	void initializationInterval(QPointer<QLineEdit>& newLine);
	void initializationButtons();
	void initializationMouseButtons();
	void initializationTimesButtons();
	void initializationButtonsOffset();
	void initializationClickNodeList();
	void initializationModeSelection();
	void initializationScreenCoor();
	void initializationConnect();

	void setupUi();

	void loadInfo();
	void saveInfo();

	void setAutoStart(bool enable);


};