#pragma once
#include <vector>
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
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
signals:
	//! Signal is emitted when a point changed;
	void point_changed(QPointF point);
public:
	QGraphicsItemGroup *green_circleGroup;



public slots:
	void DrawLine(QPointF temp_point);

};
