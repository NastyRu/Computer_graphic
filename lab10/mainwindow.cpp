#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

double MainWindow::func(double x, double z) {
    switch(form) {
    case 0:
        return 100 * sin(0.0001 * x * x) + 100 * cos(0.0001 * z * z);
    case 1:
        return x * x / 25 + z * z / 25;
    case 2:
        return sqrt(x * x + z * z);
    case 3:
        return cos(x * z * 0.01) * 100;
    case 4:
        return -sin(0.05 * sqrt(x * x + z * z)) * 50;
    }

}

int sign(double x) {
    if (x > 0) {
        return 1;
    } else if (0 == x) {
        return 0;
    } else {
        return -1;
    }
}

void MainWindow::drawline(Point_2d p1, Point_2d p2, double *top, double *bottom, QPainter &p) {
    int x = int(p1.get_x());
    int y = int(p1.get_y());
    int dx = int(p2.get_x() - p1.get_x());
    int dy = int(p2.get_y() - p1.get_y());
    int sx = sign(dx);
    int sy = sign(dy);
    dx = abs(dx);
    dy = abs(dy);

    if (0 == dx && 0 == dy && x >= (-(width() - SIZE_INF) / 2) && x < ((width() - SIZE_INF) / 2)) {
        if (y >= top[x + (width() - SIZE_INF) / 2]) {
            top[x + (width() - SIZE_INF) / 2] = y;
            p.drawPoint(x + (width() - SIZE_INF) / 2, -y + height() / 2);
        }
        if (y <= bottom[x + (width() - SIZE_INF) / 2]) {
            bottom[x + (width() - SIZE_INF) / 2] = y;
            p.drawPoint(x + (width() - SIZE_INF) / 2, -y + height() / 2);
        }
        return;
    }

    int swap = 0;
    if (dy > dx) {
        int t = dx;
        dx = dy;
        dy = t;
        swap = 1;
    }

    int y_max = top[x + (width() - SIZE_INF) / 2];
    int y_min = bottom[x + (width() - SIZE_INF) / 2];

    int e = 2 * dy - dx;

    for (int i = 0; i < dx + 1; i++) {
        if (x >= (-(width() - SIZE_INF) / 2) && x < ((width() - SIZE_INF) / 2)) {
            if (y >= top[x + (width() - SIZE_INF) / 2]) {
                if (y >= y_max) {
                    y_max = y;
                }
                p.drawPoint(x + (width() - SIZE_INF) / 2, height() / 2 - y);
            }

            if (y <= bottom[x + (width() - SIZE_INF) / 2]) {
                if (y <= y_min) {
                    y_min = y;
                }
                p.drawPoint(x + (width() - SIZE_INF) / 2, height() / 2 - y);
            }
        }

        if (e > 0) {
            if (swap) {
                top[x + (width() - SIZE_INF) / 2] = y_max;
                bottom[x + (width() - SIZE_INF) / 2] = y_min;
                x += sx;
                y_max = top[x + (width() - SIZE_INF) / 2];
                y_min = bottom[x + (width() - SIZE_INF) / 2];
            } else {
                y += sy;
            }
            e -= 2 * dx;
        }

        if (!swap) {
            top[x+ (width() - SIZE_INF) / 2] = y_max;
            bottom[x+ (width() - SIZE_INF) / 2] = y_min;
            x += sx;
            y_max = top[x + (width() - SIZE_INF) / 2];
            y_min = bottom[x + (width() - SIZE_INF) / 2];
        } else {
            y += sy;
        }
        e += 2 * dy;
    }
}

