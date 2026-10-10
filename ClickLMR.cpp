#include <ClickLMR.h>

ClickLMR::ClickLMR(QObject* parent) : QObject(parent) {}

void ClickLMR::stop() { threadRun = false; }

ClickLMR::~ClickLMR() {}