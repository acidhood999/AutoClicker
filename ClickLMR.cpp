#include <ClickLMR.h>

ClickLMR::ClickLMR(QObject* parent) : QObject(parent), clickSettings(std::make_unique<SettingsClicker>()) {}

void ClickLMR::setSettings(const SettingsClicker& clickSettings)
{
	this->clickSettings = std::make_unique<SettingsClicker>(clickSettings);
}

ClickLMR::~ClickLMR()
{
}