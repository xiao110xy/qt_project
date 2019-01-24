#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setup_connections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
	int key_num = event->key();
	if (key_num == Qt::Key_A || key_num == Qt::LeftArrow) {
		int n = ui->tableWidget->rowCount();
		int temp_n = order - 1;
		if (temp_n < 0)
			temp_n = n - 1;
		order = temp_n;
		int	temp_value = ui->tableWidget->item(temp_n, 1)->text().toInt();
		if (temp_value < 0) {
			QPointF temp_point;
			temp_point.setY(water_line);
			emit(ui->graphicsView->point_changed(temp_point));
		}
		else {
			emit(ui->tableWidget->cellClicked(temp_n, 0));
		}

	}
	if (key_num == Qt::Key_S) {
		save_model();
	}
	if (key_num == Qt::Key_D || key_num == Qt::RightArrow) {
		int n = ui->tableWidget->rowCount();
		int temp_n = order + 1;
		if (temp_n >= n)
			temp_n = 0;
		order = temp_n;
		int	temp_value = ui->tableWidget->item(temp_n, 1)->text().toInt();
		if (temp_value < 0) {
			QPointF temp_point;
			temp_point.setY(water_line);
			emit(ui->graphicsView->point_changed(temp_point));
		}
		else {
			emit(ui->tableWidget->cellClicked(temp_n, 0));
		}
	}

	if (key_num == Qt::Key_O) {
		open_folder();
	}
	if (key_num == Qt::Key_L) {
		load_model();
	}
}


void MainWindow::setup_connections()
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
	m_graphicsScene = new QGraphicsScene();
	m_graphicsScene->setItemIndexMethod(QGraphicsScene::NoIndex);
	m_graphicsScene->setBackgroundBrush(QPixmap::fromImage(bground));
	ui->graphicsView->setScene(m_graphicsScene);

	ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui->tableWidget->horizontalHeader()->setResizeContentsPrecision(QHeaderView::Stretch);
	ui->tableWidget->setColumnCount(2);

	connect(ui->open_folder, SIGNAL(clicked()), this, SLOT(open_folder()));
	connect(ui->save_model, SIGNAL(clicked()), this, SLOT(save_model()));
	connect(ui->load_model, SIGNAL(clicked()), this, SLOT(load_model()));

	connect(ui->tableWidget, SIGNAL(cellClicked(int ,int)), this, SLOT(load_image(int ,int )));

	connect(ui->graphicsView, SIGNAL(point_changed(QPointF)), this, SLOT(set_value(QPointF)));




}
QStringList MainWindow::file_filter(QStringList fileList)
{
	int iterations = fileList.length();
	QStringList tempList(fileList);
	QString str;
	fileList.clear();
	// Sorting through the image list to find those with compatible image formats
	for (int i = 0; i < iterations; i++)
	{
		if (tempList.value(i).endsWith(".tif", Qt::CaseInsensitive) ||
			tempList.value(i).endsWith(".png", Qt::CaseInsensitive) ||
			tempList.value(i).endsWith(".bmp", Qt::CaseInsensitive) ||
			tempList.value(i).endsWith(".jpg", Qt::CaseInsensitive) ||
			tempList.value(i).endsWith(".jpeg", Qt::CaseInsensitive) ||
			tempList.value(i).endsWith(".gif", Qt::CaseInsensitive) ||
			tempList.value(i).endsWith(".pbm", Qt::CaseInsensitive) ||
			tempList.value(i).endsWith(".pgm", Qt::CaseInsensitive) ||
			tempList.value(i).endsWith(".ppm", Qt::CaseInsensitive) ||
			tempList.value(i).endsWith(".xbm", Qt::CaseInsensitive) ||
			tempList.value(i).endsWith(".xpm", Qt::CaseInsensitive))
		{
			fileList.append(tempList.value(i));
		}
	}
	fileList.sort();
	return fileList;
}
void MainWindow::open_folder()
{
	if (!m_graphicsScene->sceneRect().isEmpty())
	{
		m_graphicsScene->clear();
		ui->graphicsView->green_circleGroup = nullptr;
	}

	ui->tableWidget->clearContents();
	file_path = QFileDialog::getExistingDirectory(this, "...", "./");
	//file_path = QString::fromLocal8Bit(file_path.toLocal8Bit().data());
    //file_path = QString::fromLocal8Bit("E:/water_line/result_sample");
	if (!file_path.isEmpty())
	{
		int counter = ui->tableWidget->rowCount();
		for (int index = 0; index < counter; index++)
		{
			QTableWidgetItem * item = ui->tableWidget->verticalHeaderItem(0);
			delete item;
		}

		QDir qDir = QDir(file_path);
		QStringList fileList = qDir.entryList();
		fileList = file_filter(fileList);
		ui->tableWidget->setRowCount(fileList.size());
		QTableWidgetItem  *item;
		for (int i = 0; i < fileList.size(); ++i) {
			item = new QTableWidgetItem(fileList[i]);
			item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			item->setTextAlignment(Qt::AlignCenter);
			ui->tableWidget->setItem(i,0, item);
			item = new QTableWidgetItem(QString::number(-1));
			item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			item->setTextAlignment(Qt::AlignCenter);
			ui->tableWidget->setItem(i,1, item);
		}
	}
	//int n = ui->listWidget->count();
	//for (int i = 0; i < n; ++i) {
	//	ui->listWidget->item(i)->setSelected(true);
	//}
}
void MainWindow::save_model()
{
	QString qStrFilePath = QFileDialog::getSaveFileName(this,
		tr("model txt"),
		QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
		tr("Files (*.txt)"));
	//QString qStrFilePath = "E:/water_line/result_sample/cache.txt";
	QFile f(qStrFilePath);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return;
	}
	QTextStream Out(&f);
	int row = ui->tableWidget->rowCount();
	if (row > 0) {
		for (int i = 0; i < row; ++i) {
			Out << ui->tableWidget->item(i, 0)->text() << ",";
			Out << ui->tableWidget->item(i, 1)->text() << ";" << endl;
		}
	}
	f.close();
}

