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

#define SIZE_INF 520
#define INDENT 50
#define hx SIZE_INF + (width() - SIZE_INF) / 2
#define hy height() / 2
#define pi 3.1415926536

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
    double func(double);
    double circle(double, double);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    double dx = 0, dy = 0;
    double kx = 1, ky = 1;
    double ox = 0, oy = 0;
    double angle = 0;
    double x[1000], y[1000];
    double xr[1000], yr[1000];
    int kline = 0;
    int r = 100;
    int kcircle = 0;

    int kol = 1;
    double all_dx[100] = {0}, all_dy[100] = {0};
    double all_kx[100] = {1}, all_ky[100] = {1};
    double all_ox[100] = {0}, all_oy[100] = {0};
    double all_angle[100] = {0};
    int action[100] = {0};
};

#endif // MAINWINDOW_H
