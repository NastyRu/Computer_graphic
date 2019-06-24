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

#define pi 3.14
#define paramx width() / 2
#define paramy (height() + 181) / 2

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent( QPaintEvent *event );
    void function();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    double length(double, double, double, double);
    double x_1, y_1, x_2, y_2, x_3, y_3;
    double R = 0;
    int flag = 0;
    int error = 0;
    int n = 0;
    double *x, *y;

    int x_point, y_point;
};

#endif // MAINWINDOW_H
