﻿#include "MyGraphicsView.h"

MyGraphicsView::MyGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{
	setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	setRenderHint(QPainter::HighQualityAntialiasing);
	setTransformationAnchor(AnchorUnderMouse);

	base_point.setX(-9999);
	base_point.setY(-9999);

	green_circleGroup = nullptr;
	roi_Group1 = nullptr;
	roi_Group2 = nullptr;

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
	if (event->button() == Qt::LeftButton) {
		float distance1, distance2, distance3, distance4;
		QPointF temp_point = mapToScene(event->pos());
		distance1 = pow(roi_left_top1.x() - temp_point.x(), 2) + 
					pow(roi_left_top1.y() - temp_point.y(), 2);
		distance2 = pow(roi_right_bottom1.x() - temp_point.x(), 2) + 
					pow(roi_right_bottom1.y() - temp_point.y(), 2);
		distance3 = pow(roi_left_top2.x() - temp_point.x(), 2) + 
					pow(roi_left_top2.y() - temp_point.y(), 2);
		distance4 = pow(roi_right_bottom2.x() - temp_point.x(), 2) + 
					pow(roi_right_bottom2.y() - temp_point.y(), 2);
		if ((distance1 < distance2) && ((distance1 < distance3) && ((distance1 < distance4)))) {
			m_dragged1 = true;
			m_dragged2 = m_dragged3 = m_dragged4 = false;
		}
		if ((distance2 < distance1) && ((distance2 < distance3) && ((distance2 < distance4)))) {
			m_dragged2 = true;
			m_dragged1 = m_dragged3 = m_dragged4 = false;
		}
		if ((distance3 < distance1) && ((distance3 < distance2) && ((distance3 < distance4)))) {
			m_dragged3 = true;
			m_dragged1 = m_dragged2 = m_dragged4 = false;
		}
		if ((distance4 < distance1) && ((distance4 < distance2) && ((distance4 < distance3)))) {
			m_dragged4 = true;
			m_dragged1 = m_dragged2 = m_dragged3 = false;
		}
	}
    if (event->button() ==  Qt::RightButton){
        int width = this->sceneRect().width();
        int height = this->sceneRect().height();
        if (width<=0||height<=0)
            return;
        QPointF temp_point = mapToScene(event->pos());
        if (temp_point.x()<0||temp_point.y()<0)
            return;
        if (temp_point.x()>width||temp_point.y()>height)
            return;
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
		base_point = temp_point;
		// 放到组中
		QGraphicsItemGroup *circleGroup = new QGraphicsItemGroup();
		circleGroup->addToGroup(line1);
		circleGroup->addToGroup(line2);
		if (green_circleGroup == nullptr) {
			this->scene()->addItem(circleGroup);
			green_circleGroup = circleGroup;
		}
		else {
			this->scene()->removeItem(green_circleGroup);
			this->scene()->addItem(circleGroup);
			green_circleGroup = circleGroup;
		}
		emit(point_changed(base_point));
    }
}
void MyGraphicsView::mouseMoveEvent(QMouseEvent * event)
{
	if (m_dragged1 == true) {
		QPointF temp_point = mapToScene(event->pos());
		if (roi_Group1 == nullptr)
			return;
		if (temp_point.x() >= roi_right_bottom1.x() ||
			temp_point.y() >= roi_right_bottom1.y())
			return;
		QPoint temp_left_top = temp_point.toPoint();
		QRect temp = roi_Group1->rect().toRect();
		temp.setTopLeft(temp_left_top);
		temp.setBottomRight(roi_right_bottom1);
		DrawRoi(temp);
	}
	if (m_dragged2 == true) {
		QPointF temp_point = mapToScene(event->pos());
		if (temp_point.x() <= roi_left_top1.x() ||
			temp_point.y() <= roi_left_top1.y())
			return;
		if (roi_Group1 == nullptr)
			return;
		QPoint temp_right_bottom = temp_point.toPoint();
		QRect temp = roi_Group1->rect().toRect();
		temp.setTopLeft(roi_left_top1);
		temp.setBottomRight(temp_right_bottom);
		DrawRoi(temp);
	}
	if (m_dragged3 == true) {
		QPointF temp_point = mapToScene(event->pos());
		if (roi_Group2 == nullptr)
			return;
		if (temp_point.x() >= roi_right_bottom2.x() ||
			temp_point.y() >= roi_right_bottom2.y())
			return;
		QPoint temp_left_top = temp_point.toPoint();
		QRect temp = roi_Group2->rect().toRect();
		temp.setTopLeft(temp_left_top);
		temp.setBottomRight(roi_right_bottom2);
		DrawSubRoi(temp);
	}
	if (m_dragged4 == true) {
		QPointF temp_point = mapToScene(event->pos());
		if (temp_point.x() <= roi_left_top2.x() ||
			temp_point.y() <= roi_left_top2.y())
			return;
		if (roi_Group2 == nullptr)
			return;
		QPoint temp_right_bottom = temp_point.toPoint();
		QRect temp = roi_Group2->rect().toRect();
		temp.setTopLeft(roi_left_top2);
		temp.setBottomRight(temp_right_bottom);
		DrawSubRoi(temp);
	}
}
void MyGraphicsView::mouseReleaseEvent(QMouseEvent * event)
{
	m_dragged1 = false;
	m_dragged2 = false;
	m_dragged3 = false;
	m_dragged4 = false;
}
void MyGraphicsView::refresh_view()
{
	base_point.setX(-9999);
	base_point.setY(-9999);
	emit(point_changed(base_point));
	if (green_circleGroup != nullptr) {
		this->scene()->removeItem(green_circleGroup);
		green_circleGroup = nullptr;
	}
	if (roi_Group1 != nullptr) {
		this->scene()->removeItem(roi_Group1);
		roi_Group1 = nullptr;
	}
	if (roi_Group2 != nullptr) {
		this->scene()->removeItem(roi_Group2);
		roi_Group2 = nullptr;
	}
	for (int i = 0; i < point_list.size(); ++i) {
		this->scene()->removeItem(point_list[i]);
		this->scene()->removeItem(text_list[i]);
	}
	point_list.clear();
	text_list.clear();
}
void MyGraphicsView::addDrawPoint() {
	addDrawPoint(base_point);
}
void MyGraphicsView::addDrawPoint(QPointF temp_point)
{
	// 创建一支笔
	QPen xy_pen = QPen();
	xy_pen.setBrush(Qt::blue);
	xy_pen.setWidth(1);
	int lineRadius = 3;
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
	this->scene()->addItem(circleGroup);
	point_list.push_back(circleGroup);
	// 文本组
	QGraphicsTextItem *text = new QGraphicsTextItem();
	text->setPlainText(QString::number(point_list.size(), 10));
	text->setFont(QFont("Arial", 6));
	text->setDefaultTextColor(QColor(255, 0, 0));
	QPointF offset_point;
	text->boundingRect().width();
	offset_point.setX(text->boundingRect().width() / 2 + 3);
	offset_point.setY(text->boundingRect().height() / 2 + 3);
	text->setPos(temp_point - offset_point);
	this->scene()->addItem(text);
	text_list.push_back(text);
}
void MyGraphicsView::removeDrawPoint(int row)
{
	if (row < 0)
		return;

	QGraphicsItemGroup *temp1 = point_list[row];
	this->scene()->removeItem(temp1);
	point_list.erase(point_list.begin() + row);
	for (int i = row+1; i < text_list.size(); ++i) {
		QGraphicsTextItem *temp = text_list[i];
		temp->setPlainText(QString::number(i, 10));
	}
	QGraphicsTextItem *temp2 = text_list[row];
	this->scene()->removeItem(temp2);
	text_list.erase(text_list.begin()+row);
	
}

