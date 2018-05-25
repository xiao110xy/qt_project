#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

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
    QGraphicsScene* m_graphicsScene1;
    QGraphicsScene* m_graphicsScene2;
    QGraphicsScene* m_graphicsScene3;
	QImage temp_image;
    QImage roi_image;




private slots:
    void onPressLoadImage();
    void onPressSaveImage();
    void onPressFitWindow();
    void on_image_origin_clicked();
    void on_roi_top_clicked();
    void on_roi_bottom_clicked();
    void on_roi_point_clicked();
    void on_base_fix_point_clicked();
    void on_preview_clicked();
    void on_imageprocess_clicked();
};

#endif // MAINWINDOW_H
