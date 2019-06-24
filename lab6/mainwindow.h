#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <cstdlib>
#include <math.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include <QColorDialog>
#include "QStandardItemModel"
#include <QInputDialog>
#include "QStandardItem"
#include <QApplication>
#include <QPixmap>
#include <QDesktopWidget>
#include <ctime>
#include <QThread>
#include <unistd.h>
#include <QTimer>
#include <QGraphicsView>
#include <QStack>

#define SIZE_INF 250

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void drawline(int xo, int yo, int xk, int yk, QImage &img, QRgb rgb);

private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();

private:
    QGraphicsScene scene;
    QTimer timer;
    int j = 0;
    Ui::MainWindow *ui;
    int x[100] = {0};
    int y[100] = {0};
    int n = 0;

    int xc[100] = {0};
    int yc[100] = {0};
    int ac[100] = {0};
    int bc[100] = {0};
    int nc = 0;

    int time;
    QColor col(int x, int y);
    QColor background = Qt::white;
    QColor edge = Qt::black;
    QColor fill = Qt::red;

    int zx = -1, zy = -1;
};



#endif // MAINWINDOW_H
