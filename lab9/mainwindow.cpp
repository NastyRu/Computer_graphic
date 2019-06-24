#include "mainwindow.h"
#include "ui_mainwindow.h"

static bool axis = 0;
static bool line = 0;
static int point = 0;

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

    p.setPen(QPen(rectcolor));
    for (size_t i = 0; i < no; i++) {
        if (i == no - 1) {
            break;
        }
        p.drawLine(otsekatel[i], otsekatel[i + 1]);
    }

    p.setPen(QPen(linecolor));
    for (size_t i = 0; i < np; i++) {
        if (i == np - 1) {
            break;
        }
        p.drawLine(polygon[i], polygon[i + 1]);
    }

    p.setPen(QPen(dellinecolor));
    for (size_t i = 0; i < newnp; i++) {
        if (i == newnp - 1) {
            break;
        }
        /*for (int j = 0; j < int(i); j++) {
            if ((newpolygon[j] == newpolygon[i]) && (newpolygon[j + 1] == newpolygon[i + 1])) {
                p.setPen(QPen(rectcolor));
            }
        }*/
        p.drawLine(newpolygon[i], newpolygon[i + 1]);
        //p.setPen(QPen(dellinecolor));
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
    QString str = QInputDialog::getText(nullptr, "Input", "Введите координаты вершины многоугольника через пробел (х у)", QLineEdit::Normal, nullptr, &ok);

    if (ok) {
        QStringList list = str.split(" ");
        if (list.size() == 2) {
            bool yesx = 0;
            int x = list[0].toInt(&yesx);
            bool yesy = 0;
            int y = list[1].toInt(&yesy);

            if (yesx && yesy) {
                polygon[np].setX(x);
                polygon[np].setY(y);
                np++;
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

void MainWindow::on_pushButton_9_clicked()
{
    polygon[np] = polygon[0];
    np++;
    repaint();
    return;
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

int scalar(QVector2D a, QVector2D b) {
    return a.x() * b.x() + a.y() * b.y();
}

void nul(QPoint &p) {
    p.setX(0);
    p.setY(0);
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

void MainWindow::copy(QPoint *a, QPoint *b, size_t n) {
    for (int i = 0; i < int(n); i++) {
        a[i] = b[i];
    }
}

void MainWindow::clip()
{
    QPoint s;
    QVector2D d;
    QVector2D n;
    QVector2D w;
    int wsk = 0, dsk = 0;
    double t = 0;

    QPoint b[100];
    size_t nb = 0;

    copy(newpolygon, polygon, np);
    newnp = np;

    for (int i = 0; i < int(no) - 1; i++) {
        for (int j = 0; j < int(newnp); j++) {
            if (j > 0) {
                d = setpoint(newpolygon[j], s);
                n = find_norm(otsekatel[i], otsekatel[i + 1], obhod);
                w = setpoint(s, otsekatel[i]);

                wsk = scalar(w, n);
                dsk = scalar(d, n);

                if (dsk != 0) {
                    t = -double(wsk) / dsk;
                    if (t >= 0 && t <= 1) {
                        b[nb].setX(s.x() + int((newpolygon[j].x() - s.x()) * t));
                        b[nb].setY(s.y() + int((newpolygon[j].y() - s.y()) * t));
                        nb++;
                    }
                }
            }
            s = newpolygon[j];

            QVector2D vspom = setpoint(otsekatel[i], s);
            n = find_norm(otsekatel[i], otsekatel[i + 1], obhod);
            if (scalar(n, vspom) > 0) {
                b[nb].setX(s.x());
                b[nb].setY(s.y());
                nb++;
            }
        }
        copy(newpolygon, b, nb);
        if (0 == nb) {
            newnp = 0;
            return;
        }
        newpolygon[nb].setX(newpolygon[0].x());
        newpolygon[nb].setY(newpolygon[0].y());
        newnp = nb + 1;
        nb = 0;
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (0 == obhod) {
        QMessageBox::critical(this,"ошибка","Многоугольник не является выпуклым");
        return;
    }

    clip();
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
    for (size_t i = 0; i < np; i++) {
        polygon[i].setX(0);
        polygon[i].setY(0);
    }
    np = 0;
    newnp = 0;
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
        line = 1;
        repaint();
        return;
    }

    if (event->key() == Qt::Key_Alt) {
        point = 1;
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
        line = 0;
        repaint();
        return;
    }

    if (event->key() == Qt::Key_Alt) {
        point = 0;
        repaint();
        return;
    }
}

double len(QPoint p1, QPoint p2) {
    return sqrt(pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (ui->radioButton_2->isChecked()) {
        if (axis && np > 0) {
            if (abs(event->pos().x() - polygon[np - 1].x()) < abs(event->pos().y() - polygon[np - 1].y())) {
                polygon[np].setX(polygon[np - 1].x());
                polygon[np].setY(event->pos().y());
            } else {
                polygon[np].setY(polygon[np - 1].y());
                polygon[np].setX(event->pos().x());
            }
        } else if (line) {
            int xb = event->pos().x();
            int yb = event->pos().y();
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
            polygon[np].setY(int(((otsekatel[ip + 1].y() - otsekatel[ip].y()) * (xb - otsekatel[ip].x())) / (otsekatel[ip+1].x() - otsekatel[ip].x())) + otsekatel[ip].y());
            polygon[np].setX(xb);
        } else if (point) {
            QPoint now = event->pos();
            for (int i = 0; i < no; i++) {
                if (len(now, otsekatel[i]) < 4) {
                    polygon[np] = otsekatel[i];
                    break;
                }
            }
        } else {
            polygon[np].setX(event->pos().x());
            polygon[np].setY(event->pos().y());
        }
        np++;
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
        } else if (line) {
            int xb = event->pos().x();
            int yb = event->pos().y();
            for (size_t i = 0; i < np; i++) {
                if (i == np - 1) {
                    break;
                }
                int a = polygon[i].y() - polygon[i + 1].y();
                int b = polygon[i + 1].x() - polygon[i].x();
                int c = polygon[i].x() * polygon[i + 1].y() - polygon[i + 1].x() * polygon[i].y();
                if (abs(a * xb + b * yb + c) / sqrt(a * a + b * b) < 2) {
                    ip = i;
                    break;
                }
            }
            otsekatel[no].setY(int(((polygon[ip + 1].y() - polygon[ip].y()) * (xb - polygon[ip].x())) / (polygon[ip+1].x() - polygon[ip].x())) + polygon[ip].y());
            otsekatel[no].setX(xb);
        } else if (point) {
            QPoint now = event->pos();
            for (int i = 0; i < np; i++) {
                if (len(now, polygon[i]) < 4) {
                    otsekatel[no] = polygon[i];
                    break;
                }
            }
        } else {
            otsekatel[no].setX(event->pos().x());
            otsekatel[no].setY(event->pos().y());
        }
        no++;
    }
    repaint();
}
