#pragma once

#include <QString>
#include <QWidget>
#include <windows.h>
#include <unordered_map>
#include <memory>
#include <QThread>

struct SettingsClicker
{
	unsigned long long ms_time{}; // время
	QString selectedKey{ "Left" }; // какая кнопка
	bool controlClick{ true }; // дабл клик
	int time_click{}; // сколько раз
}; 

class ClickLMR : public QObject
{
	Q_OBJECT
public:
	ClickLMR(QObject* parent = nullptr);

	void setSettings (const SettingsClicker& click);

	void startClick()
	{
		click();
	}

	~ClickLMR();

private:

	std::unique_ptr<SettingsClicker> clickSettings;

	std::unordered_map<QString, std::vector<int>> mouseButtonsSelectName = {
		{"Left",   {MOUSEEVENTF_LEFTDOWN,   MOUSEEVENTF_LEFTUP}},
		{"Middle", {MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP}},
		{"Right",  {MOUSEEVENTF_RIGHTDOWN,  MOUSEEVENTF_RIGHTUP}}
	};

	void click()
	{
		INPUT input = { 0 };
		const auto& event = mouseButtonsSelectName.at(clickSettings->selectedKey);
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = event[0];

		SendInput(1, &input, sizeof(INPUT));

		QThread::msleep(10);
		input.mi.dwFlags = event[1];

		SendInput(1, &input, sizeof(INPUT));
	}

};

