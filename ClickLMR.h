#pragma once

#include <QString>
#include <QWidget>
#include <windows.h>
#include <unordered_map>
#include <memory>
#include <QThread>
#include <atomic>
#include <QCoreApplication>

struct SettingsClicker
{
	unsigned long long ms_time{}; // время
	unsigned long long ms_offset{};
	QString selectedKey{ "Left" }; // какая кнопка
	bool controlClick{ true }; // дабл клик
	int time_click{-1}; // сколько раз
	
}; 

class ClickLMR : public QObject
{
	Q_OBJECT
public:

	ClickLMR(QObject* parent = nullptr);
	void stop();

	~ClickLMR();

public slots:

	void startClick(const SettingsClicker& settings)
	{
		threadRun = true;
		int timeClick = settings.time_click;

		for (;threadRun && timeClick != 0;)
		{
			click(settings.selectedKey);
			if (!settings.controlClick) click(settings.selectedKey);

			unsigned long long totalDelay = settings.ms_time;
			const unsigned long long step = 10;

			if (settings.ms_offset && settings.ms_offset > 0)
			{
				if (settings.ms_offset >= settings.ms_time) totalDelay = rand() % (settings.ms_offset + settings.ms_time) + 1;
				else if (totalDelay == 0) totalDelay = rand() % settings.ms_offset + 1;
				else totalDelay = rand() % (totalDelay) + settings.ms_offset+1;
			}

			while (threadRun && totalDelay > 0)
			{
				unsigned long long currSleep = totalDelay;
				if (totalDelay > step) currSleep = step;
				QThread::msleep(currSleep);
				totalDelay -= currSleep;
			}

			if (!threadRun)break;			
			if (timeClick > 0)timeClick--;
		}

		finished();
	}

signals:

	void finished();

private:

	std::atomic<bool> threadRun{ false };

	void click(const QString& key)
	{
		DWORD downFlag = MOUSEEVENTF_LEFTDOWN;
		DWORD upFlag = MOUSEEVENTF_LEFTUP;

		if (key == "Right") 
		{
			downFlag = MOUSEEVENTF_RIGHTDOWN;
			upFlag = MOUSEEVENTF_RIGHTUP;
		}
		else if (key == "Middle") 
		{
			downFlag = MOUSEEVENTF_MIDDLEDOWN;
			upFlag = MOUSEEVENTF_MIDDLEUP;
		}

		INPUT input = { 0 };
		input.type = INPUT_MOUSE;

		input.mi.dwFlags = downFlag;
		SendInput(1, &input, sizeof(INPUT));

		QThread::msleep(10);

		input.mi.dwFlags = upFlag;
		SendInput(1, &input, sizeof(INPUT));
	}

};

