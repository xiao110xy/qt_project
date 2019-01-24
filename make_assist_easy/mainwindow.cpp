#include "mainwindow.h"
#include "ui_mainwindow.h"
bool get_number(string line_string, vector<double>& temp)
{
	temp.clear();
	for (int i = 0; i < line_string.size(); ++i) {
		if (line_string[i] >= 48 && line_string[i] <= 57) {
			double temp_value = 0;
			int j = i;
			int n = -1;
			for (; j < line_string.size(); ++j) {
				if (line_string[j] == 44 || line_string[j] == 59) {
					temp_value = n == -1 ? temp_value : temp_value / pow(10, j - n - 1);
					temp.push_back(temp_value);
					break;
				}
				if (line_string[j] >= 48 && line_string[j] <= 57) {
					temp_value = temp_value * 10 + line_string[j] - 48;
				}
				if (line_string[j] == 46)
					n = j;
			}
			i = j;
		}
	}
	return true;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	////
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
	ui->graphicsView_1->setScene(m_graphicsScene1);
	//
	m_graphicsScene2 = new QGraphicsScene();
	m_graphicsScene2->setItemIndexMethod(QGraphicsScene::NoIndex);
	m_graphicsScene2->setBackgroundBrush(QPixmap::fromImage(bground));
	ui->graphicsView_2->setScene(m_graphicsScene2);
	////
	connect(ui->load_image_1, SIGNAL(clicked()), this, SLOT(load_image_1()));
	connect(ui->load_image_2, SIGNAL(clicked()), this, SLOT(load_image_2()));
	connect(ui->load_assist_1, SIGNAL(clicked()), this, SLOT(load_assist_1()));
	connect(ui->load_assist_2, SIGNAL(clicked()), this, SLOT(load_assist_2()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load_image_1()
{
	//QString qStrFilePath = QFileDialog::getOpenFileName(this,
	//	tr("Open Image"),
	//	QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
	//	tr("Image Files (*.png *.jpg *.bmp)"));

	//if (qStrFilePath.isEmpty())
	//	return;
	QString qStrFilePath = "E:/water_line/assist/assist_1_1.bmp";
	string temp = string(qStrFilePath.toLocal8Bit());
	Mat im = imread(temp);
	cvtColor(im, im, CV_BGR2RGB);
	// 转为qimage
	QImage temp_image((const uchar*)im.data, im.cols, im.rows, QImage::Format_RGB888);
	if (!m_graphicsScene1->sceneRect().isEmpty())
	{
		m_graphicsScene1->clear();
	}
	m_graphicsScene1->setSceneRect(temp_image.rect());
	m_graphicsScene1->addPixmap(QPixmap::fromImage(temp_image));
	ui->graphicsView_1->viewFit();
	return;
}
void MainWindow::load_image_2()
{
	//QString qStrFilePath = QFileDialog::getOpenFileName(this,
//	tr("Open Image"),
//	QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
//	tr("Image Files (*.png *.jpg *.bmp)"));

//if (qStrFilePath.isEmpty())
//	return;
	QString qStrFilePath = "E:/water_line/assist/assist_1_2.bmp";
	string temp = string(qStrFilePath.toLocal8Bit());
	Mat im = imread(temp);
	cvtColor(im, im, CV_BGR2RGB);
	// 转为qimage
	QImage temp_image((const uchar*)im.data, im.cols, im.rows, QImage::Format_RGB888);
	if (!m_graphicsScene2->sceneRect().isEmpty())
	{
		m_graphicsScene2->clear();
	}
	m_graphicsScene2->setSceneRect(temp_image.rect());
	m_graphicsScene2->addPixmap(QPixmap::fromImage(temp_image));
	ui->graphicsView_2->viewFit();
	return;
}

void MainWindow::load_assist_1()
{
	//QString qStrFilePath = QFileDialog::getOpenFileName(this,
	//	tr("Open txt"),
	//	QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
	//	tr("Image Files (*.txt)"));

	//if (qStrFilePath.isEmpty())
	//	return;
	QString qStrFilePath = QString::fromLocal8Bit("E:/water_line/assist/assist_1 - 副本.txt");
	string temp = string(qStrFilePath.toLocal8Bit());
	fstream assist_file_name(temp);
	if (!assist_file_name)
		return;
	//
	string temp_name;
	vector<double> temp_value;
	assist_information temp_assist;
	while (!assist_file_name.eof())
	{
		//
		getline(assist_file_name, temp_name);
		get_number(temp_name, temp_value);

		// roi
		vector<double> roi;
		getline(assist_file_name, temp_name);
		get_number(temp_name, roi);
		if (roi.size() != 4)
			break;
		temp_assist.roi = roi;
		//
		vector<double> sub_roi;
		getline(assist_file_name, temp_name);
		get_number(temp_name, sub_roi);
		if (sub_roi.size() != 4)
			break;
		temp_assist.sub_roi = sub_roi;
		// 
		vector<vector<double>> temp_point;
		for (int n = 0; n < temp[temp.size() - 1]; ++n) {
			getline(assist_file_name, temp_name);
			vector<double> temp;
			get_number(temp_name, temp);
			temp_point.push_back(temp);
		}
		temp_assist.point = temp_point;
	}
	assist = temp_assist;
	return;
}

void MainWindow::load_assist_2()
{
	//QString qStrFilePath = QFileDialog::getOpenFileName(this,
//	tr("Open txt"),
//	QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
//	tr("Image Files (*.txt)"));

//if (qStrFilePath.isEmpty())
//	return;
	QString qStrFilePath = QString::fromLocal8Bit("E:/water_line/assist/assist_1 - 副本.txt");
	string temp = string(qStrFilePath.toLocal8Bit());
	fstream assist_file_name(temp);
	if (!assist_file_name)
		return;
	//
	string temp_name;
	vector<double> temp_value;
	assist_information temp_assist;
	while (!assist_file_name.eof())
	{
		//
		getline(assist_file_name, temp_name);
		get_number(temp_name, temp_value);

		// roi
		vector<double> roi;
		getline(assist_file_name, temp_name);
		get_number(temp_name, roi);
		if (roi.size() != 4)
			break;
		temp_assist.roi = roi;
		//
		vector<double> sub_roi;
		getline(assist_file_name, temp_name);
		get_number(temp_name, sub_roi);
		if (sub_roi.size() != 4)
			break;
		temp_assist.sub_roi = sub_roi;
		// 
		vector<vector<double>> temp_point;
		for (int n = 0; n < temp[temp.size() - 1]; ++n) {
			getline(assist_file_name, temp_name);
			vector<double> temp;
			get_number(temp_name, temp);
			temp_point.push_back(temp);
		}
		temp_assist.point = temp_point;
	}
	xy.x = temp_assist.roi[0] - assist.roi[0];
	xy.y = temp_assist.roi[1] - assist.roi[1];
	
	return;
}

void MainWindow::save_assist()
{
	int i = 0;
	++i;
	for (int i = 0; i < 100; ++i) {
		Mat im = im1;
		for (int j = 0; j < 100; ++j)
			continue;
	}
}
