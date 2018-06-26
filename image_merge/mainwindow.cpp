#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>

#include <QImageReader>
#include <QImageWriter>

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1050,780);
    QImage bground(50, 50, QImage::Format_RGB888);
    for (int y = 0; y < 25; y++)
    {
        for (int x = 0; x < 25; x++)
        {
            bground.setPixel(x, y, qRgb(0xCA, 0xCA, 0xCA));
            bground.setPixel(x + 25, y, qRgb(0xFF, 0xFF, 0xFF));
            bground.setPixel(x, y + 25, qRgb(0xFF, 0xFF, 0xFF));
            bground.setPixel(x + 25, y + 25, qRgb(0xCA, 0xCA, 0xCA));
        }
    }
    //
    m_graphicsScene1 = new QGraphicsScene();
    m_graphicsScene1->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_graphicsScene1->setBackgroundBrush(QPixmap::fromImage(bground));
    ui->m_graphicsView1->setScene(m_graphicsScene1);
    //
    m_graphicsScene2 = new QGraphicsScene();
    m_graphicsScene2->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_graphicsScene2->setBackgroundBrush(QPixmap::fromImage(bground));
    ui->m_graphicsView2->setScene(m_graphicsScene2);
	//
    m_graphicsScene3 = new QGraphicsScene();
    m_graphicsScene3->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_graphicsScene3->setBackgroundBrush(QPixmap::fromImage(bground));

    ui->statusBar->showMessage("ready", 5000);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPressLoadImage()
{
    QString qStrFilePath = QFileDialog::getOpenFileName(this,
        tr("Open Image"),
        QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
        tr("Image Files (*.png *.jpg *.bmp)"));

    if (qStrFilePath.isEmpty())
        return;

    QImageReader reader(qStrFilePath);
    if (!reader.canRead())
    {
        QMessageBox msgBox;
        msgBox.setText("Cannot read file");
        msgBox.exec();
        return;
    }
    if (!m_graphicsScene1->sceneRect().isEmpty())
    {
        m_graphicsScene1->clear();
    }
    QImage qimg = reader.read();
    m_graphicsScene1->setSceneRect(qimg.rect());
    m_graphicsScene1->addPixmap(QPixmap::fromImage(qimg));
    if (!m_graphicsScene2->sceneRect().isEmpty())
    {
        m_graphicsScene2->clear();
    }
    image = qimg;
	temp_image = qimg;
	roi_image = qimg;
    m_graphicsScene2->setSceneRect(qimg.rect());
    m_graphicsScene2->addPixmap(QPixmap::fromImage(qimg));
    ui->m_graphicsView1->viewFit();
    ui->m_graphicsView2->viewFit();
    ui->statusBar->showMessage("image loaded", 3000);
}

void MainWindow::onPressSaveImage()
{
    if (m_graphicsScene1->sceneRect().isEmpty())
        return;

    m_graphicsScene1->clearSelection();
    QImage img(m_graphicsScene1->sceneRect().size().toSize(), QImage::Format_RGB888);
    QPainter painter(&img);
    m_graphicsScene1->render(&painter);

    QString qStrFilePath = QFileDialog::getSaveFileName(this,
            tr("Save Image"),
            QStandardPaths::writableLocation(QStandardPaths::CacheLocation).replace("cache", "newfile.bmp"),
            tr("BMP file (*.bmp);;JPG file (*.jpg);;PNG file (*.png)"));

    if (qStrFilePath.isEmpty())
        return;

    QImageWriter writer(qStrFilePath);
    if(!writer.canWrite())
    {
        QMessageBox msgBox;
        msgBox.setText("Cannot write file");
        msgBox.exec();
        return;
    }
    writer.write(img);

    ui->statusBar->showMessage("image saved", 3000);
}

void MainWindow::onPressFitWindow()
{
    ui->m_graphicsView1->viewFit();
    ui->m_graphicsView2->viewFit();
}


