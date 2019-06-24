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

#define SIZE_INF 420
#define pi 3.1415927

struct Circle {
    int xo[50] = {0};
    int yo[50] = {0};
    QColor color[50];
    int alg[50] = {0};
    int ro[50] = {0};
    int rk[50] = {0};
    int n[50] = {0};
    int type[50] = {0};
};

struct Ellipse {
    int xo[50] = {0};
    int yo[50] = {0};
    QColor color[50];
    int alg[50] = {0};
    int ao[50] = {0};
    int bo[50] = {0};
    int ak[50] = {0};
    int bk[50] = {0};
    int n[50] = {0};
    int type[50] = {0};
};

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

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

private:
    Ui::MainWindow *ui;

    QColor background_color = "white";
    Ellipse ell;
    Circle cir;
    int kol = 0;
    int what[50];
};

#endif // MAINWINDOW_H
