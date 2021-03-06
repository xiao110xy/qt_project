﻿#ifndef WIDGET_H
#define WIDGET_H

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
using namespace std;


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
namespace Ui {
class Widget;
}
struct assist_information {
	vector<int> first_line;
	int roi[4];
	int sub_roi[4];
	vector<vector<double>> point;
};
class Widget : public QWidget
{
	Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
protected:
	//! Setup the various windows which can be brought up using the main window
	void setupWindows();
	//! Establish the myriad of signal and slot connections
	void setupConnections();
private:
    Ui::Widget *ui;
	QGraphicsScene *m_graphicsScene1;
	QGraphicsScene *m_graphicsScene2;
	QImage left_qimage;
	QImage right_qimage;
	int water_number = 0;
	vector<assist_information> assist;
	vector<double> get_line_number(string temp_name);
signals:
	void draw_roi(QRect rect);
	void draw_sub_roi(QRect rect);

private slots:
	void pushbotton_load_base_image();
	void pushbotton_load_wrap_image();
	void pushbotton_get_roi();
	void pushbotton_get_sub_roi();
	void pushbotton_savePts();
	void pushbotton_reloadPts();
};

#endif // WIDGET_H