void MainWindow::float_horizon(QPainter &p) {
    Point_3d left(-1, -1, 0);
    Point_3d right(-1, -1, 0);

    double *top = new double[width() - SIZE_INF];
    double *bottom = new double[width() - SIZE_INF];

    for (int i = 0; i <= (width() - SIZE_INF); i++) {
        top[i] = -height();
        bottom[i] = height();
    }

    for(double z = zk; z >= zo; z -= dz) {
        Point_3d tmp(xo, func(xo, z), z);
        tmp.rotate_x(anx, centre);
        tmp.rotate_y(any, centre);
        tmp.rotate_z(anz, centre);

        if (left.get_x() != -1) {
            drawline(Point_2d(tmp), Point_2d(left), top, bottom, p);
        }
        left.set_x(tmp.get_x());
        left.set_y(tmp.get_y());

        for(double x = xo; x <= xk; x += dx) {
            Point_3d buf(x, func(x, z), z);
            buf.rotate_x(anx, centre);
            buf.rotate_y(any, centre);
            buf.rotate_z(anz, centre);

            drawline(Point_2d(tmp), Point_2d(buf), top, bottom, p);
            tmp.set_x(buf.get_x());
            tmp.set_y(buf.get_y());
        }

        if (z != zk) {
            right.set_x(xk);
            right.set_y(func(xk, z - dz));
            right.set_z(z - dz);
            right.rotate_x(anx, centre);
            right.rotate_y(any, centre);
            right.rotate_z(anz, centre);
            tmp.set_z(right.get_z());
            drawline(Point_2d(tmp), Point_2d(right), top, bottom, p);
        }
    }
}

void MainWindow::paintEvent( QPaintEvent *)
{
    QPainter p(this);

    p.setBrush(QBrush(backgroundcolor));
    p.drawRect(0, 0, width() - SIZE_INF, height());
    p.setClipRect(0, 0, width() - SIZE_INF, height());
    p.setBrush((QBrush(Qt::NoBrush)));

    Point_3d tmp;

    /*p.setPen(QPen("red"));
    for (double z = zo; z < zk; z += dz) {
        tmp = Point_3d(xo, func(xo, z), z);
        tmp.rotate_x(anx, centre);
        tmp.rotate_y(any, centre);
        tmp.rotate_z(anz, centre);
        for (double x = xo; x < xk; x += dx) {
            Point_3d point(x, func(x, z), z);
            point.rotate_x(anx, centre);
            point.rotate_y(any, centre);
            point.rotate_z(anz, centre);
            p.drawPoint(point.get_x() + (width() - SIZE_INF) / 2, -point.get_y() + height() / 2);
            p.drawLine(tmp.get_x()+ (width() - SIZE_INF) / 2, -tmp.get_y()+ height() / 2, point.get_x()+ (width() - SIZE_INF) / 2, -point.get_y()+ height() / 2);
            tmp = point;
        }
    }*/
    p.setPen(QPen(linecolor));
    float_horizon(p);
}

void MainWindow::on_pushButton_3_clicked()
{
    QString l = ui->lineEdit->text();
    bool ok = 0;
    l.toDouble(&ok);
    if (!ok) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    xo = l.toDouble();

    l = ui->lineEdit_2->text();
    ok = 0;
    l.toDouble(&ok);
    if (!ok) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    xk = l.toDouble();

    l = ui->lineEdit_3->text();
    ok = 0;
    l.toDouble(&ok);
    if (!ok){
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    dx = l.toDouble();

    l = ui->lineEdit_7->text();
    ok = 0;
    l.toDouble(&ok);
    if (!ok){
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    zo = l.toDouble();

    l = ui->lineEdit_9->text();
    ok = 0;
    l.toDouble(&ok);
    if (!ok){
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    zk = l.toDouble();

    l = ui->lineEdit_8->text();
    ok = 0;
    l.toDouble(&ok);
    if (!ok){
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    dz = l.toDouble();

    form = ui->comboBox->currentIndex();


    anx = 0;
    any = 0;
    anz = 0;

    repaint();
}

void MainWindow::on_pushButton_4_clicked()
{
    anx += ui->spinBox->value();
    any += ui->spinBox_3->value();
    anz += ui->spinBox_2->value();

    repaint();
}
