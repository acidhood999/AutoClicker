#pragma once

#include <QString>
#include <QWidget>
#include <windows.h>
#include <unordered_map>
#include <memory>
#include <QThread>
#include <atomic>
#include <QCoreApplication>
#include "include/NodeMarker.h"

struct SettingsClicker
{
	unsigned long long ms_time{ 0 }; // время
	unsigned long long ms_offset{ 0 };
	QString selectedKey{ "Left" }; // какая кнопка
	bool controlClick{ true }; // дабл клик
	int time_click{-1}; // сколько раз
	
	bool useCooedinates{ false };
	int x{ 0 };
	int y{ 0 };

	QList<QPoint> nodePoints;
}; 

class ClickLMR : public QObject
{
	Q_OBJECT
public:

	ClickLMR(QObject* parent = nullptr);
	void stop();

	~ClickLMR() = default;

public slots:
	void startClick(const SettingsClicker& settings);

signals:
	void finished();

private:
	std::atomic<bool> threadRun { false };

	void clickAt(int x, int y, const SettingsClicker& settings);
	void click(const SettingsClicker& settings) { clickAt(settings.x, settings.y, settings); }

	bool delay(const SettingsClicker& settings);
};

