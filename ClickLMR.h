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
		static std::unordered_map<QString, std::vector<int>> mouseButtonsSelectName = {
		{"Left",   {MOUSEEVENTF_LEFTDOWN,   MOUSEEVENTF_LEFTUP}},
		{"Middle", {MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP}},
		{"Right",  {MOUSEEVENTF_RIGHTDOWN,  MOUSEEVENTF_RIGHTUP}}
		};

		INPUT input = { 0 };
		const auto& event = mouseButtonsSelectName.at(key);
		input.type = INPUT_MOUSE;

		input.mi.dwFlags = event[0];
		SendInput(1, &input, sizeof(INPUT));

		QThread::msleep(10);

		input.mi.dwFlags = event[1];
		SendInput(1, &input, sizeof(INPUT));
	}

};