void MyGraphicsView::DrawRoi(QRect roi_rect)
{
	if (sceneRect().isEmpty())
		return;

	if (roi_Group1 != nullptr) {
		this->scene()->removeItem(roi_Group1);
	}
	roi_left_top1 = roi_rect.topLeft();
	roi_right_bottom1 = roi_rect.bottomRight();
	if (roi_left_top1.x() <= 0 || roi_left_top1.y() <= 0 ||
		roi_right_bottom1.x() >= this->scene()->width() ||
		roi_right_bottom1.y() >= this->scene()->height()) {
		DrawRoi();
		return;
	}
	roi_Group1 = new QGraphicsRectItem();
	roi_Group1->setFlags(QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable);
	roi_Group1->setRect(roi_rect);
	roi_Group1->setPen(QPen(Qt::red, 3, Qt::DashDotLine));
	this->scene()->addItem(roi_Group1);
}

void MyGraphicsView::DrawRoi()
{
	if (sceneRect().isEmpty())
		return;
	roi_left_top1.setX(0 + sceneRect().width() / 4);
	roi_left_top1.setY(0 + sceneRect().height() / 4);
	roi_right_bottom1.setX(3 * sceneRect().width() / 4);
	roi_right_bottom1.setY(3 * sceneRect().height() /4);
	QRect roi_rect;
	roi_rect.setTopLeft(roi_left_top1);
	roi_rect.setBottomRight(roi_right_bottom1);
	DrawRoi(roi_rect);
}

void MyGraphicsView::DrawSubRoi(QRect roi_rect)
{
	if (sceneRect().isEmpty())
		return;

	if (roi_Group2 != nullptr) {
		this->scene()->removeItem(roi_Group2);
	}
	roi_left_top2 = roi_rect.topLeft();
	roi_right_bottom2 = roi_rect.bottomRight();
	if (roi_left_top2.x() <= 0 || roi_left_top2.y() <= 0 ||
		roi_right_bottom2.x() >= this->scene()->width() ||
		roi_right_bottom2.y() >= this->scene()->height()) {
		DrawSubRoi();
		return;
	}
	roi_Group2 = new QGraphicsRectItem();
	roi_Group2->setFlags(QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable);
	roi_Group2->setRect(roi_rect);
	roi_Group2->setPen(QPen(Qt::green, 3, Qt::DashDotLine));
	this->scene()->addItem(roi_Group2);
}

void MyGraphicsView::DrawSubRoi()
{
	if (sceneRect().isEmpty())
		return;
	roi_left_top2.setX(0 + sceneRect().width() / 3);
	roi_left_top2.setY(0 + sceneRect().height() / 3);
	roi_right_bottom2.setX(2 * sceneRect().width() / 3);
	roi_right_bottom2.setY(2 * sceneRect().height() / 3);
	QRect roi_rect;
	roi_rect.setTopLeft(roi_left_top2);
	roi_rect.setBottomRight(roi_right_bottom2);
	DrawSubRoi(roi_rect);
}