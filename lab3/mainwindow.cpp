#include "mainwindow.h"
#include "ui_mainwindow.h"

static int circle = 0;

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

int sign(int x) {
    if (x > 0) {
        return 1;
    } else if (x == 0) {
        return 0;
    } else {
        return -1;
    }
}

void MainWindow::paintEvent( QPaintEvent *)
{
    QPainter p(this);
    p.setPen(QPen(Qt::black));
    p.setBrush(QBrush(background_color));
    p.drawRect(SIZE_INF, 0, width() - SIZE_INF, height());
    p.setBrush((QBrush(Qt::NoBrush)));
    p.drawLine(DX, 0, DX, height());
    p.drawLine(SIZE_INF, DY, width(), DY);

    for (int iv = 0; iv < kol; iv++) {
        int koln = 0;
        if (1 == mean[iv]) {
            koln = 360 / angle[iv];
            xo[iv] = 0;
            yo[iv] = 0;
            circle = 0;
            xk[iv] = l[iv];
            yk[iv] = 0;
        }
        p.setPen(color[iv]);
        for (int n = 0; n <= koln; n++) {
            if (4 == oper[iv]) {
                p.drawLine(xo[iv] + DX, -yo[iv] + DY, xk[iv] + DX, -yk[iv] + DY);
            }
            if (0 == oper[iv]) {
                if (xo[iv] == xk[iv] && yo[iv] == yk[iv]) {
                    p.drawPoint(xo[iv] + DX, -yo[iv] + DY);
                } else {
                    double x = xo[iv];
                    double y = yo[iv];
                    double l = 0;
                    double dx = xk[iv] - xo[iv];
                    double dy = yk[iv] - yo[iv];
                    double sx = 0, sy = 0;

                    if (fabs(dx) > fabs(dy)) {
                        l = fabs(dx);
                    } else {
                        l = fabs(dy);
                    }
                    sx = dx / l;
                    sy = dy / l;

                    for (int i = 0; i < l + 1; i++) {
                        p.drawPoint(int(x) + DX, int(-y) + DY);
                        x += sx;
                        y += sy;
                    }
                }
            }
            if (1 == oper[iv]) {
                if (xo[iv] == xk[iv] && yo[iv] == yk[iv]) {
                    p.drawPoint(xo[iv] + DX, -yo[iv] + DY);
                } else {
                    int x = xo[iv];
                    int y = yo[iv];
                    int dx = xk[iv] - xo[iv];
                    int dy = yk[iv] - yo[iv];
                    int sx = sign(dx);
                    int sy = sign(dy);
                    dx = abs(dx);
                    dy = abs(dy);

                    int swap = 0;
                    if (dy > dx) {
                        swap = 1;
                        int t = dx;
                        dx = dy;
                        dy = t;
                    }
                    double m = double(dy) / dx;
                    double e = m - 0.5;

                    for (int i = 0; i < dx + 1; i++) {
                        p.drawPoint(int(x + DX), int(-y + DY));
                        if (e > 0) {
                            if (0 == swap) {
                                y += sy;
                            } else {
                                x += sx;
                            }
                            e--;
                        }
                        if (1 == swap) {
                            y += sy;
                        } else {
                            x += sx;
                        }
                        e += m;
                    }
                 }
            }
            if (2 == oper[iv]) {
                if (xo[iv] == xk[iv] && yo[iv] == yk[iv]) {
                    p.drawPoint(xo[iv] + DX, -yo[iv] + DY);
                } else {
                    int x = xo[iv];
                    int y = yo[iv];
                    int dx = xk[iv] - xo[iv];
                    int dy = yk[iv] - yo[iv];
                    int sx = sign(dx);
                    int sy = sign(dy);
                    dx = abs(dx);
                    dy = abs(dy);

                    int swap = 0;
                    if (dy > dx) {
                        swap = 1;
                        int t = dx;
                        dx = dy;
                        dy = t;
                    }
                    double e = 2 * dy - dx;

                    for (int i = 0; i < dx + 1; i++) {
                        p.drawPoint(int(x + DX), int(-y + DY));
                        if (e > 0) {
                            if (0 == swap) {
                                y += sy;
                            } else {
                                x += sx;
                            }
                            e -= 2 * dx;
                        }
                        if (1 == swap) {
                            y += sy;
                        } else {
                            x += sx;
                        }
                        e += 2 * dy;
                    }
                }
            }
            if (3 == oper[iv]) {
                if (xo[iv] == xk[iv] && yo[iv] == yk[iv]) {
                    p.drawPoint(xo[iv] + DX, -yo[iv] + DY);
                } else {
                    int x = xo[iv];
                    //int xa = 0;
                    //int ya = 0;
                    int y = yo[iv];
                    int dx = xk[iv] - xo[iv];
                    int dy = yk[iv] - yo[iv];
                    int sx = sign(dx);
                    int sy = sign(dy);
                    dx = abs(dx);
                    dy = abs(dy);

                    int swap = 0;
                    if (dy > dx) {
                        swap = 1;
                        int t = dx;
                        dx = dy;
                        dy = t;
                    }
                    double m = double(dy) / dx * I;
                    double e = I / 2;
                    double w = I - m;
                    /*if (swap) {
                        xa = x - sx;
                        ya = y;
                    } else {
                        xa = x;
                        ya = y - sy;
                    }*/
                    QColor copy = color[iv];
                    copy.setAlpha(int(e));
                    p.setPen(copy);
                    p.drawPoint(x + DX, -y + DY);
                    for (int i = 0; i < dx; i++) {
                        if (e < w) {
                            if (0 == swap) {
                                x += sx;
                                //xa += sx;
                            } else {
                                y += sy;
                                //ya += sy;
                            }
                            e += m;
                        } else {
                            x += sx;
                            //xa += sx;
                            y += sy;
                            //ya += sy;
                            e -= w;
                        }
                        QColor copy = color[iv];
                        copy.setAlpha(int(e));
                        p.setPen(copy);
                        p.drawPoint(x + DX, -y + DY);
                        //copy.setAlpha(int(I - e));
                        //p.setPen(copy);
                        //p.drawPoint(xa + DX, -ya + DY);
                    }

                }
            }
            if (5 == oper[iv]) {
                if (xo[iv] == xk[iv] && yo[iv] == yk[iv]) {
                    p.drawPoint(xo[iv] + DX, -yo[iv] + DY);
                } else if (xo[iv] == xk[iv] || yo[iv] == yk[iv]) {
                    p.drawLine(xo[iv] + DX, -yo[iv] + DY, xk[iv] + DX, -yk[iv] + DY);
                } else {
                    int x0 = xo[iv], y0 = yo[iv], x1 = xk[iv], y1 = yk[iv];
                    int dx = abs(x1 - x0);
                    int dy = abs(y1 - y0);
                    int ed = 1;
                    if (dy < dx)
                    {
                        if (x1 < x0)
                        {
                            int t = x1;
                            x1 = x0;
                            x0 = t;
                            t = y1;
                            y1 = y0;
                            y0 = t;
                        }

                        float grad = float(dy) / dx;
                        if (y1 < y0)
                            grad = -grad;
                        float intery = y0 + grad;
                        p.drawPoint(x0 + DX, -y0 + DY);

                        for (int x = x0 + 1; x < x1; x++)
                        {
                            if (intery < 0) {
                                ed = -1;
                            }
                            QColor copy = color[iv];
                            copy.setAlpha(int(255 - fabs(intery - int(intery)) * 255));
                            p.setPen(copy);
                            p.drawPoint(x + DX, -int(intery) + DY);
                            copy = color[iv];
                            copy.setAlpha(int(fabs(intery - int(intery)) * 255));
                            p.setPen(copy);
                            p.drawPoint(x + DX, -(int(intery) + ed) + DY);
                            intery += grad;
                        }

                        p.setPen(color[iv]);
                        p.drawPoint(x1 + DX, -y1 + DY);
                    }
                    else
                    {
                        if (y1 < y0)
                        {
                            int t = x1;
                            x1 = x0;
                            x0 = t;
                            t = y1;
                            y1 = y0;
                            y0 = t;
                        }
                        float grad = float(dx) / dy;
                        if (x1 < x0)
                            grad = -grad;
                        float interx = x0 + grad;
                        p.drawPoint(x0 + DX, -y0 + DY);

                        for (int y = y0 + 1; y < y1; y++)
                        {
                            if (interx < 0) {
                                ed = -1;
                            }
                            QColor copy = color[iv];
                            copy.setAlpha(int(255 - fabs(interx - int(interx)) * 255));
                            p.setPen(copy);
                            p.drawPoint(int(interx) + DX, -y + DY);
                            copy = color[iv];
                            copy.setAlpha(int(fabs(interx - int(interx)) * 255));
                            p.setPen(copy);
                            p.drawPoint(int(interx) + ed + DX, -y + DY);
                            interx += grad;
                        }
                        p.setPen(color[iv]);
                        p.drawPoint(x1 + DX, -y1 + DY);
                    }
                }

            }

            if (koln > 0) {
                xk[iv] = int(l[iv] * cos((n + 1) * angle[iv] * 0.01745));
                yk[iv] = int(l[iv] * sin((n + 1) * angle[iv] * 0.01745));
            }
        }
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    color[kol] = background_color;
    QPalette pal = ui->pushButton_3->palette();
    pal.setColor(QPalette::Button, color[kol]);
    ui->pushButton_3->setAutoFillBackground(true);
    ui->pushButton_3->setPalette(pal);
    ui->pushButton_3->update();
}

void MainWindow::on_pushButton_3_clicked()
{
    QColorDialog *box = new QColorDialog;
    color[kol] = box->getColor();
    QPalette pal = ui->pushButton_3->palette();
    pal.setColor(QPalette::Button, color[kol]);
    ui->pushButton_3->setAutoFillBackground(true);
    ui->pushButton_3->setPalette(pal);
    ui->pushButton_3->update();
}

void MainWindow::on_pushButton_clicked()
{
    QString xo_s = ui->lineEdit->text();
    if (fabs(xo_s.toDouble()) < 0.00001 && QString::compare(xo_s, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    xo[kol] = int(xo_s.toDouble());
    QString yo_s = ui->lineEdit_3->text();
    if (fabs(yo_s.toDouble()) < 0.00001 && QString::compare(yo_s, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    yo[kol] = int(yo_s.toDouble());
    QString xk_s = ui->lineEdit_4->text();
    if (fabs(xk_s.toDouble()) < 0.00001 && QString::compare(xk_s, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    xk[kol] = int(xk_s.toDouble());
    QString yk_s = ui->lineEdit_2->text();
    if (fabs(yk_s.toDouble()) < 0.00001 && QString::compare(yk_s, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    yk[kol] = int(yk_s.toDouble());
    oper[kol] = ui->comboBox->currentIndex();
    mean[kol] = 0;
    kol++;
    this->update();
}

void MainWindow::on_pushButton_8_clicked()
{
    QString l_s = ui->lineEdit_5->text();
    if (fabs(l_s.toDouble()) < 0.00001 && QString::compare(l_s, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    l[kol] = int(l_s.toDouble());
    QString angle_s = ui->lineEdit_6->text();
    if (fabs(angle_s.toDouble()) < 0.00001 && QString::compare(angle_s, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    angle[kol] = int(angle_s.toDouble());
    mean[kol] = 1;
    oper[kol] = ui->comboBox->currentIndex();
    kol++;
    this->update();
}

void MainWindow::on_pushButton_9_clicked()
{
    for (int i = 0; i < kol; i++) {
        xo[i] = 0;
        xk[i] = 0;
        yo[i] = 0;
        yk[i] = 0;
        oper[i] = 0;
        l[i] = 0;
        angle[i] = 0;
        mean[i] = 0;
        color[i] = "black";
    }
    kol = 0;
    this->update();
}
