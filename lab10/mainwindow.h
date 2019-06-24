#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <math.h>
#include <QVector2D>
#include <QVector3D>
#include "points.h"

#define RAD 0.017453
#define SIZE_INF 320


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event );
    double func(double, double);
    void float_horizon(QPainter &p);

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QColor backgroundcolor = "white";
    QColor linecolor = "blue";


    double xo, xk, dx;
    double zo, zk, dz;
    double anx = 0, any = 0, anz = 0;
    int form = 1;

    Point_3d centre = Point_3d(0, 0, 0);

    void drawline(Point_2d p1, Point_2d p2, double *top, double *bottom, QPainter &p);
};

#endif // MAINWINDOW_H
