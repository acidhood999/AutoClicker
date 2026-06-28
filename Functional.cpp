#include "Functional.h"

Functional::Functional(QWidget* parent) : QWidget(parent)
{
    toolsContainer = new QGridLayout(this);

    QGroupBox* resolutionGroup = new QGroupBox("Resolution", this);
    QGridLayout* resolutionLayout = new QGridLayout(resolutionGroup);

    initializationInterval(lines);

    resolutionLayout->addWidget(lines[0], 0, 0);
    resolutionLayout->addWidget(new QLabel("h", this), 0, 1);
    resolutionLayout->addWidget(lines[1], 0, 2);
    resolutionLayout->addWidget(new QLabel("m", this), 0, 3);
    resolutionLayout->addWidget(lines[2], 0, 4);
    resolutionLayout->addWidget(new QLabel("s", this), 0, 5);
    resolutionLayout->addWidget(lines[3], 0, 6);
    resolutionLayout->addWidget(new QLabel("ms", this), 0, 7);

    toolsContainer->addWidget(resolutionGroup, 0, 0);



}

void Functional::initializationInterval(QVector<QPointer<QLineEdit>>& lines)
{
    lines.resize(4);
    for (int i = 0; i < lines.size();++i)
    {
        lines[i] = new QLineEdit(this);

    }
}

Functional::~Functional()
{
}


