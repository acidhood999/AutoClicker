#pragma once

#include <QWidget>
#include <QPushButton>
#include <QPointer>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QVector>

class Functional : public QWidget
{
	Q_OBJECT
public:
	Functional(QWidget* parent);
	~Functional();
private:
	QPointer<QGridLayout> toolsContainer;
	QPointer<QPushButton> b = new QPushButton("ewr");
	QVector<QPointer<QLineEdit>> lines;

	void initializationInterval(QVector<QPointer<QLineEdit>>& lines);
};