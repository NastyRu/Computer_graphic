#include "mainwindow.h"
#include "ui_mainwindow.h"

static bool axis = 0;
static bool begin = 1;
static bool paral = 0;
static int vvod = 0;

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

void MainWindow::paintEvent( QPaintEvent *)
{
    QPainter p(this);

    ui->label_7->setStyleSheet("background: " + rectcolor.name());
    ui->label_8->setStyleSheet("background: " + linecolor.name());
    ui->label_9->setStyleSheet("background: " + dellinecolor.name());

    p.setBrush(QBrush(backgroundcolor));
    p.drawRect(0, 0, width() - SIZE_INF, height());
    p.setClipRect(0, 0, width() - SIZE_INF, height());
    p.setBrush((QBrush(Qt::NoBrush)));

    p.setPen(QPen(linecolor));
    for (size_t i = 0; i < n; i++) {
        p.drawLine(lineb[i], linee[i]);
    }

    p.setPen(QPen(dellinecolor));
    for (size_t i = 0; i < n; i++) {
        p.drawLine(newlineb[i], newlinee[i]);
    }

    p.setPen(QPen(rectcolor));
    for (size_t i = 0; i < no; i++) {
        if (i == no - 1) {
            break;
        }
        p.drawLine(otsekatel[i], otsekatel[i + 1]);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    bool ok = 0;
    QString str = QInputDialog::getText(nullptr, "Input", "Введите координаты вершины отсекателя через пробел (х у)", QLineEdit::Normal, nullptr, &ok);

    if (ok) {
        QStringList list = str.split(" ");
        if (list.size() == 2) {
            bool yesx = 0;
            int x = list[0].toInt(&yesx);
            bool yesy = 0;
            int y = list[1].toInt(&yesy);

            if (yesx && yesy) {
                otsekatel[no].setX(x);
                otsekatel[no].setY(y);
                no++;
                repaint();
            } else {
                QMessageBox::critical(this,"ошибка","Неверно введены данные");
                return;
            }
        } else {
            QMessageBox::critical(this,"ошибка","Неверно введены данные");
            return;
        }
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    otsekatel[no] = otsekatel[0];
    no++;
    is_convex();
    if (0 == obhod) {
        QMessageBox::critical(this,"ошибка","Многоугольник не является выпуклым");
    }
    repaint();
    return;
}

void MainWindow::on_pushButton_2_clicked()
{
    bool ok = 0;
    QString str = QInputDialog::getText(nullptr, "Input", "Введите координаты отрезка через пробел (х1 у1 х2 у2)", QLineEdit::Normal, nullptr, &ok);

    if (ok) {
        QStringList list = str.split(" ");
        if (list.size() == 4) {
            bool yesx1 = 0;
            int x1 = list[0].toInt(&yesx1);
            bool yesy1 = 0;
            int y1 = list[1].toInt(&yesy1);
            bool yesx2 = 0;
            int x2 = list[2].toInt(&yesx2);
            bool yesy2 = 0;
            int y2 = list[3].toInt(&yesy2);

            if (yesx1 && yesy1 && yesx2 && yesy2) {
                lineb[n].setX(x1);
                lineb[n].setY(y1);
                linee[n].setX(x2);
                linee[n].setY(y2);
                n++;
                repaint();
            } else {
                QMessageBox::critical(this,"ошибка","Неверно введены данные");
                return;
            }
        } else {
            QMessageBox::critical(this,"ошибка","Неверно введены данные");
            return;
        }
    }
}

QVector2D setpoint(QPoint p1, QPoint p2) {
    QVector2D d;
    d.setX(p2.x() - p1.x());
    d.setY(p2.y() - p1.y());

    return d;
}

QVector3D setpoint3d(QPoint p1, QPoint p2) {
    QVector3D d;
    d.setX(p2.x() - p1.x());
    d.setY(p2.y() - p1.y());
    d.setZ(0);

    return d;
}

QVector3D multvec(QVector3D a, QVector3D b) {
    QVector3D m;
    m.setX(a.y() * b.z() - a.z() * b.y());
    m.setY(a.z() * b.x() - a.x() * b.z());
    m.setZ(a.x() * b.y() - a.y() * b.x());

    return m;
}

int sign(float x) {
    if (0 == int(x)) {
        return 0;
    } else if (x > 0) {
        return 1;
    } else {
        return -1;
    }
}

void MainWindow::is_convex() {
    QVector3D a = setpoint3d(otsekatel[0], otsekatel[1]);
    QVector3D b;
    QVector3D m;

    obhod = 0;

    for (int i = 0; i < int(no - 1); i++) {
        b = setpoint3d(otsekatel[i], otsekatel[i + 1]);
        m = multvec(a, b);

        if (0 == obhod) {
            obhod = sign(m.z());
        }
        if (obhod != sign(m.z())) {
            if (sign(m.z()) != 0) {
                obhod = 0;
                return;
            }
        }
        a = b;
    }
}

QVector2D find_norm(QPoint p1, QPoint p2, int obhod) {
    QVector2D d;
    d = setpoint(p1, p2);

    QVector2D n;
    if (obhod == -1) {
        n.setX(d.y());
        n.setY(-d.x());
    } else {
        n.setX(-d.y());
        n.setY(d.x());
    }

    return n;
}

int scalar(QVector2D a, QVector2D b) {
    return a.x() * b.x() + a.y() * b.y();
}

void nul(QPoint &p) {
    p.setX(0);
    p.setY(0);
}

void MainWindow::clip(QPoint &p1, QPoint &p2)
{
    QVector2D d = setpoint(p1, p2);
    QVector2D n;
    QVector2D w;
    int wsk = 0, dsk = 0;

    double tn = 0;
    double tv = 1;
    double t = 0;

    for (int i = 0; i < int(no - 1); i++) {
        n = find_norm(otsekatel[i], otsekatel[i + 1], obhod);
        w = setpoint(otsekatel[i], p1);

        wsk = scalar(w, n);
        dsk = scalar(d, n);

        if (dsk != 0) {
            t = -double(wsk) / dsk;
            if (dsk > 0) {
                if (t > 1) {
                    nul(p1);
                    nul(p2);
                    return;
                }
                tn = (t > tn) ? t : tn;
            } else {
                if (t < 0) {
                    nul(p1);
                    nul(p2);
                    return;
                }
                tv = (t < tv) ? t : tv;
            }
        } else if (wsk < 0) {
            nul(p1);
            nul(p2);
            return;
        }
    }
    if (tn > tv) {
        nul(p1);
        nul(p2);
        return;
    }

    QPoint tmp = p1;
    p1.setX(tmp.x() + int(round((p2.x() - tmp.x()) * tn)));
    p1.setY(tmp.y() + int(round((p2.y() - tmp.y()) * tn)));
    p2.setX(tmp.x() + int(round((p2.x() - tmp.x()) * tv)));
    p2.setY(tmp.y() + int(round((p2.y() - tmp.y()) * tv)));
}

void MainWindow::on_pushButton_clicked()
{
    if (0 == obhod) {
        QMessageBox::critical(this,"ошибка","Многоугольник не является выпуклым");
        return;
    }
    for (size_t i = 0; i < n; i++) {
        newlineb[i] = lineb[i];
        newlinee[i] = linee[i];
        clip(newlineb[i], newlinee[i]);
    }
    repaint();
}

void MainWindow::on_pushButton_7_clicked()
{
    QColorDialog *box = new QColorDialog;
    rectcolor = box->getColor();
    ui->label_7->update();
}

void MainWindow::on_pushButton_6_clicked()
{
    QColorDialog *box = new QColorDialog;
    linecolor = box->getColor();
    ui->label_8->update();
}

void MainWindow::on_pushButton_5_clicked()
{
    QColorDialog *box = new QColorDialog;
    dellinecolor = box->getColor();
    ui->label_9->update();
}

void MainWindow::on_pushButton_3_clicked()
{
    for (size_t i = 0; i < n; i++) {
        lineb[i].setX(0);
        lineb[i].setY(0);
        linee[i].setX(0);
        linee[i].setY(0);
        newlineb[i].setX(0);
        newlineb[i].setY(0);
        newlinee[i].setX(0);
        newlinee[i].setY(0);
    }
    n = 0;
    for (size_t i = 0; i < no; i++) {
        otsekatel[i].setX(0);
        otsekatel[i].setY(0);
    }
    no = 0;
    repaint();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift) {
        axis = 1;
        repaint();
        return;
    }

    if (event->key() == Qt::Key_Control) {
        paral = 1;
        repaint();
        return;
    }

    if (event->key() == Qt::Key_Return) {
        otsekatel[no] = otsekatel[0];
        no++;
        is_convex();
        if (0 == obhod) {
            QMessageBox::critical(this,"ошибка","Многоугольник не является выпуклым");
        }
        repaint();
        return;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift)
    {
        axis = 0;
        repaint();
        return;
    }

    if (event->key() == Qt::Key_Control) {
        paral = 0;
        vvod = 0;
        repaint();
        return;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (ui->radioButton_2->isChecked()) {
        int xb = event->pos().x();
        int yb = event->pos().y();

        if (paral) {
            if (0 == vvod) {
                for (size_t i = 0; i < no; i++) {
                    if (i == no - 1) {
                        break;
                    }
                    int a = otsekatel[i].y() - otsekatel[i + 1].y();
                    int b = otsekatel[i + 1].x() - otsekatel[i].x();
                    int c = otsekatel[i].x() * otsekatel[i + 1].y() - otsekatel[i + 1].x() * otsekatel[i].y();
                    if (abs(a * xb + b * yb + c) / sqrt(a * a + b * b) < 2) {
                        ip = i;
                        break;
                    }
                }
                vvod++;
            } else if (1 == vvod) {
                lineb[n].setX(xb);
                lineb[n].setY(yb);
                vvod++;
            } else if (2 == vvod) {
                if (abs(yb - lineb[n].y()) > abs(xb - lineb[n].x())) {
                    double tg = double(otsekatel[ip + 1].y() - otsekatel[ip].y()) / (otsekatel[ip + 1].x() - otsekatel[ip].x());
                    linee[n].setY(yb);
                    linee[n].setX(lineb[n].x() - (lineb[n].y() - yb) / tg);
                } else {
                    double tg = double(otsekatel[ip + 1].y() - otsekatel[ip].y()) / (otsekatel[ip + 1].x() - otsekatel[ip].x());
                    linee[n].setX(xb);
                    linee[n].setY(lineb[n].y() - (lineb[n].x() - xb) * tg);
                }
                n++;
                vvod = 0;
            }
            repaint();
            return;
        }
        if (begin) {
            lineb[n].setX(xb);
            lineb[n].setY(yb);
            begin = 0;
        } else {
            if (axis) {
                if (abs(xb - lineb[n].x()) < abs(yb - lineb[n].y())) {
                    linee[n].setX(lineb[n].x());
                    linee[n].setY(yb);
                } else {
                    linee[n].setY(lineb[n].y());
                    linee[n].setX(xb);
                }
            } else {
                linee[n].setX(xb);
                linee[n].setY(yb);
            }
            n++;
            begin = 1;
        }
    }
    if (ui->radioButton->isChecked()) {
        if (axis && no > 0) {
            if (abs(event->pos().x() - otsekatel[no - 1].x()) < abs(event->pos().y() - otsekatel[no - 1].y())) {
                otsekatel[no].setX(otsekatel[no - 1].x());
                otsekatel[no].setY(event->pos().y());
            } else {
                otsekatel[no].setY(otsekatel[no - 1].y());
                otsekatel[no].setX(event->pos().x());
            }
        } else {
            otsekatel[no].setX(event->pos().x());
            otsekatel[no].setY(event->pos().y());
        }
        no++;
    }
    repaint();
}
