#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

static int rotate = 0;
static int scale = 0;
static int transfer = 0;
static int istop = 0, jstop = 0;
static int modify = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    for (double i = 0.1; i < 6; kline++, i += 0.01) {
        x[kline]= i * 50;
        y[kline] = -func(i) * 50;
    }

    for (double i = -r; i <= r; kcircle++, i+= 1) {
        xr[kcircle] = i;
        yr[kcircle] = circle(i + int(hx), r);
    }
    for (double i = r; i >= -r; kcircle++, i-= 1) {
        xr[kcircle] = i;
        yr[kcircle] = -circle(i + int(hx), r);
    }

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

double MainWindow::func(double x) {
    return 0.5 / x;
}

double MainWindow::circle(double x, double r) {
    return sqrt(r * r - (x - int(hx)) * (x - int(hx)));
}

void MainWindow::paintEvent( QPaintEvent *)
{
    QPainter p(this);

    p.setBrush(QBrush(Qt::white));
    p.drawRect(SIZE_INF, 0, width() - SIZE_INF, height());
    p.setBrush((QBrush(Qt::NoBrush)));
    p.setPen(QPen(Qt::black,1));
    p.drawLine(hx, 0, hx, height());
    p.drawLine(SIZE_INF, hy, width(), hy);

    for (int i = 0; i < kline; i++) {
        if (scale) {
            x[i] = x[i] * kx + (1 - kx) * ox;
            y[i] = y[i] * ky - (1 - ky) * oy;
        }
        if (rotate) {
            double bufx = x[i], bufy = y[i];
            x[i] = ox + (bufx - ox) * cos(angle) - (bufy - oy) * sin(angle);
            y[i] = oy + (bufx - ox) * sin(angle) + (bufy - oy) * cos(angle);
        }
        if (transfer) {
            x[i] += dx;
            y[i] -= dy;
        }
    }

    p.setPen(QPen(Qt::red, 1));
    for (int i = 0; i < kline - 1; i++) {
        p.drawLine(int(x[i] + hx), int(y[i] + hy), int(x[i + 1] + hx), int(y[i + 1] + hy));
    }

    for (int i = 0; i < kcircle; i+= 1) {
        if (scale) {
            xr[i] = xr[i] * kx + (1 - kx) * ox;
            yr[i] = yr[i] * ky - (1 - ky) * oy;
        }
        if (rotate) {
            double bufx = xr[i], bufy = yr[i];
            xr[i] = ox + (bufx - ox) * cos(angle) - (bufy - oy) * sin(angle);
            yr[i] = oy + (bufx - ox) * sin(angle) + (bufy - oy) * cos(angle);
        }
        if (transfer) {
            xr[i] = xr[i] + dx;
            yr[i] = yr[i] - dy;
        }
    }

    for (int i = 0; i < kcircle - 1; i++) {
        p.drawLine(int(xr[i] + hx), int(yr[i] + hy), int(xr[i + 1] + hx), int(yr[i + 1] + hy));
    }
    scale = 0;
    transfer = 0;
    rotate = 0;

    if (modify) {
        for (int i = 0; i < kline; i++) {
            for (int j = 0; j < kcircle; j++) {
                if ((fabs(x[i] - xr[j]) < 1) && (fabs(y[i] - yr[j]) < 1)) {
                    istop = i;
                    jstop = j;
                    break;
                }
            }
        }
        modify = 0;
    }

    for (int i = 0, k = 0; i < 11; i++, k += 8) {
        p.drawLine(int(x[istop + k + 10] + hx), int(y[istop + k + 10] + hy), int(xr[jstop - k] + hx), int(yr[jstop - k] + hy));
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString dx_s = ui->lineEdit_2->text();
    if (fabs(dx_s.toDouble()) < 0.00001 && QString::compare(dx_s, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    dx = dx_s.toDouble();
    QString dy_s = ui->lineEdit->text();
    if (fabs(dy_s.toDouble()) < 0.00001 && QString::compare(dy_s, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    dy = dy_s.toDouble();

    all_dx[kol] = dx;
    all_dy[kol] = dy;
    all_ox[kol] = ox;
    all_oy[kol] = oy;
    all_kx[kol] = kx;
    all_ky[kol] = ky;
    all_angle[kol] = angle;
    action[kol] = 1;
    kol++;

    transfer = 1;
    this->update();
}

void MainWindow::on_pushButton_2_clicked()
{
    QString ox_s = ui->lineEdit_3->text();
    if (fabs(ox_s.toDouble()) < 0.00001 && QString::compare(ox_s, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    ox = ox_s.toDouble();
    QString oy_s = ui->lineEdit_4->text();
    if (fabs(oy_s.toDouble()) < 0.00001 && QString::compare(oy_s, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    oy = oy_s.toDouble();
    QString kx_s = ui->lineEdit_5->text();
    if (fabs(kx_s.toDouble()) < 0.00001 && QString::compare(kx_s, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    kx = kx_s.toDouble();
    QString ky_s = ui->lineEdit_6->text();
    if (fabs(ky_s.toDouble()) < 0.00001 && QString::compare(ky_s, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    ky = ky_s.toDouble();

    all_dx[kol] = dx;
    all_dy[kol] = dy;
    all_ox[kol] = ox;
    all_oy[kol] = oy;
    all_kx[kol] = kx;
    all_ky[kol] = ky;
    all_angle[kol] = angle;
    action[kol] = 2;
    kol++;

    scale = 1;
    this->update();
}

void MainWindow::on_pushButton_3_clicked()
{
    QString ox_s = ui->lineEdit_7->text();
    if (fabs(ox_s.toDouble()) < 0.00001 && QString::compare(ox_s, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    ox = ox_s.toDouble();
    QString oy_s = ui->lineEdit_8->text();
    if (fabs(oy_s.toDouble()) < 0.00001 && QString::compare(oy_s, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    oy = oy_s.toDouble();
    QString angle_s = ui->lineEdit_9->text();
    if (fabs(angle_s.toDouble()) < 0.00001 && QString::compare(angle_s, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    angle = angle_s.toDouble() / 180 * pi;

    rotate = 1;
    all_dx[kol] = dx;
    all_dy[kol] = dy;
    all_ox[kol] = ox;
    all_oy[kol] = oy;
    all_kx[kol] = kx;
    all_ky[kol] = ky;
    all_angle[kol] = angle;
    action[kol] = 3;
    kol++;

    rotate = 1;
    this->update();
}

void MainWindow::on_pushButton_5_clicked()
{
    dx = 0;
    dy = 0;
    kx = 1;
    ky = 1;
    ox = 0;
    oy = 0;
    angle = 0;
    kol = 1;
    kline = 0;
    kcircle = 0;

    for (double i = 0.1; i < 6; kline++, i += 0.01) {
        x[kline]= i * 50;
        y[kline] = -func(i) * 50;
    }

    for (double i = -r; i <= r; kcircle++, i+= 1) {
        xr[kcircle] = i;
        yr[kcircle] = circle(i + int(hx), r);
    }
    for (double i = r; i >= -r; kcircle++, i-= 1) {
        xr[kcircle] = i;
        yr[kcircle] = -circle(i + int(hx), r);
    }
    modify = 1;

    this->update();
}

void MainWindow::on_pushButton_4_clicked()
{
    kol--;
    int n = kol;

    on_pushButton_5_clicked();
    modify = 0;
    kol = n;

    for (int i = 1; i < n; i++) {
        if (action[i] == 1) {
            dx = all_dx[i];
            dy = all_dy[i];

            transfer = 1;
            repaint();
        } else if (action[i] == 2) {
            ox = all_ox[i];
            oy = all_oy[i];
            kx = all_kx[i];
            ky = all_ky[i];

            scale = 1;
            repaint();
        } else if (action[i] == 3) {
            ox = all_ox[i];
            oy = all_oy[i];
            angle = all_angle[i];

            rotate = 1;
            repaint();
        }
        this->update();
    }
}
