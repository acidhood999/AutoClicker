#include "NodeMarker.h"
#include <QPainter>       
#include <QMouseEvent>

NodeMarker::NodeMarker(int number, int x, int y, QWidget* parent) : QWidget(parent), numberN(number)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);
	setFixedSize(36, 36);
	setCursor(Qt::SizeAllCursor);
	move(x - width() / 2, y - height() / 2);
}

void NodeMarker::setNumber(int number)
{ 
	this->numberN = number;
	update();
}

int NodeMarker::getNumber() const { return numberN; }

void NodeMarker::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.setBrush(QColor(255, 255, 255, 220));
	painter.setPen(QPen(Qt::black, 2));        
	painter.drawEllipse(2, 2, width() - 4, height() - 4);

	painter.setPen(Qt::black);
	QFont font = painter.font();
	font.setBold(true);
	font.setPointSize(11);
	painter.setFont(font);
	painter.drawText(rect(), Qt::AlignCenter, QString::number(numberN));
}

void NodeMarker::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void NodeMarker::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		QPoint newPosition = event->globalPosition().toPoint() - dragPosition;
		move(newPosition);

		int centerX = newPosition.x() + width() / 2;
		int centerY = newPosition.y() + height() / 2;

		positionChanged(numberN, centerX, centerY);
		event->accept();
	}
}

NodeMarker::~NodeMarker() {}

