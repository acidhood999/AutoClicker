#include <ClickLMR.h>

ClickLMR::ClickLMR()
{
}

void ClickLMR::setSettings(const SettingsClicker& clickSettings)
{
	click = std::make_unique<SettingsClicker>(clickSettings);
}

ClickLMR::~ClickLMR()
{
}