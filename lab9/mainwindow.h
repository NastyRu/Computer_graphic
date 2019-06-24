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
    void clip();
    void copy(QPoint *a, QPoint *b, size_t n);
    void is_convex();

private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
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


    QPoint polygon[100];
    QPoint newpolygon[100];
    size_t np = 0;
    size_t newnp = 0;

    QPoint otsekatel[100];
    size_t no = 0;
    size_t ip = 0;
    int obhod = -1;
};

#endif // MAINWINDOW_H
