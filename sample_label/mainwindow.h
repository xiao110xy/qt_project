#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTextStream>
#include <QDir>
#include <QScrollBar>
#include <map>
#include "MyGraphicsView.h"
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
	void keyPressEvent(QKeyEvent *event) override;
private:
    Ui::MainWindow *ui;
	void setup_connections();
	QStringList file_filter(QStringList fileList);
	QString file_path;
	QGraphicsScene *m_graphicsScene;
	int order = -1;
	int water_line = -1;
public slots:
	void open_folder();
	void save_model();
	void load_model();
	void load_image(int row,int col);
	void set_value(QPointF data);
};

#endif // MAINWINDOW_H
