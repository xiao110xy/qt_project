#include "mainwindow.h"
#include "ui_mainwindow.h"


vector<string> MainWindow::getFiles(string folder, string firstname, string lastname)
{
	vector<string> files;
	//文件句柄  
	long long hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;   //大家可以去查看一下_finddata结构组成                            
								   //以及_findfirst和_findnext的用法，了解后妈妈就再也不用担心我以后不会编了  
	string p(folder);
	p.append("\\");
	p.append(firstname);
	p.append("*");
	p.append(lastname);
	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1) {
		do {
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib & _A_SUBDIR)) {
				continue;
				/*if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				getFiles(p.assign(folder).append("\\").append(fileinfo.name), files);*/
			}
			else {
				string temp = p.assign(folder).append("/").append(fileinfo.name);
				files.push_back(p.assign(folder).append("/").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	return files;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(open_include_directory()));
	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(open_lib_directory()));
	connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(save_result()));

	include_directory = qApp->applicationDirPath()+"/include";
    lib_directory = qApp->applicationDirPath()+"/lib";

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::open_include_directory()
{
	
	QString directory = QFileDialog::getExistingDirectory();
	if (directory.size() > 0) {
		include_directory = directory;
	}
}
void MainWindow::open_lib_directory()
{
	QString directory = QFileDialog::getExistingDirectory();
	if (directory.size() > 0) {
		lib_directory = directory;
	}
}


void MainWindow::save_result()
{
	vector<string> temp_strings = getFiles(string(lib_directory.toLocal8Bit()), "", ".lib");
	vector<string> temp_strings1 = getFiles(string(lib_directory.toLocal8Bit()), "", "d.lib");
	vector<string> temp_strings2;
	for (auto i : temp_strings) {
		bool flag = true;
		for (auto j : temp_strings1) {
			if (i == j) {
				flag = false;
				break;
			}
		}
		if (flag) {
			temp_strings2.push_back(i);
		}
	}
	ofstream file1("result_debug.txt");
	file1 << "INCLUDEPATH += " + string(include_directory.toLocal8Bit()) << endl;
	for (int i = 0; i < temp_strings1.size(); ++i) {
		if (i == 0)
			file1 << "LIBS += "+temp_strings1[i];
		else
			file1 << "        " + temp_strings1[i];
		if (i+1<temp_strings1.size())
			file1 << " \\" << endl;
	}
	file1.close();
	ofstream file2("result_release.txt");
	file2 << "INCLUDEPATH += " + string(include_directory.toLocal8Bit()) << endl;
	for (int i = 0; i < temp_strings2.size(); ++i) {
		if (i == 0)
			file2 << "LIBS += " + temp_strings2[i];
		else
			file2 << "        " + temp_strings2[i];
		if (i + 1<temp_strings2.size())
			file2 << " \\" << endl;
	}
	file2.close();
	QMessageBox::information(this, tr("Hmmm..."), tr("the file is ok"));
}



