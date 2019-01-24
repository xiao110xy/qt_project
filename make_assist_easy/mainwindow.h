#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <fstream>
#include <string>
#include <map>
using namespace std;
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"  
#include "opencv2/objdetect/objdetect.hpp"
using namespace cv;

#include <QMainWindow>
#include <QWidget>
#include <QFileDialog>
#include <QStandardPaths>
#include <QImageReader>
#include <QImageWriter>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QTextStream>
#include <QTranslator>
struct assist_information {
	vector<double> roi;
	vector<double> sub_roi;
	vector<vector<double>> point;
};
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
	QGraphicsScene *m_graphicsScene1;
	QGraphicsScene *m_graphicsScene2;
	assist_information assist;
	Point xy = Point(-1,-1);
public slots:
	void load_image_1();
	void load_image_2();
	void load_assist_1();
	void load_assist_2();
	void save_assist();
};

bool get_number(string line_string, vector<double>& temp);
#endif // MAINWINDOW_H
