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
	std::atomic<bool> runCur{ false };

	void initializationInterval(QVector<QPointer<QLineEdit>>& lines);
	void initializationButtons(QVector<QPointer<QPushButton>>& buttons);

	void ClickLMB()										
	{
		INPUT input;									

		input.type = 0;									
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;							
		input.mi.mouseData = 0;							
		input.mi.time = 0;								

		SendInput(1, &input, sizeof(INPUT));			
		input.mi.dwFlags = 4;							
		Sleep(100);										

		SendInput(1, &input, sizeof(INPUT));			
	}

};