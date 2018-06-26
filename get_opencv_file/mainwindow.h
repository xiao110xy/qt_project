#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <map>
#include <iomanip>
#include <io.h>
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

private:
    Ui::MainWindow *ui;
    QString include_directory;
    QString lib_directory;
	vector<string> getFiles(string folder, string firstname, string lastname);
public slots:
    void  open_include_directory();
    void  open_lib_directory();
    void  save_result();
};

#endif // MAINWINDOW_H
