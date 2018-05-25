#pragma once

#include <QGraphicsView>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QPoint>
#include <QGraphicsLineItem>
class MyGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	MyGraphicsView(QWidget *parent = 0);
	~MyGraphicsView();
	void viewFit();
    void zoomIn();
    void zoomOut();

private:
    void scaleView(qreal scaleFactor);
    bool isResized;
	bool isLandscape;

protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
public:
	QPointF base_point;
	QPointF top_point;
	QPointF bottom_point;

    QGraphicsItemGroup *green_circleGroup;
    QGraphicsItem *top_line;
    QGraphicsItem *bottom_line;

    bool top_flag;
    bool bottom_flag;
    bool point_flag;
};
