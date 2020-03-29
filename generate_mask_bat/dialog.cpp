#include "dialog.h"
#include "ui_dialog.h"

QString get_string(QString temp, QChar split) {
	QString result;
	for (int i = temp.size() - 1; i >= 0; --i) {
		if (temp[i] == split) {
			result = temp.left(i);
			break;
		}
	}
	return result;
}
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
	ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
	setup_connections();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setup_connections()
{
    connect(ui->open_image,SIGNAL(clicked()), this, SLOT(push_open_image()));
	connect(ui->assist_water,SIGNAL(clicked()), this, SLOT(push_assist_water()));
	//connect(ui->assist_txt,SIGNAL(clicked()), this, SLOT(push_assist_txt()));
	//connect(ui->assist_image,SIGNAL(clicked()), this, SLOT(push_assist_image()));
	//connect(ui->ref,SIGNAL(clicked()), this, SLOT(push_ref()));
	//connect(ui->result_txt,SIGNAL(clicked()), this, SLOT(push_result_txt()));
	connect(ui->save_bat,SIGNAL(clicked()), this, SLOT(push_save_bat()));
	connect(ui->open_bat,SIGNAL(clicked()), this, SLOT(push_open_bat()));
	connect(ui->model_path,SIGNAL(clicked()), this, SLOT(push_model_path()));
	//connect(ui->sub,SIGNAL(clicked()), this, SLOT(push_sub()));
	//connect(ui->result_image,SIGNAL(clicked()), this, SLOT(push_result_image()));
	connect(ui->mask_image, SIGNAL(clicked()), this, SLOT(push_mask_image()));
	connect(ui->assist_txt, SIGNAL(clicked()), this, SLOT(push_assist_txt()));
	connect(ui->template_image, SIGNAL(clicked()), this, SLOT(push_template_image()));
}
QStringList Dialog::file_filter(QStringList fileList)
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



void Dialog::push_open_image()
{
	file_path = QFileDialog::getExistingDirectory(this,"...", "./");
	//file_path = QString::fromLocal8Bit(file_path.toLocal8Bit().data());
 //   file_path = QString::fromLocal8Bit("E:/water_line/水尺_现场图片");
	if (!file_path.isEmpty())
	{
		int counter = ui->listWidget->count();
		for (int index = 0; index<counter; index++)
		{
			QListWidgetItem *item = ui->listWidget->takeItem(0);
			delete item;
		}
		//if (ui->listWidget->isemp)
		QDir qDir = QDir(file_path);
		fileList = qDir.entryList();
		fileList = file_filter(fileList);
		ui->listWidget->addItems(fileList);
	}
	int n = ui->listWidget->count();
	for (int i = 0; i < n; ++i) {
		ui->listWidget->item(i)->setSelected(true);
	}
}

void Dialog::push_assist_water()
{
	QString temp = QFileDialog::getOpenFileName(this,tr(""),
		QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
		tr("Files (*.exe)"));
	ui->textEdit_assist_water->setText(temp);
}
void Dialog::push_assist_txt()
{
	QString temp = QFileDialog::getOpenFileName(this, tr(""),
		QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
		tr("Files (*.txt)"));
	ui->textEdit_assist_txt->setText(temp);
}
void Dialog::push_template_image()
{
	QString temp = QFileDialog::getOpenFileName(this, tr(""),
		QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
		tr("Files (*.*)"));
	temp = get_string(temp, '_') + temp.right(4);

	ui->textEdit_template_image->setText(temp);
}
//void Dialog::push_ref()
//{
//	QString temp = QFileDialog::getOpenFileName(this, tr(""),
//		QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
//		tr("Files (*.bmp)"));
//	temp = temp.left(temp.size() - 6) + temp.right(4);
//	ui->textEdit_ref->setText(temp);
//}
void Dialog::push_model_path()
{
    QString temp = QFileDialog::getOpenFileName(this, tr(""),
        QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
        tr("Files (*.*)"));
    //temp = get_string(temp);
    //temp = temp.left(temp.size() - 6) + temp.right(4);
    ui->textEdit_model_path->setText(temp);
}

