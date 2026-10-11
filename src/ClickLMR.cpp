#include "include/ClickLMR.h"

ClickLMR::ClickLMR(QObject* parent) : QObject(parent) {}

void ClickLMR::startClick(const SettingsClicker& settings)
{
	threadRun = true;
	int timeClick = settings.time_click;

	for (;threadRun && timeClick != 0;)
	{
		if (settings.useCooedinates && !settings.nodePoints.isEmpty())
		{
			for (const QPoint& pt : settings.nodePoints)
			{
				if (!threadRun) break;
				clickAt(pt.x(), pt.y(), settings);
				if (!settings.controlClick) clickAt(pt.x(), pt.y(), settings);
				if (!delay(settings)) break;
			}
		}
		else
		{
			click(settings);
			if (!settings.controlClick) click(settings);
			if (!delay(settings)) break;
		}

		if (!threadRun)break;
		if (timeClick > 0)timeClick--;
	}

	finished();
}

void ClickLMR::clickAt(int x, int y, const SettingsClicker& settings)
{
	if(settings.useCooedinates)SetCursorPos(x, y);

	DWORD downFlag = MOUSEEVENTF_LEFTDOWN;
	DWORD upFlag = MOUSEEVENTF_LEFTUP;

	if (settings.selectedKey == "Right")
	{
		downFlag = MOUSEEVENTF_RIGHTDOWN;
		upFlag = MOUSEEVENTF_RIGHTUP;
	}
	else if (settings.selectedKey == "Middle")
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

bool ClickLMR::delay(const SettingsClicker& settings)
{
	unsigned long long totalDelay = settings.ms_time;const unsigned long long step = 10;
	if (settings.ms_offset && settings.ms_offset > 0)
	{
		if (settings.ms_offset >= settings.ms_time) totalDelay = rand() % (settings.ms_offset + settings.ms_time) + 1;
		else if (totalDelay == 0) totalDelay = rand() % settings.ms_offset + 1;
		else totalDelay = rand() % (totalDelay)+settings.ms_offset + 1;
	}
	while (threadRun && totalDelay > 0)
	{
		unsigned long long currSleep = totalDelay;
		if (totalDelay > step) currSleep = step;
		QThread::msleep(currSleep);
		totalDelay -= currSleep;
	}
	return threadRun;
}


void ClickLMR::stop() { threadRun = false; }
