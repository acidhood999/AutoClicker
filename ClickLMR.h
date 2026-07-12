#pragma once

#include <QString>
#include <QWidget>
#include <windows.h>
#include <memory>

struct SettingsClicker
{
	unsigned long long ms_time{ 10 }; // время
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



	~ClickLMR();

private:

	std::unique_ptr<SettingsClicker> click;

};

