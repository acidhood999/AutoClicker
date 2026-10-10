#include "PositionPicker.h"
#include <QMouseEvent>
#include <QGuiApplication>
#include <QScreen>
#include <QCursor>

PositionPicker::PositionPicker(QWidget* parent) : QWidget(parent)
{
	setWindowFlags(Qt::WindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool));
	setCursor(Qt::CrossCursor);
	setWindowOpacity(0.3);
	setGeometry(QGuiApplication::primaryScreen()->virtualGeometry());
}

void PositionPicker::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		QPoint globalPos = QCursor::pos();
		positionSelected(globalPos.x(), globalPos.y());
		close();
	}
}
