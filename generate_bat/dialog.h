#ifndef DIALOG_H
#define DIALOG_H
#include <vector>
#include <string>
#include <map>
#include <iostream>
using namespace std; 
#include <QTextCodec>
#include <QDialog>
#include <QFileDialog>
#include <QWidget>
#include <QStandardPaths>
#include <QMessageBox>
#include <QTextStream>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
	void setup_connections();
	QStringList fileList;
	QString file_path;
	QStringList file_filter(QStringList fileList);

private:
    Ui::Dialog *ui;
public slots:
	void push_open_image();
	void push_assist_water();
	void push_assist_txt();
	void push_assist_image();
	void push_ref();
	void push_template_path();
	void push_sub();
	void push_result_image();
	void push_result_txt();
	void push_save_bat();
	void push_open_bat();
    void push_mask_image();
};
QString get_string(QString input_string);

#endif // DIALOG_H
