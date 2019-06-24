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

int my_round(double x) {
    if (x < 0) {
        return int(x - 0.5);
    } else {
        return int(x + 0.5);
    }
}

void MainWindow::paintEvent( QPaintEvent *)
{
    QPainter p(this);
    p.setBrush(QBrush(background_color));
    p.drawRect(SIZE_INF, 0, width() - SIZE_INF, height());
    p.setClipRect(SIZE_INF, 0, width() - SIZE_INF, height());
    p.setBrush((QBrush(Qt::NoBrush)));

    for (int iv = 0; iv < kol; iv++) {
        if (0 == what[iv]) { //circle
            int dr = 0;
            if (1 == cir.type[iv]) {
                dr = int(cir.rk[iv] - cir.ro[iv]) / cir.n[iv];
                cir.xo[iv] = (width() - SIZE_INF) / 2;
                cir.yo[iv] = height() / 2;
            }

            p.setPen(cir.color[iv]);

            for (int k = 0; k <= cir.n[iv]; k++) {
                switch(cir.alg[iv]) {
                case 0: {// Каноническая
                    double r = (cir.ro[iv] + k * dr) * (cir.ro[iv] + k * dr);
                    int max = my_round((cir.ro[iv] + k * dr) / sqrt(2));

                    int x = 0;
                    int y = 0;
                    while (x <= max) {
                        y = my_round(sqrt(r - x * x));
                        p.drawPoint(cir.xo[iv] + x + SIZE_INF, cir.yo[iv] + y);
                        p.drawPoint(cir.xo[iv] - x + SIZE_INF, cir.yo[iv] + y);
                        p.drawPoint(cir.xo[iv] + x + SIZE_INF, cir.yo[iv] - y);
                        p.drawPoint(cir.xo[iv] - x + SIZE_INF, cir.yo[iv] - y);
                        x++;
                    }

                    y = my_round(sqrt(r - max * max));
                    while (y >= 0) {
                        x = my_round(sqrt(r - y * y));
                        p.drawPoint(cir.xo[iv] + x + SIZE_INF, cir.yo[iv] + y);
                        p.drawPoint(cir.xo[iv] - x + SIZE_INF, cir.yo[iv] + y);
                        p.drawPoint(cir.xo[iv] + x + SIZE_INF, cir.yo[iv] - y);
                        p.drawPoint(cir.xo[iv] - x + SIZE_INF, cir.yo[iv] - y);
                        y--;
                    }
                    break;
                }
                case 1: {// Параметрическая
                    double dt = 1.0 / (cir.ro[iv] + k * dr);
                    double max = double(M_PI) / 2 + dt;
                    double t = 0.0;

                    while (t <= max) {
                        int x = my_round((cir.ro[iv] + k * dr) * cos(t));
                        int y = my_round((cir.ro[iv] + k * dr) * sin(t));
                        p.drawPoint(cir.xo[iv] + x + SIZE_INF, cir.yo[iv] + y);
                        p.drawPoint(cir.xo[iv] - x + SIZE_INF, cir.yo[iv] + y);
                        p.drawPoint(cir.xo[iv] + x + SIZE_INF, cir.yo[iv] - y);
                        p.drawPoint(cir.xo[iv] - x + SIZE_INF, cir.yo[iv] - y);
                        t += dt;
                    }
                    break;
                }
                case 2: {// Брезенхем
                    int x = 0;
                    int y = (cir.ro[iv] + k * dr);

                    int err = 2 * (1 - (cir.ro[iv] + k * dr));
                    int err1 = 0;
                    int err2 = 0;

                    while (y >= 0) {
                        p.drawPoint(cir.xo[iv] + x + SIZE_INF, cir.yo[iv] + y);
                        p.drawPoint(cir.xo[iv] - x + SIZE_INF, cir.yo[iv] + y);
                        p.drawPoint(cir.xo[iv] + x + SIZE_INF, cir.yo[iv] - y);
                        p.drawPoint(cir.xo[iv] - x + SIZE_INF, cir.yo[iv] - y);
                        if (err < 0) {
                            err1 = 2 * err + 2 * y -1;
                            if (err1 <= 0) {
                                x++;
                                err += 2 * x + 1;
                            } else {
                                x++;
                                y--;
                                err += 2 * (x - y + 1);
                            }
                        } else if (err > 0) {
                            err2 = 2 * err - 2 * x - 1;
                            if (err2 <= 0) {
                                x++;
                                y--;
                                err += 2 * (x - y + 1);
                            } else {
                                y--;
                                err += -2 * y + 1;
                            }
                        } else {
                            x++;
                            y--;
                            err += 2 * (x - y + 1);
                        }
                    }
                    break;
                }
                case 3: {// Средняя точка
                    int point = int(5.0 / 4 - (cir.ro[iv] + k * dr));
                    int x = 0;
                    int y = int(cir.ro[iv] + k * dr);
                    p.drawPoint(cir.xo[iv] + x + SIZE_INF, cir.yo[iv] + y);
                    p.drawPoint(cir.xo[iv] - x + SIZE_INF, cir.yo[iv] + y);
                    p.drawPoint(cir.xo[iv] + x + SIZE_INF, cir.yo[iv] - y);
                    p.drawPoint(cir.xo[iv] - x + SIZE_INF, cir.yo[iv] - y);

                    p.drawPoint(cir.xo[iv] + y + SIZE_INF, cir.yo[iv] + x);
                    p.drawPoint(cir.xo[iv] - y + SIZE_INF, cir.yo[iv] + x);
                    p.drawPoint(cir.xo[iv] + y + SIZE_INF, cir.yo[iv] - x);
                    p.drawPoint(cir.xo[iv] - y + SIZE_INF, cir.yo[iv] - x);

                    while (x < y) {
                        x++;
                        if (point < 0) {
                            point += 2 * x + 1;
                        } else {
                            y--;
                            point += 2 * (x - y) + 1;
                        }
                        p.drawPoint(cir.xo[iv] + x + SIZE_INF, cir.yo[iv] + y);
                        p.drawPoint(cir.xo[iv] - x + SIZE_INF, cir.yo[iv] + y);
                        p.drawPoint(cir.xo[iv] + x + SIZE_INF, cir.yo[iv] - y);
                        p.drawPoint(cir.xo[iv] - x + SIZE_INF, cir.yo[iv] - y);

                        p.drawPoint(cir.xo[iv] + y + SIZE_INF, cir.yo[iv] + x);
                        p.drawPoint(cir.xo[iv] - y + SIZE_INF, cir.yo[iv] + x);
                        p.drawPoint(cir.xo[iv] + y + SIZE_INF, cir.yo[iv] - x);
                        p.drawPoint(cir.xo[iv] - y + SIZE_INF, cir.yo[iv] - x);

                    }
                    break;
                }
                case 4: {// Стандартный
                    p.drawEllipse(my_round(cir.xo[iv] - cir.ro[iv] - k * dr + SIZE_INF), my_round(cir.yo[iv] - cir.ro[iv] - k * dr), my_round(2 * (cir.ro[iv] + k * dr)), my_round(2 * (cir.ro[iv] + k * dr)));
                    break;
                }
                }
            }
        } else { //ellipse
            double da = 0;
            double db = 0;
            if (1 == ell.type[iv]) {
                da = double(ell.ak[iv] - ell.ao[iv]) / ell.n[iv];
                db = double(ell.bk[iv] - ell.bo[iv]) / ell.n[iv];
                ell.xo[iv] = (width() - SIZE_INF) / 2;
                ell.yo[iv] = height() / 2;
            }
            p.setPen(ell.color[iv]);

            for (int k = 0; k <= ell.n[iv]; k++) {
                switch(ell.alg[iv]) {
                case 0: {// Каноническая
                    double a = my_round((ell.ao[iv] + k * da) * (ell.ao[iv] + k * da));
                    double b = my_round((ell.bo[iv] + k * db) * (ell.bo[iv] + k * db));
                    double c1 = (ell.bo[iv] + k * db) / (ell.ao[iv] + k * da);
                    double c2 = (ell.ao[iv] + k * da) / (ell.bo[iv] + k * db);
                    int max = my_round(a / sqrt(a + b));

                    int x = 0;
                    int y = 0;
                    while (x <= max) {
                        y = my_round(c1 * sqrt(a - x * x));
                        p.drawPoint(ell.xo[iv] + x + SIZE_INF, ell.yo[iv] + y);
                        p.drawPoint(ell.xo[iv] - x + SIZE_INF, ell.yo[iv] + y);
                        p.drawPoint(ell.xo[iv] + x + SIZE_INF, ell.yo[iv] - y);
                        p.drawPoint(ell.xo[iv] - x + SIZE_INF, ell.yo[iv] - y);
                        x++;
                    }

                    y = my_round(c1 * sqrt(a - max * max));
                    while (y >= 0) {
                        x = my_round(c2 * sqrt(b - y * y));
                        p.drawPoint(ell.xo[iv] + x + SIZE_INF, ell.yo[iv] + y);
                        p.drawPoint(ell.xo[iv] - x + SIZE_INF, ell.yo[iv] + y);
                        p.drawPoint(ell.xo[iv] + x + SIZE_INF, ell.yo[iv] - y);
                        p.drawPoint(ell.xo[iv] - x + SIZE_INF, ell.yo[iv] - y);
                        y--;
                    }
                    break;
                }
                case 1: {// Параметрическая
                    double t = double(M_PI) / 2;
                    double dx = 1.0 / (ell.ao[iv] + k * da);
                    double dy = 1.0 / (ell.bo[iv] + k * db);
                    double a = my_round((ell.ao[iv] + k * da) * (ell.ao[iv] + k * da));
                    double b = my_round((ell.bo[iv] + k * db) * (ell.bo[iv] + k * db));
                    double tmax = acos(double(a) / (a + b));

                    while (t >= tmax) {
                        int x = my_round((ell.ao[iv] + k * da) * cos(t));
                        int y = my_round((ell.bo[iv] + k * db) * sin(t));
                        p.drawPoint(ell.xo[iv] + x + SIZE_INF, ell.yo[iv] + y);
                        p.drawPoint(ell.xo[iv] - x + SIZE_INF, ell.yo[iv] + y);
                        p.drawPoint(ell.xo[iv] + x + SIZE_INF, ell.yo[iv] - y);
                        p.drawPoint(ell.xo[iv] - x + SIZE_INF, ell.yo[iv] - y);
                        t -= dx;
                    }

                    t = tmax;
                    while (t >= -1) {
                        int x = my_round((ell.ao[iv] + k * da) * cos(t));
                        int y = my_round((ell.bo[iv] + k * db) * sin(t));
                        p.drawPoint(ell.xo[iv] + x + SIZE_INF, ell.yo[iv] + y);
                        p.drawPoint(ell.xo[iv] - x + SIZE_INF, ell.yo[iv] + y);
                        p.drawPoint(ell.xo[iv] + x + SIZE_INF, ell.yo[iv] - y);
                        p.drawPoint(ell.xo[iv] - x + SIZE_INF, ell.yo[iv] - y);
                        t -= dy;
                    }
                    break;
                }
                case 2: {// Брезенхем
                    double a = my_round((ell.ao[iv] + k * da) * (ell.ao[iv] + k * da));
                    double b = my_round((ell.bo[iv] + k * db) * (ell.bo[iv] + k * db));
                    int x = 0;
                    int y = ell.bo[iv] + k * db;

                    int err = a + b - a * 2 * y;
                    int err1 = 0;
                    int err2 = 0;

                    while (y >= 0) {
                        p.drawPoint(ell.xo[iv] + x + SIZE_INF, ell.yo[iv] + y);
                        p.drawPoint(ell.xo[iv] - x + SIZE_INF, ell.yo[iv] + y);
                        p.drawPoint(ell.xo[iv] + x + SIZE_INF, ell.yo[iv] - y);
                        p.drawPoint(ell.xo[iv] - x + SIZE_INF, ell.yo[iv] - y);

                        if (err < 0) {
                            err1 = 2 * err + a * 2 * y - a;

                            if (err1 < 0) {
                                x++;
                                err += b * 2 * x + b;
                            } else {
                                y--;
                                x++;
                                err += b * 2 * x + b + a - a * 2 * y;
                            }
                        } else if (err > 0) {
                            err2 = 2 * err - b * 2 * x - b;
                            if (err2 <= 0) {
                                y--;
                                x++;
                                err += b * 2 * x + b + a - a * 2 * y;
                            } else {
                                y--;
                                err += a - a * 2 * y;
                            }
                        } else {
                            y--;
                            x++;
                            err += b * 2 * x + b + a - a * 2 * y;
                        }
                    }
                    break;
                }
                case 3: {// Средняя точка
                    int x = 0;
                    int y = my_round(ell.bo[iv] + k * db);
                    double a = my_round((ell.ao[iv] + k * da) * (ell.ao[iv] + k * da));
                    double b = my_round((ell.bo[iv] + k * db) * (ell.bo[iv] + k * db));
                    double ad = 2 * a;
                    double bd = 2 * b;
                    int max = int(a / sqrt(a + b));
                    double t = -ad * y;
                    int dx = 0;

                    double f = b - a * y + a / 4;

                    while (x <= max) {
                        p.drawPoint(ell.xo[iv] + x + SIZE_INF, ell.yo[iv] + y);
                        p.drawPoint(ell.xo[iv] - x + SIZE_INF, ell.yo[iv] + y);
                        p.drawPoint(ell.xo[iv] + x + SIZE_INF, ell.yo[iv] - y);
                        p.drawPoint(ell.xo[iv] - x + SIZE_INF, ell.yo[iv] - y);

                        if (f > 0) {
                            y--;
                            t += ad;
                            f += t;
                        }
                        dx += bd;
                        x++;
                        f += dx + b;
                    }

                    f += 0.75 * (a - b) - (b * x + a * y);
                    t = bd * x;
                    int dy = my_round(-ad * y);

                    while (y >= 0) {
                        p.drawPoint(ell.xo[iv] + x + SIZE_INF, ell.yo[iv] + y);
                        p.drawPoint(ell.xo[iv] - x + SIZE_INF, ell.yo[iv] + y);
                        p.drawPoint(ell.xo[iv] + x + SIZE_INF, ell.yo[iv] - y);
                        p.drawPoint(ell.xo[iv] - x + SIZE_INF, ell.yo[iv] - y);

                        if (f < 0) {
                            x++;
                            t += bd;
                            f += t;
                        }
                        dy += ad;
                        f += dy + a;
                        y--;
                    }
                    break;
                }
                case 4: {// Стандартный
                    p.drawEllipse(my_round(ell.xo[iv] - ell.ao[iv] - k * da + SIZE_INF), my_round(ell.yo[iv] - ell.bo[iv] - k * db), my_round(2 * (ell.ao[iv] + k * da)), my_round(2 * (ell.bo[iv] + k * db)));
                    break;
                }
                }
            }
        }
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QColorDialog *box = new QColorDialog;
    cir.color[kol] = box->getColor();
    QPalette pal = ui->pushButton_3->palette();
    pal.setColor(QPalette::Button, cir.color[kol]);
    ui->pushButton_3->setAutoFillBackground(true);
    ui->pushButton_3->setPalette(pal);
    ui->pushButton_3->update();
}

void MainWindow::on_pushButton_7_clicked()
{
    cir.color[kol] = background_color;
    QPalette pal = ui->pushButton_3->palette();
    pal.setColor(QPalette::Button, cir.color[kol]);
    ui->pushButton_3->setAutoFillBackground(true);
    ui->pushButton_3->setPalette(pal);
    ui->pushButton_3->update();
}

void MainWindow::on_pushButton_6_clicked()
{
    QColorDialog *box = new QColorDialog;
    ell.color[kol] = box->getColor();
    QPalette pal = ui->pushButton_6->palette();
    pal.setColor(QPalette::Button, ell.color[kol]);
    ui->pushButton_6->setAutoFillBackground(true);
    ui->pushButton_6->setPalette(pal);
    ui->pushButton_6->update();
}

void MainWindow::on_pushButton_9_clicked()
{
    ell.color[kol] = background_color;
    QPalette pal = ui->pushButton_6->palette();
    pal.setColor(QPalette::Button, ell.color[kol]);
    ui->pushButton_6->setAutoFillBackground(true);
    ui->pushButton_6->setPalette(pal);
    ui->pushButton_6->update();
}

void MainWindow::on_pushButton_clicked()
{
    QString prov = ui->lineEdit->text();
    if (fabs(prov.toDouble()) < 0.00001 && QString::compare(prov, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    cir.xo[kol] = int(prov.toInt());

    prov = ui->lineEdit_3->text();
    if (fabs(prov.toDouble()) < 0.00001 && QString::compare(prov, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    cir.yo[kol] = int(prov.toInt());

    prov = ui->lineEdit_4->text();
    if (fabs(prov.toDouble()) < 0.00001 && QString::compare(prov, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    cir.ro[kol] = int(prov.toInt());

    cir.alg[kol] = ui->comboBox->currentIndex();
    what[kol] = 0;
    cir.type[kol] = 0;
    kol++;
    cir.color[kol] = cir.color[kol - 1];
    this->update();
}

void MainWindow::on_pushButton_5_clicked()
{
    QString prov = ui->lineEdit_2->text();
    if (fabs(prov.toDouble()) < 0.00001 && QString::compare(prov, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    ell.xo[kol] = int(prov.toInt());

    prov = ui->lineEdit_17->text();
    if (fabs(prov.toDouble()) < 0.00001 && QString::compare(prov, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    ell.yo[kol] = int(prov.toInt());

    prov = ui->lineEdit_18->text();
    if (fabs(prov.toDouble()) < 0.00001 && QString::compare(prov, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    ell.ao[kol] = int(prov.toInt());

    prov = ui->lineEdit_21->text();
    if (fabs(prov.toDouble()) < 0.00001 && QString::compare(prov, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    ell.bo[kol] = int(prov.toInt());

    ell.alg[kol] = ui->comboBox_3->currentIndex();
    what[kol] = 1;
    ell.type[kol] = 0;
    kol++;
    ell.color[kol] = ell.color[kol - 1];
    this->update();
}

void MainWindow::on_pushButton_8_clicked()
{
    QString prov = ui->lineEdit_5->text();
    if (fabs(prov.toDouble()) < 0.00001 && QString::compare(prov, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    cir.ro[kol] = int(prov.toInt());

    prov = ui->lineEdit_13->text();
    if (fabs(prov.toDouble()) < 0.00001 && QString::compare(prov, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    cir.rk[kol] = int(prov.toInt());

    prov = ui->lineEdit_14->text();
    if (prov.toInt() == 0 && QString::compare(prov, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    cir.n[kol] = int(prov.toInt());

    cir.alg[kol] = ui->comboBox->currentIndex();
    what[kol] = 0;
    cir.type[kol] = 1;
    kol++;
    cir.color[kol] = cir.color[kol - 1];
    this->update();
}

void MainWindow::on_pushButton_13_clicked()
{
    QString prov = ui->lineEdit_20->text();
    if (fabs(prov.toDouble()) < 0.00001 && QString::compare(prov, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    ell.ao[kol] = int(prov.toInt());

    prov = ui->lineEdit_16->text();
    if (fabs(prov.toDouble()) < 0.00001 && QString::compare(prov, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    ell.ak[kol] = int(prov.toInt());

    prov = ui->lineEdit_15->text();
    if (fabs(prov.toDouble()) < 0.00001 && QString::compare(prov, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    ell.bo[kol] = int(prov.toInt());

    prov = ui->lineEdit_19->text();
    if (fabs(prov.toDouble()) < 0.00001 && QString::compare(prov, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    ell.bk[kol] = int(prov.toInt());

    prov = ui->lineEdit_22->text();
    if (prov.toInt() == 0 && QString::compare(prov, "0") != 0) {
        QMessageBox::critical(this,"ошибка","Неверно введены данные");
        return;
    }
    ell.n[kol] = int(prov.toInt());

    ell.alg[kol] = ui->comboBox_3->currentIndex();
    what[kol] = 1;
    ell.type[kol] = 1;
    kol++;
    ell.color[kol] = ell.color[kol - 1];
    this->update();
}

void MainWindow::on_pushButton_12_clicked()
{
    for (int i = 0; i < kol; i++) {
        cir.xo[i] = 0;
        cir.yo[i] = 0;
        cir.alg[i] = 0;
        cir.ro[i] = 0;
        cir.rk[i] = 0;
        cir.n[i] = 0;
        cir.type[i] = 0;

        ell.color[i] = Qt::black;
        cir.color[i] = Qt::black;

        ell.xo[i] = 0;
        ell.yo[i] = 0;
        ell.alg[i] = 0;
        ell.ao[i] = 0;
        ell.ak[i] = 0;
        ell.bo[i] = 0;
        ell.bk[i] = 0;
        ell.n[i] = 0;
        ell.type[i] = 0;
    }

    kol = 0;

    QPalette pal = ui->pushButton_6->palette();
    pal.setColor(QPalette::Button, Qt::black);
    ui->pushButton_6->setAutoFillBackground(true);
    ui->pushButton_6->setPalette(pal);
    ui->pushButton_6->update();

    pal = ui->pushButton_3->palette();
    pal.setColor(QPalette::Button, Qt::black);
    ui->pushButton_3->setAutoFillBackground(true);
    ui->pushButton_3->setPalette(pal);
    ui->pushButton_3->update();
    this->update();
}
