#include "mainwindow.h"
#include "ui_mainwindow.h"

static bool horisontal = 0;
static bool vertical = 0;
static bool begin = 1;
static bool otsek = 0;

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



void MainWindow::correctrect(QPoint &n, QPoint &e) {
    if (e.x() < n.x()) {
        xchng(e.rx(), n.rx());
    }
    if (e.y() < n.y()) {
        xchng(e.ry(), n.ry());
    }
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
    p.drawRect(otv.x(), otv.y(), abs(otv.x() - otn.x()), abs(otv.y() - otn.y()));

    p.setPen(QPen(linecolor));
    for (size_t i = 0; i < n; i++) {
        p.drawLine(lineb[i], linee[i]);
    }

    p.setPen(QPen(dellinecolor));
    for (size_t i = 0; i < n; i++) {
        p.drawLine(newlineb[i], newlinee[i]);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (otsek) {
        if (begin) {
            otv.setX(event->pos().x());
            otv.setY(event->pos().y());
            otn = otv;
            ui->lineEdit->setText(QString::number(otv.x()));
            ui->lineEdit_3->setText(QString::number(otv.y()));
            ui->lineEdit_2->setText("");
            ui->lineEdit_4->setText("");
            begin = 0;
        } else {
            otn.setX(event->pos().x());
            otn.setY(event->pos().y());
            correctrect(otv, otn);
            ui->lineEdit->setText(QString::number(otv.x()));
            ui->lineEdit_3->setText(QString::number(otv.y()));
            ui->lineEdit_2->setText(QString::number(otn.x()));
            ui->lineEdit_4->setText(QString::number(otn.y()));
            begin = 1;
        }
    } else {
        if (begin) {
            lineb[n].setX(event->pos().x());
            lineb[n].setY(event->pos().y());
            begin = 0;
        } else {
            if (horisontal) {
                linee[n].setY(lineb[n].y());
                linee[n].setX(event->pos().x());
            } else if (vertical) {
                linee[n].setX(lineb[n].x());
                linee[n].setY(event->pos().y());
            } else {
                linee[n].setX(event->pos().x());
                linee[n].setY(event->pos().y());
            }
            n++;
            begin = 1;
        }
    }
    repaint();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Alt) {
        otsek = 1;
        repaint();
        return;
    }

    if (event->key() == Qt::Key_Shift)
    {
        horisontal = 1;
        repaint();
        return;
    }

    if (event->key() == Qt::Key_Control)
    {
        vertical = 1;
        repaint();
        return;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Alt) {
        otsek = 0;
        repaint();
        return;
    }

    if (event->key() == Qt::Key_Shift)
    {
        horisontal = 0;
        repaint();
        return;
    }

    if (event->key() == Qt::Key_Control)
    {
        vertical = 0;
        repaint();
        return;
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    otv.setX(ui->lineEdit->text().toInt());
    otv.setY(height() - ui->lineEdit_3->text().toInt());
    otn.setX(ui->lineEdit_2->text().toInt());
    otn.setY(height() - ui->lineEdit_4->text().toInt());
    repaint();
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
    ui->lineEdit->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_4->setText("");
    otv.setX(0);
    otv.setY(0);
    otn.setX(0);
    otn.setY(0);
    repaint();
}

int sum(int *a, int n) {
    int s = 0;
    for (int i = 0; i < n; i++) {
        s += a[i];
    }
    return s;
}

int MainWindow::seeing(QPoint a, QPoint b, int *ta, int *tb) {
    int wid;

    if (a.x() < otv.x()) {
        ta[0] = 1;
    }
    if (a.x() > otn.x()) {
        ta[1] = 1;
    }
    if (a.y() < otv.y()) {
        ta[2] = 1;
    }
    if (a.y() > otn.y()) {
        ta[3] = 1;
    }

    if (b.x() < otv.x()) {
        tb[0] = 1;
    }
    if (b.x() > otn.x()) {
        tb[1] = 1;
    }
    if (b.y() < otv.y()) {
        tb[2] = 1;
    }
    if (b.y() > otn.y()) {
        tb[3] = 1;
    }

    if (sum(ta, 4) + sum(tb, 4) == 0) {
        wid = 1;
        return wid;
    }

    int p = 0;
    for (int i = 0; i < 4; i++) {
        p += ta[i] * tb[i];
    }

    if (p != 0) {
        wid = -1;
    } else {
        wid = 0;
    }
    return wid;
}

void MainWindow::clip(QPoint &p1, QPoint &p2) {
    int fl = -1;
    double m = 1;

    if (p2.x() - p1.x() != 0) {
        m = double(p2.y() - p1.y()) / (p2.x() - p1.x());
        if (0 == m) {
            fl = 1;
        } else {
            fl = 0;
        }
    }

    int o[4] = {otv.x(), otn.x(), otv.y(), otn.y()};

    for (int i = 0; i < 4; i++) {
        int ta[4] = {0};
        int tb[4] = {0};
        int pr = seeing(p1, p2, ta, tb);

        if (-1 == pr) {
            p1.setX(-1);
            p1.setY(-1);
            p2.setX(-1);
            p2.setY(-1);
            return;
        }
        if (1 == pr) {
            return;
        }

        if (ta[i] == tb[i]) {
            continue;
        }
        if (0 == ta[i]) {
            xchng(p1, p2);
        }

        if (-1 != fl) {
            if (i < 2) {
                p1.setY(m * (o[i] - p1.x()) + p1.y());
                p1.setX(o[i]);
                continue;
            }
            p1.setX((o[i] - p1.y()) / m + p1.x());
        }
        p1.setY(o[i]);
    }
}

void MainWindow::on_pushButton_clicked()
{
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
