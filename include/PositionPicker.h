#pragma once

#include <QWidget>
#include <QPoint>

class PositionPicker : public QWidget
{
	Q_OBJECT
public:
	PositionPicker(QWidget* parent = nullptr);
	~PositionPicker() = default;
signals:
	void positionSelected(int x, int y);
protected:
	void mousePressEvent(QMouseEvent* event) override;
};