void MainWindow::on_roi_top_clicked()
{
	ui->m_graphicsView2->setScene(m_graphicsScene2);
    ui->m_graphicsView2->top_flag = true;
    ui->m_graphicsView2->bottom_flag = false;
    ui->m_graphicsView2->point_flag = false;
}

void MainWindow::on_roi_bottom_clicked()
{
	ui->m_graphicsView2->setScene(m_graphicsScene2);
    ui->m_graphicsView2->top_flag = false;
    ui->m_graphicsView2->bottom_flag = true;
    ui->m_graphicsView2->point_flag = false;
}

void MainWindow::on_roi_point_clicked()
{
	ui->m_graphicsView2->setScene(m_graphicsScene2);
    ui->m_graphicsView2->top_flag = false;
    ui->m_graphicsView2->bottom_flag = false;
    ui->m_graphicsView2->point_flag = true;
}

void MainWindow::on_base_fix_point_clicked()
{
	ui->m_graphicsView2->setScene(m_graphicsScene2);
	ui->m_graphicsView1->top_flag = false;
	ui->m_graphicsView1->bottom_flag = false;
	ui->m_graphicsView1->point_flag = true;
}

void MainWindow::on_preview_clicked()
{
	QPointF top_point = ui->m_graphicsView2->top_point;
	QPointF bottom_point = ui->m_graphicsView2->bottom_point;
	QPointF base_point1 = ui->m_graphicsView1->base_point;
	QPointF base_point2 = ui->m_graphicsView2->base_point;
	if (top_point.x() < 0 || top_point.y() < 0 ||
		bottom_point.x() < 0 || bottom_point.y() < 0 ||
		base_point1.x() < 0 || base_point1.y() < 0 ||
		base_point2.x() < 0 || base_point2.y() < 0) {
		QMessageBox msgBox;
		msgBox.setText(tr("please select two line and two point,error"));
		msgBox.exec();
		return;
	}
	// roi image 裁剪
	QPoint temp_distance = (base_point1 - base_point2).toPoint();
    QImage data = image;
	int height = m_graphicsScene2->sceneRect().height();
	int width = m_graphicsScene2->sceneRect().width();
	for (int i = 0; i < width; ++i) 
		for (int j = round(top_point.y()); j < bottom_point.y(); ++j) {
			QPoint temp(i, j);
			temp.setY(temp.y() + temp_distance.y());
			if (temp.x() < 0 || temp.y() < 0 || temp.x() > width || temp.y() > height)
				continue;
			
			data.setPixel(temp, temp_image.pixel(QPoint(i, j)));
		}
	// 贴合显示到原图上
	temp_image = data;
	if (!m_graphicsScene3->sceneRect().isEmpty())
	{
		m_graphicsScene3->clear();
	}
	m_graphicsScene3->setSceneRect(temp_image.rect());
	m_graphicsScene3->addPixmap(QPixmap::fromImage(temp_image));
	ui->m_graphicsView2->setScene(m_graphicsScene3);
}

void MainWindow::on_imageprocess_clicked()
{
	ui->m_graphicsView2->setScene(m_graphicsScene2);

	QImage bground(50, 50, QImage::Format_RGB888);
	for (int y = 0; y < 25; y++)
	{
		for (int x = 0; x < 25; x++)
		{
			bground.setPixel(x, y, qRgb(0xCA, 0xCA, 0xCA));
			bground.setPixel(x + 25, y, qRgb(0xFF, 0xFF, 0xFF));
			bground.setPixel(x, y + 25, qRgb(0xFF, 0xFF, 0xFF));
			bground.setPixel(x + 25, y + 25, qRgb(0xCA, 0xCA, 0xCA));
		}
    }
    image = temp_image;
	m_graphicsScene1 = new QGraphicsScene();
	m_graphicsScene1->setItemIndexMethod(QGraphicsScene::NoIndex);
	m_graphicsScene1->setBackgroundBrush(QPixmap::fromImage(bground));
	m_graphicsScene1->setSceneRect(temp_image.rect());
    m_graphicsScene1->addPixmap(QPixmap::fromImage(image));
	ui->m_graphicsView1->setScene(m_graphicsScene1);
}