void MainWindow::load_model()
{
	if (!m_graphicsScene->sceneRect().isEmpty())
	{
		m_graphicsScene->clear();
		ui->graphicsView->green_circleGroup = nullptr;
	}
	QString qStrFilePath = QFileDialog::getOpenFileName(this,
		tr("model txt"),
		QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
		tr("Files (*.txt)"));
	//QString qStrFilePath = "E:/water_line/result_sample/cache.txt";
	QFile f(qStrFilePath);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return;
	}
	int temp_value = -1;
	for (int i = qStrFilePath.size()-1; i >=0; --i) {
		if (qStrFilePath.at(i) == "/"|| qStrFilePath.at(i) == "\\") {
			temp_value = i;
			break;
		}
	}
	if (temp_value < 0)
		return;
	file_path = qStrFilePath.left(temp_value);

	ui->tableWidget->clearContents();//只清除表中数据，不清除表头内容
	
	//
	QTextStream txtInput(&f);
	vector<vector<QString>> str_array;
	while (1) {
		QString LineStr = txtInput.readLine();
		if (LineStr.isEmpty())
			break;
		vector<QString> temp;
		temp.clear();
		int temp_value = -1;
		for (int i = 0; i < LineStr.size(); ++i) {
			if (LineStr.at(i) == ",") {
				temp_value = i;
				break;
			}
		}
		if (temp_value < 1)
			return;
		temp.push_back(LineStr.left(temp_value));
		temp.push_back(LineStr.mid(temp_value+1,LineStr.size()-temp_value-2));
		str_array.push_back(temp);
	}
	ui->tableWidget->setRowCount(str_array.size());
	QTableWidgetItem  *item;
	for (int i = 0; i < str_array.size(); ++i) {
		vector<QString> temp = str_array[i];
		item = new QTableWidgetItem(temp[0]);
		//item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		item->setTextAlignment(Qt::AlignCenter);
		ui->tableWidget->setItem(i, 0, item);
		item = new QTableWidgetItem(temp[1]);
		//item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		item->setTextAlignment(Qt::AlignCenter);
		ui->tableWidget->setItem(i, 1, item);
	}
}

void MainWindow::load_image(int row, int col)
{
	//
	QString temp = this->file_path + "/" + ui->tableWidget->item(row, 0)->text();
	QImageReader reader(temp);
	if (!reader.canRead())
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Cannot read file"));
		msgBox.exec();
		return;
	}
	if (!m_graphicsScene->sceneRect().isEmpty())
	{
		m_graphicsScene->clear();
		ui->graphicsView->green_circleGroup = nullptr;
	}
	QImage qimage = reader.read();
	m_graphicsScene->setSceneRect(qimage.rect());
	m_graphicsScene->addPixmap(QPixmap::fromImage(qimage));
	//画线
	order = row;
	int	n = ui->tableWidget->item(row, 1)->text().toInt();
	if (n < 0)
		return;
	QPointF temp_point;
	temp_point.setY(n);
	ui->graphicsView->DrawLine(temp_point);

	// 将table widger视角改变

	QScrollBar *scrollBar = ui->tableWidget->verticalScrollBar();
	float y1 = scrollBar->minimum();
	float y2 = scrollBar->maximum();
	int y_value = ((order+1)*1.0 / ui->tableWidget->rowCount())*(y2 - y1) + y1;
	scrollBar->setValue(y_value);

}

void MainWindow::set_value(QPointF data)
{
	if (order < 0)
		return;
	int temp = data.y();
	water_line = temp;
	QTableWidgetItem  *item = new QTableWidgetItem(QString::number(temp));
	//item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	item->setTextAlignment(Qt::AlignCenter);
	ui->tableWidget->setItem(order, 1, item);
	emit(ui->tableWidget->cellClicked(order, 1));
}
