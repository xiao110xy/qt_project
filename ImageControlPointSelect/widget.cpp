#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
	QTranslator* translator = new QTranslator;
	QString runPath = QCoreApplication::applicationDirPath();       //获取文件运行路径
	if (translator->load(runPath + "/cn.qm")) {
		qApp->installTranslator(translator);
	}

    ui->setupUi(this);
	setupWindows();
	setupConnections();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setupWindows()
{
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
	ui->graphicsView1->setScene(m_graphicsScene1);
	//
	m_graphicsScene2 = new QGraphicsScene();
	m_graphicsScene2->setItemIndexMethod(QGraphicsScene::NoIndex);
	m_graphicsScene2->setBackgroundBrush(QPixmap::fromImage(bground));
	ui->graphicsView2->setScene(m_graphicsScene2);
	// table 
	ui->tableWidget->setColumnCount(4);
	ui->tableWidget->setRowCount(0);
	QStringList header1;
	header1 <<"Base X" << "Base Y" << "Wrap X" << "Wrap Y";
	ui->tableWidget->setHorizontalHeaderLabels(header1);
	ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
	// list table 
	ui->tableWidget_List->setColumnCount(6);
	ui->tableWidget_List->setRowCount(0);
	QStringList header2;
	header2 << "ruler number" << "assist number" << "template lenght" << "order" << "back" << "point number";
	ui->tableWidget_List->setHorizontalHeaderLabels(header2);
	//ui->tableWidget_List->setSelectionMode(QAbstractItemView::SingleSelection);
	//ui->tableWidget_List->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//ui->tableWidget_List->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
}

void Widget::setupConnections()
{
	connect(this, SIGNAL(draw_roi(QRect)), ui->graphicsView2, SLOT(DrawRoi(QRect)));
	connect(this, SIGNAL(draw_sub_roi(QRect)), ui->graphicsView2, SLOT(DrawSubRoi(QRect)));

	connect(ui->load_base_image, SIGNAL(clicked()), this, SLOT(pushbotton_load_base_image()));
	connect(ui->load_wrap_image, SIGNAL(clicked()), this, SLOT(pushbotton_load_wrap_image()));
    connect(ui->get_roi_1, SIGNAL(clicked()), this, SLOT(pushbotton_get_roi()));
    connect(ui->get_roi_2, SIGNAL(clicked()), this, SLOT(pushbotton_get_sub_roi()));
    connect(ui->savePts, SIGNAL(clicked()), this, SLOT(pushbotton_savePts()));
	connect(ui->reloadPts, SIGNAL(clicked()), this, SLOT(pushbotton_reloadPts()));

	connect(ui->graphicsView1, SIGNAL(point_changed(QPointF)), ui->base_point, SLOT(change_value(QPointF)));
	connect(ui->graphicsView2, SIGNAL(point_changed(QPointF)), ui->wrap_point, SLOT(change_value(QPointF)));
	
	connect(ui->graphicsView1, SIGNAL(point_changed(QPointF)), ui->tableWidget, SLOT(base_point_changed(QPointF)));
	connect(ui->graphicsView2, SIGNAL(point_changed(QPointF)), ui->tableWidget, SLOT(wrap_point_changed(QPointF)));
	
	connect(ui->tableWidget, SIGNAL(add_index()),ui->graphicsView1, SLOT(addDrawPoint()));
	connect(ui->tableWidget, SIGNAL(add_index()),ui->graphicsView2, SLOT(addDrawPoint()));
	connect(ui->tableWidget, SIGNAL(add_base_index(QPointF)),ui->graphicsView1, SLOT(addDrawPoint(QPointF)));
	connect(ui->tableWidget, SIGNAL(add_wrap_index(QPointF)),ui->graphicsView2, SLOT(addDrawPoint(QPointF)));
	connect(ui->tableWidget, SIGNAL(remove_index(int)),ui->graphicsView1, SLOT(removeDrawPoint(int)));
	connect(ui->tableWidget, SIGNAL(remove_index(int)),ui->graphicsView2, SLOT(removeDrawPoint(int)));

}

