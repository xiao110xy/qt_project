#include "MyGraphicsView.h"

MyGraphicsView::MyGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{
	setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	setRenderHint(QPainter::HighQualityAntialiasing);
	setTransformationAnchor(AnchorUnderMouse);


	green_circleGroup = nullptr;

	

}

MyGraphicsView::~MyGraphicsView()
{
}

void MyGraphicsView::zoomIn()
{
	scaleView(qreal(1.05));
}

void MyGraphicsView::zoomOut()
{
	scaleView(1 / qreal(1.05));
}

void MyGraphicsView::viewFit()
{
	fitInView(sceneRect(), Qt::KeepAspectRatio);
	isResized = true;

	if (sceneRect().width() > sceneRect().height())
		isLandscape = true;
	else
		isLandscape = false;
}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
	if (event->modifiers() == Qt::ControlModifier)
	{
		if (event->delta() > 0) zoomIn();
		else zoomOut();
	}
	else if (event->modifiers() == Qt::ShiftModifier)
	{
		QWheelEvent fakeEvent(event->pos(), event->delta(), event->buttons(), Qt::NoModifier, Qt::Horizontal);
		QGraphicsView::wheelEvent(&fakeEvent);
	}
	else if (event->modifiers() == Qt::NoModifier)
	{
		QGraphicsView::wheelEvent(event);
	}
}

void MyGraphicsView::scaleView(qreal scaleFactor)
{
    if(sceneRect().isEmpty())
        return;

	QRectF expectedRect = transform().scale(scaleFactor, scaleFactor).mapRect(sceneRect());
	qreal expRectLength;
	int viewportLength;
	int imgLength;

	if (isLandscape)
	{
		expRectLength = expectedRect.width();
		viewportLength = viewport()->rect().width();
		imgLength = sceneRect().width();
	}
	else
	{
		expRectLength = expectedRect.height();
		viewportLength = viewport()->rect().height();
		imgLength = sceneRect().height();
	}

    if (expRectLength < viewportLength / 2) // minimum zoom : half of viewport
	{
		if (!isResized || scaleFactor < 1)
			return;
	}
    else if (expRectLength > imgLength * 10) // maximum zoom : x10
	{
		if (!isResized || scaleFactor > 1)
			return;
	}
	else
	{
		isResized = false;
	}

	scale(scaleFactor, scaleFactor);
}

void MyGraphicsView::resizeEvent(QResizeEvent *event)
{
	isResized = true;
    QGraphicsView::resizeEvent(event);
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
        int width = this->sceneRect().width();
        int height = this->sceneRect().height();
        if (width<=0||height<=0)
            return;
        QPointF temp_point = mapToScene(event->pos());
        if (temp_point.x()<0||temp_point.y()<0)
            return;
        if (temp_point.x()>width||temp_point.y()>height)
            return;
		DrawLine(temp_point);
		emit(point_changed(temp_point));
}
void MyGraphicsView::mouseMoveEvent(QMouseEvent * event)
{

}
void MyGraphicsView::mouseReleaseEvent(QMouseEvent * event)
{

}




void MyGraphicsView::DrawLine(QPointF temp_point)
{
	if (temp_point.y() < 0)
		return;
	// 创建一支笔
	QPen xy_pen = QPen();
	xy_pen.setBrush(Qt::green);
	xy_pen.setWidth(1);
	int lineRadius = 2;
	//横线
	QGraphicsLineItem *line = new QGraphicsLineItem();
	line->setPen(xy_pen);
	line->setLine(0, temp_point.y(),this->scene()->width(), temp_point.y());

	// 放到组中
	QGraphicsItemGroup *circleGroup = new QGraphicsItemGroup();
	circleGroup->addToGroup(line);
	if (green_circleGroup == nullptr) {
		this->scene()->addItem(circleGroup);
		green_circleGroup = circleGroup;
	}
	else {
		this->scene()->removeItem(green_circleGroup);
		this->scene()->addItem(circleGroup);
		green_circleGroup = circleGroup;
	}



}
