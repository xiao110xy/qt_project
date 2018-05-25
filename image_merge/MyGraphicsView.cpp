#include "MyGraphicsView.h"

MyGraphicsView::MyGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{
	setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	setRenderHint(QPainter::HighQualityAntialiasing);
	setTransformationAnchor(AnchorUnderMouse);

	isResized = false;
	isLandscape = false;

    base_point.setX(-99);
    base_point.setY(-99);
	top_point.setX(-99);
	top_point.setY(-99);
	bottom_point.setX(-99);
	bottom_point.setY(-99);

    top_flag = false;
    bottom_flag = false;
    point_flag= false;


	green_circleGroup = nullptr;
	top_line = nullptr;
	bottom_line = nullptr;
}

MyGraphicsView::~MyGraphicsView()
{
}

void MyGraphicsView::zoomIn()
{
	scaleView(qreal(1.2));
}

void MyGraphicsView::zoomOut()
{
	scaleView(1 / qreal(1.2));
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
    if (event->button() ==  Qt::LeftButton){
        int width = this->sceneRect().width();
        int height = this->sceneRect().height();
        if (width<=0||height<=0)
            return;
        QPointF temp_point = mapToScene(event->pos());
        if (temp_point.x()<0||temp_point.y()<0)
            return;
        if (temp_point.x()>width||temp_point.y()>height)
            return;
        // 创建一个新的红线
        if (top_flag==true){
			if (bottom_line!=nullptr&&temp_point.y() > bottom_point.y()) {
				top_point = bottom_point;
				bottom_point = temp_point;
				this->scene()->removeItem(bottom_line);
				QPen xy_pen = QPen();
				xy_pen.setBrush(Qt::blue);
				xy_pen.setWidth(1);
				QGraphicsLineItem *line = new QGraphicsLineItem();
				line->setPen(xy_pen);
				line->setLine(0, bottom_point.y(), width, bottom_point.y());
				bottom_line = line;
				this->scene()->addItem(bottom_line);
			}
			else {
				top_point = temp_point;
			}
			QPen xy_pen = QPen();
			xy_pen.setBrush(Qt::red);
			xy_pen.setWidth(1);
			QGraphicsLineItem *line = new QGraphicsLineItem();
			line->setPen(xy_pen);
			line->setLine(0, top_point.y(), width, top_point.y());
			if (top_line == nullptr) {
				this->scene()->addItem(line);
				top_line = line;
			}
			else {
				this->scene()->removeItem(top_line);
				this->scene()->addItem(line);
				top_line = line;
			}
        }
        // 创建一个新的蓝线
        if (bottom_flag == true){
			if (top_line != nullptr&&top_point.y() > temp_point.y()) {
				bottom_point = top_point;
				top_point = temp_point;
				this->scene()->removeItem(top_line);
				QPen xy_pen = QPen();
				xy_pen.setBrush(Qt::red);
				xy_pen.setWidth(1);
				QGraphicsLineItem *line = new QGraphicsLineItem();
				line->setPen(xy_pen);
				line->setLine(0, top_point.y(), width, top_point.y());
				top_line = line;
				this->scene()->addItem(top_line);
			}
			else {
				bottom_point = temp_point;
			}
			QPen xy_pen = QPen();
			xy_pen.setBrush(Qt::blue);
			xy_pen.setWidth(1);
			QGraphicsLineItem *line = new QGraphicsLineItem();
			line->setPen(xy_pen);
			line->setLine(0, bottom_point.y(), width, bottom_point.y());
			if (bottom_line == nullptr) {
				this->scene()->addItem(line);
				bottom_line = line;
			}
			else {
				this->scene()->removeItem(bottom_line);
				this->scene()->addItem(line);
				bottom_line = line;
			}
        }    
		if (top_line != nullptr&&bottom_line != nullptr) {
			
		}
		// 创建一个绿点
		if (point_flag == true ) {
			// 创建一支笔
			QPen xy_pen = QPen();
			xy_pen.setBrush(Qt::green);
			xy_pen.setWidth(1);
			int lineRadius = 2;
			//横线
			QGraphicsLineItem *line1 = new QGraphicsLineItem();
			line1->setPen(xy_pen);
			line1->setLine(temp_point.x() - lineRadius, temp_point.y(),
				temp_point.x() + lineRadius, temp_point.y());
			//竖线
			QGraphicsLineItem *line2 = new QGraphicsLineItem();
			line2->setPen(xy_pen);
			line2->setLine(temp_point.x(), temp_point.y() - lineRadius,
				temp_point.x(), temp_point.y() + lineRadius);
			// 放到组中
			QGraphicsItemGroup *circleGroup = new QGraphicsItemGroup();
			circleGroup->addToGroup(line1);
			circleGroup->addToGroup(line2);
			base_point = temp_point;
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
    }
}

