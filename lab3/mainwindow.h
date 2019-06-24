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

#define SIZE_INF 410

#define DX (SIZE_INF / 2 + width() / 2)
#define DY (height() / 2)

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
    void on_pushButton_7_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

private:
    Ui::MainWindow *ui;
    QColor background_color = "white";
    QColor color[50] = {"black"};

    int xo[50], yo[50], xk[50], yk[50];
    int kol = 0;
    int oper[50];
    int l[50], angle[50];
    int mean[50];
    int I = 255;
};

#endif // MAINWINDOW_H