void Widget::pushbotton_load_base_image()
{
	QString qStrFilePath = QFileDialog::getOpenFileName(this,
		tr("Open Image"),
		QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
		tr("Image Files (*.png *.jpg *.bmp)"));

	if (qStrFilePath.isEmpty())
		return;
	// 刷新
	ui->graphicsView1->refresh_view();
	ui->graphicsView2->refresh_view();
    ui->tableWidget->clearContents();//只清除表中数据，不清除表头内容
    ui->tableWidget->setRowCount(0);
	//
	//QString qStrFilePath = "C:/Users/Alexia/Desktop/template_1.bmp";
	QImageReader reader(qStrFilePath);
	if (!reader.canRead())
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Cannot read file"));
		msgBox.exec();
		return;
	}
	if (!m_graphicsScene1->sceneRect().isEmpty())
	{
		m_graphicsScene1->clear();
	}
	left_qimage = reader.read();
	m_graphicsScene1->setSceneRect(left_qimage.rect());
	m_graphicsScene1->addPixmap(QPixmap::fromImage(left_qimage));
	ui->graphicsView1->viewFit();
}

vector<double> Widget::get_line_number(string temp_name)
{
	vector<double> temp;
	temp.clear();
	for (int i = 0; i < temp_name.size(); ++i) {
		if (temp_name[i] >= 48 && temp_name[i] <= 57) {
			double temp_value = 0;
			int j = i;
			int n = -1;
			for (; j < temp_name.size(); ++j) {
				if (temp_name[j] == 44 || temp_name[j] == 59) {
					temp_value = n == -1 ? temp_value : temp_value / pow(10, j - n - 1);
					temp.push_back(temp_value);
					break;
				}
				if (temp_name[j] >= 48 && temp_name[j] <= 57) {
					temp_value = temp_value * 10 + temp_name[j] - 48;
				}
				if (temp_name[j] == 46)
					n = j;
			}
			i = j;
		}
	}
	return temp;
}

void Widget::pushbotton_load_wrap_image()
{
	QString qStrFilePath = QFileDialog::getOpenFileName(this,
		tr("Open Image"),
		QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
		tr("Image Files (*.png *.jpg *.bmp)"));
	if (qStrFilePath.isEmpty())
		return;
	//QString qStrFilePath="C:/Users/xy/Pictures/bing/20180701.jpg";
	// 刷新
    ui->graphicsView1->refresh_view();
    ui->graphicsView2->refresh_view();
    ui->tableWidget->clearContents();//只清除表中数据，不清除表头内容
    ui->tableWidget->setRowCount(0);
	//
	//QString qStrFilePath = "C:/Users/Alexia/Desktop/1.jpg";
	QImageReader reader(qStrFilePath);
	if (!reader.canRead())
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Cannot read file"));
		msgBox.exec();
		return;
	}
	if (!m_graphicsScene2->sceneRect().isEmpty())
	{
		m_graphicsScene2->clear();
	}
	right_qimage = reader.read();
	m_graphicsScene2->setSceneRect(right_qimage.rect());
	m_graphicsScene2->addPixmap(QPixmap::fromImage(right_qimage));
	ui->graphicsView2->viewFit();
}

void Widget::pushbotton_get_roi()
{
	if (ui->graphicsView2->sceneRect().isEmpty())
		return;
	QPoint roi_left_top, roi_right_bottom;
	roi_left_top.setX(0 + ui->graphicsView2->sceneRect().width() / 4);
	roi_left_top.setY(0 + ui->graphicsView2->sceneRect().height() / 4);
	roi_right_bottom.setX(3 * ui->graphicsView2->sceneRect().width() / 4);
	roi_right_bottom.setY(3 * ui->graphicsView2->sceneRect().height() / 4);
	QRect roi_rect;
	roi_rect.setTopLeft(roi_left_top);
	roi_rect.setBottomRight(roi_right_bottom);
	emit(draw_roi(roi_rect));
}

void Widget::pushbotton_get_sub_roi()
{
	QPoint roi_left_top, roi_right_bottom;
	roi_left_top.setX(0 + ui->graphicsView2->sceneRect().width() / 3);
	roi_left_top.setY(0 + ui->graphicsView2->sceneRect().height() / 3);
	roi_right_bottom.setX(2 * ui->graphicsView2->sceneRect().width() / 3);
	roi_right_bottom.setY(2 * ui->graphicsView2->sceneRect().height() / 3);
	QRect roi_rect;
	roi_rect.setTopLeft(roi_left_top);
	roi_rect.setBottomRight(roi_right_bottom);
	emit(draw_sub_roi(roi_rect));
}

