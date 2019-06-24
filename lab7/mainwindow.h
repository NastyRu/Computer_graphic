#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QColorDialog>

#define SIZE_INF 300

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

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;

    QPoint lineb[100];
    QPoint linee[100];
    QPoint newlineb[100];
    QPoint newlinee[100];
    QPoint otv;
    QPoint otn;
    size_t n = 0;

    QColor backgroundcolor = "white";
    QColor linecolor = "blue";
    QColor dellinecolor = "red";
    QColor rectcolor = "green";

    template <typename T>
    void xchng(T &x, T &y) {
        T buf = x;
        x = y;
        y = buf;
    }
    void correctrect(QPoint &n, QPoint &e);
    void clip(QPoint &p1, QPoint &p2);
    int seeing(QPoint a, QPoint b, int *ta, int *tb);
};

#endif // MAINWINDOW_H
