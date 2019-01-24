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
public:
	void refresh_view();
signals:
	//! Signal is emitted when a point changed;
	void point_changed(QPointF point);
public:
	QPointF base_point;
	QGraphicsItemGroup *green_circleGroup;

	std::vector<QGraphicsItemGroup*> point_list;
	std::vector<QGraphicsTextItem*> text_list;
	QGraphicsRectItem *roi_Group1;
	QPoint roi_left_top1;
	bool m_dragged1 = false;
	QPoint roi_right_bottom1;
	bool m_dragged2 = false;
	QGraphicsRectItem *roi_Group2;
	QPoint roi_left_top2;
	bool m_dragged3 = false;
	QPoint roi_right_bottom2;
	bool m_dragged4 = false;

protected slots:
	void addDrawPoint();
	void addDrawPoint(QPointF temp_point);
	void removeDrawPoint(int row);
	void DrawRoi(QRect roi_rect);
	void DrawRoi();
	void DrawSubRoi(QRect roi_rect);
	void DrawSubRoi();
};