void Widget::pushbotton_savePts()
{
	QString qStrFilePath = QFileDialog::getSaveFileName(this,
		tr("save assit txt"),
		QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
		tr("Assit  Files (*.txt)"));
	//QString qStrFilePath = "C:/Users/Alexia/Desktop/assist_3.txt";
	QFile f(qStrFilePath);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
		cout << "Open failed." << endl;
		return;
	}

	QTextStream Out(&f);
	int row = ui->tableWidget->rowCount();
	if (row > 0) {
        Out << "0,0,0,0,0," << QString::number(row, 10) <<";"<<endl;
		QRect temp1 = ui->graphicsView2->roi_Group1->rect().toRect();
		Out << temp1.x()<<","<<temp1.y() << "," <<
			temp1.width() << "," <<temp1.height()<<";" << endl;
		QRect temp2 = ui->graphicsView2->roi_Group2->rect().toRect();
		Out << temp2.x() << "," << temp2.y() << "," <<
			temp2.width() << "," << temp2.height() << ";" << endl;
		for (int i = 0; i < row; ++i) {
			Out << ui->tableWidget->item(i, 0)->text() << ",";
			Out << ui->tableWidget->item(i, 1)->text() << ","; 
			Out << ui->tableWidget->item(i, 2)->text() << ","; 
			Out << ui->tableWidget->item(i, 3)->text() << ";"<< endl;
		}
		QMessageBox::information(this, tr("notice"),tr("file has writed"));
	}
	f.close();
}

void Widget::pushbotton_reloadPts()
{
	//QString qStrFilePath = QFileDialog::getOpenFileName(this,
	//	tr("Open assit txt"),
	//	QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
	//	tr("Assit  Files (*.txt)"));
	QString qStrFilePath = "E:/water_line/assist/assist_1.txt";
	QFile f(qStrFilePath);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text)){
		cout << "Open failed." << endl;
		return;
	}
	// 刷新
    ui->graphicsView1->refresh_view();
    ui->graphicsView2->refresh_view();
    ui->tableWidget->clearContents();//只清除表中数据，不清除表头内容
    ui->tableWidget->setRowCount(0);
	ui->tableWidget_List->clearContents();//只清除表中数据，不清除表头内容
    ui->tableWidget_List->setRowCount(0);
	//
	QTextStream txtInput(&f);
	QString LineStr= txtInput.readLine();
	vector<double> temp;
	string temp_name;
	temp_name = LineStr.toLocal8Bit();
	temp = get_line_number(temp_name);
	water_number = temp[0];
	while (1) {
		temp_name = txtInput.readLine().toLocal8Bit();
		temp = get_line_number(temp_name);
		if (temp.size() < 5)
			break;
		assist_information temp_assist;
		for (int i = 0; i < temp.size(); ++i) {
			temp_assist.first_line.push_back(temp[i]);
		}
		int nLine = temp[temp.size() - 1];
		ui->tableWidget->clearContents();//只清除表中数据，不清除表头内容
		ui->tableWidget->setRowCount(nLine);
		temp_name = txtInput.readLine().toLocal8Bit();
		temp = get_line_number(temp_name);
		for (int i = 0; i < temp.size(); ++i) {
			temp_assist.roi[i]=temp[i];
		}
		temp_name = txtInput.readLine().toLocal8Bit();
		temp = get_line_number(temp_name);
		for (int i = 0; i < temp.size(); ++i) {
			temp_assist.sub_roi[i] = temp[i];
		}
		for (int n = 0; n < nLine; ++n) {
			temp_name = txtInput.readLine().toLocal8Bit();
			temp = get_line_number(temp_name);
			temp_assist.point.push_back(temp);
		}
		assist.push_back(temp_assist);
	}
	f.close();
	ui->tableWidget_List->setRowCount(assist.size());
	for (int i = 0; i < assist.size(); ++i) {
		for (int j = 0; j < assist[i].first_line.size(); ++j) {
			ui->tableWidget_List->setItem(i, j, new QTableWidgetItem(QString::number(assist[i].first_line[j])));
		}
	}
	
}
