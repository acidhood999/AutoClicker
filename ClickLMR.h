#pragma once
#include <QString>
class ClickLMR
{
public:
	ClickLMR();

	~ClickLMR();
private:
	unsigned long long ms_time {};
	QString selectedKey {""};
	bool controlClick = true;
	int time_click{};

};

