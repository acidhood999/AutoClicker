#pragma once
#include <QWidget>
#include <QPoint>
#include <QObject>

class NodeMarker : public QWidget
{
	Q_OBJECT

public:
	NodeMarker(int number, int x, int y, QWidget* parent = nullptr);

	void setNumber(int number);
	int getNumber() const;

	~NodeMarker();
signals:
	void positionChanged(int number, int x, int y);

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	int numberN;
	QPoint dragPosition;

};