//void Dialog::push_sub()
//{
//    QString temp = QFileDialog::getOpenFileName(this,
//		tr("save bmp"),
//		QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
//		tr("Assit  Files (*.bmp)"));
//	temp = temp.left(temp.size() - 6) + temp.right(4);
//	ui->textEdit_sub->setText(temp);
//}
//void Dialog::push_result_image()
//{
//	QString file_path = QFileDialog::getExistingDirectory(this, "...", "./");
//	file_path = QString::fromLocal8Bit(file_path.toLocal8Bit().data());
//	ui->textEdit_result_image->setText(file_path);
//}
//void Dialog::push_result_txt()
//{
//	QString file_path = QFileDialog::getExistingDirectory(this, "...", "./");
//	file_path = QString::fromLocal8Bit(file_path.toLocal8Bit().data());
//	ui->textEdit_result_txt->setText(file_path);
//}
void Dialog::push_mask_image()
{
    QString file_path = QFileDialog::getExistingDirectory(this, "...", "./");
    file_path = QString::fromLocal8Bit(file_path.toLocal8Bit().data());
    ui->textEdit_mask_image->setText(file_path);
}
void Dialog::push_save_bat()
{
	QString assist_water = ui->textEdit_assist_water->toPlainText();
	QString model_path = ui->textEdit_model_path->toPlainText();
	QString mask_image = ui->textEdit_mask_image->toPlainText();
	QString assist_txt = ui->textEdit_assist_txt->toPlainText();
	QString template_image = ui->textEdit_template_image->toPlainText();
	if ((assist_water == "") ||
		(model_path == "") ||
		(mask_image == "")) {
		QMessageBox msgBox;
		msgBox.setText("problem.");
		msgBox.exec();
		return;
	}

	QString qStrFilePath = QFileDialog::getSaveFileName(this,
		tr("save bat"),
		QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
		tr("Assit  Files (*.bat)"));
	//QString qStrFilePath = "E:/water_line/bat/20180709.bat";
	QFile f(qStrFilePath);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
		cout << "Open failed." << endl;
		f.close();
		return;
	}

	QTextStream Out(&f);
	QList<QListWidgetItem*> temp = ui->listWidget->selectedItems();

	for (int i = 0; i < temp.size(); ++i) {
		Out << assist_water << " ";
		Out << file_path + "/" + temp[i]->text();
		Out << " " << model_path;
		Out << " mask_image=" << mask_image + "/mask_" + (temp[i]->text().left(temp[i]->text().size() - 4)) + (".png");
		Out << " assist_txt=" << assist_txt;
		Out << " template=" << template_image << endl;
	}
	f.close();
	QMessageBox msgBox;
	msgBox.setText("ok");
	msgBox.exec();
}

void Dialog::push_open_bat()
{
    QString temp = QFileDialog::getOpenFileName(this, tr(""),
        QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
        tr("Files (*.*)"));
    //QString temp = "E:/water_line/bat/20180618_ref.bat";
    if (temp=="")
        return;
	QFile file_in(temp);
	if (!file_in.open(QIODevice::ReadOnly))
	{
		QMessageBox msgBox;
		msgBox.setText("error");
		msgBox.exec();
	}
	QTextStream in(&file_in);
	QString str;
	str = in.readLine();
	QStringList list = str.split(" ");
	if (list.size() < 4)
		return;
	QStringList temp_list;

	// exe
	ui->textEdit_assist_water->setText(list[0]);
	// image
	str = list[1];
	temp_list = str.split("/");
	str.clear();
	if (temp_list.size() == 1) {
		file_path = "";
	}
	else {
		for (int i = 0; i < temp_list.size() - 2; ++i) {
			str = str + temp_list[i]+"/";
		}
		str = str + temp_list[temp_list.size() - 2];
		file_path = str;
	}	
	if (!file_path.isEmpty())
	{
		int counter = ui->listWidget->count();
		for (int index = 0; index<counter; index++)
		{
			QListWidgetItem *item = ui->listWidget->takeItem(0);
			delete item;
		}
		//if (ui->listWidget->isemp)
		QDir qDir = QDir(file_path);
		fileList = qDir.entryList();
		fileList = file_filter(fileList);
		ui->listWidget->addItems(fileList);
	}
	int n = ui->listWidget->count();
	for (int i = 0; i < n; ++i) {
		ui->listWidget->item(i)->setSelected(true);
	}
	//
	ui->textEdit_model_path->setText(list[2]);
	for (int i = 3; i < list.size(); ++i) {
		str = list[i];
		temp_list = str.split("=");
		str = temp_list[0];
		if (str == "assist_txt") {
			ui->textEdit_assist_txt->setText(temp_list[1]);
		}

		if (str == "template") {
			ui->textEdit_template_image->setText(temp_list[1]);
		}
		if (str == "mask_image") {
			str = temp_list[1];
			QStringList temp_str_list = str.split("/");
			str.clear();
			QString temp_file_path;
			if (temp_str_list.size() == 1) {
				temp_file_path = "";
			}
			else {
				for (int i = 0; i < temp_str_list.size() - 2; ++i) {
					str = str + temp_str_list[i] + "/";
				}
				str = str + temp_str_list[temp_str_list.size() - 2];
				temp_file_path = str;
			}
			ui->textEdit_mask_image->setText(temp_file_path);
		}
	}
}
