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

#define SIZE_INF 310

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
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void clip(QPoint &p1, QPoint &p2);
    void is_convex();

private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;

    QColor backgroundcolor = "white";
    QColor linecolor = "blue";
    QColor dellinecolor = "red";
    QColor rectcolor = "green";


    QPoint lineb[100];
    QPoint linee[100];
    QPoint newlineb[100];
    QPoint newlinee[100];
    size_t n = 0;

    QPoint otsekatel[100];
    size_t no = 0;
    size_t ip = 0;
    int obhod = -1;
};

#endif // MAINWINDOW_H
