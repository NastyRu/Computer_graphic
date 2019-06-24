#include "mainwindow.h"
#include "ui_mainwindow.h"

static int horisontal = 0;
static int vertical = 0;
static int ellipse = 0;
static int step = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->canvas->setScene(&scene);
}

int my_round(double x) {
    if (x < 0) {
        return int(x - 0.5);
    } else {
        return int(x + 0.5);
    }
}

int mean_value(int *x, int n) {
    int min = x[0];
    int max = x[0];

    for (int i = 0; i < n; i++) {
        if (x[i] < min) {
            min = x[i];
        }
        if (x[i] > max) {
            max = x[i];
        }
    }

    return my_round((max + min) / 2);
}

void pause(int msec) {
    QApplication::processEvents(QEventLoop::AllEvents, 1);
}

void MainWindow::paintEvent( QPaintEvent *)
{
    scene.clear();
    QPixmap pix;
    QPainter p;
    QImage img(width() - SIZE_INF, height(), QImage::Format_RGB32);

    ui->label_3->setStyleSheet("background: " + background.name());
    ui->label_5->setStyleSheet("background: " + edge.name());
    ui->label_4->setStyleSheet("background: " + fill.name());

    p.begin(&img);
    p.setPen(edge);

    int xo = x[0], yo = y[0];
    for (int i = 0; i < n - 1; i++) {
        if (x[i + 1] < 0) {
            p.drawLine(x[i], y[i], xo, yo);
            xo = x[i + 2];
            yo = y[i + 2];
            i++;
        } else {
            p.drawLine(x[i], y[i], x[i + 1], y[i + 1]);
        }
    }
    for (int i = 0; i < nc; i++) {
        p.drawEllipse(xc[i], yc[i], ac[i], bc[i]);
    }

    pix.convertFromImage(img);
    scene.addPixmap(pix);
    p.end();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        zx = event->pos().x();
        zy = event->pos().y();
        ui->lineEdit->setText(QString::number(zx));
        ui->lineEdit_2->setText(QString::number(zy));
    }
    if (event->button() == Qt::LeftButton) {
        if (ellipse) {
            if (0 == step) {
                xc[nc] = event->pos().x();
                yc[nc] = event->pos().y();
                step++;
            } else if (1 == step) {
                int r = abs(event->pos().x() - xc[nc]);
                xc[nc] -= r;
                ac[nc] = 2 * r;
                step++;
            } else if (2 == step) {
                int r = abs(event->pos().y() - yc[nc]);
                yc[nc] -= r;
                bc[nc] = 2 * r;
                nc++;
                step = 0;
                QString str = QString::number(xc[nc - 1] + ac[nc - 1] / 2) + "   " + QString::number(yc[nc - 1] + bc[nc - 1] / 2) + " " + QString::number(ac[nc - 1]) + " " + QString::number(bc[nc - 1]);
                ui->table->addItem(str);
            }
            repaint();
            return;
        }
        if (horisontal) {
            y[n] = y[n - 1];
            x[n] = event->pos().x();
        } else if (vertical) {
            x[n] = x[n - 1];
            y[n] = event->pos().y();
        } else {
            x[n] = event->pos().x();
            y[n] = event->pos().y();
        }
        n++;
        QString strx, stry;
        strx.setNum(x[n - 1]);
        stry.setNum(y[n - 1]);
        QString str = strx + "   " + stry;
        ui->table->addItem(str);
    }
    repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_4_clicked()
{
    QColorDialog *box = new QColorDialog;
    background = box->getColor();
    ui->label_3->update();
    repaint();
}

void MainWindow::on_pushButton_5_clicked()
{
    QColorDialog *box = new QColorDialog;
    edge = box->getColor();
    ui->label_5->update();
    repaint();
}

void MainWindow::on_pushButton_6_clicked()
{
    QColorDialog *box = new QColorDialog;
    fill = box->getColor();
    ui->label_4->update();
    repaint();
}

void MainWindow::on_pushButton_clicked()
{
    bool ok = 0;
    QString str = QInputDialog::getText(nullptr, "Input", "Введите х и у \n(через пробел) для точки:\n"
                                                          "Введите х,у,a,b \n(через пробел) для эллипса:", QLineEdit::Normal, nullptr, &ok);
    if (ok) {
         QStringList list = str.split(" ");
         if (!(list.size() == 2 || list.size() == 4)) {
             QMessageBox::critical(this,"ошибка","Неверно введены данные");
             return;
         }
         if (2 == list.size()) {
             bool yesx = 0;
             int xn = list[0].toInt(&yesx);
             bool yesy = 0;
             int yn = list[1].toInt(&yesy);

             if (yesx && yesy) {
                 x[n] = xn;
                 y[n] = yn;
                 n++;
                 QString strx, stry;
                 strx.setNum(x[n - 1]);
                 stry.setNum(y[n - 1]);
                 QString str = strx + "   " + stry;
                 ui->table->addItem(str);
                 repaint();
             } else {
                 QMessageBox::critical(this,"ошибка","Неверно введены данные");
                 return;
             }
         } else {
             bool yesx = 0;
             int xn = list[0].toInt(&yesx);
             bool yesy = 0;
             int yn = list[1].toInt(&yesy);
             bool yesa = 0;
             int an = list[2].toInt(&yesa);
             bool yesb = 0;
             int bn = list[3].toInt(&yesb);

             if (yesx && yesy && yesa && yesb) {
                 xc[nc] = xn - an;
                 yc[nc] = yn - bn;
                 ac[nc] = an * 2;
                 bc[nc] = bn * 2;
                 nc++;
                 QString str = QString::number(xc[nc - 1] + an / 2) + "   " + QString::number(yc[nc - 1] + bn / 2) + " " + QString::number(ac[nc - 1]) + " " + QString::number(bc[nc - 1]);
                 ui->table->addItem(str);
                 repaint();
             } else {
                 QMessageBox::critical(this,"ошибка","Неверно введены данные");
                 return;
             }
         }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    delete ui->table->takeItem(ui->table->currentRow());
    n--;
    for (int i = ui->table->currentRow(); i < n; i++) {
        x[i] = x[i + 1];
        y[i] = y[i + 1];
    }
    repaint();
}

void MainWindow::on_pushButton_3_clicked()
{
    while (ui->table->count() > 0) {
        delete ui->table->takeItem(0);
    }
    n = 0;
    repaint();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return)
    {
        x[n] = -1;
        y[n] = -1;
        n++;
        QString str = "end";
        ui->table->addItem(str);
        repaint();
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

    if (event->key() == Qt::Key_Alt) {
        ellipse = 1;
        step = 0;
        repaint();
        return;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
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

    if (event->key() == Qt::Key_Alt) {
        ellipse = 0;
        step = 0;
        repaint();
        return;
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    //scene.clear();
    QPixmap pix;
    QPainter p;
    QStack<int> stack_x;
    QStack<int> stack_y;
    QImage img(width() - SIZE_INF, height(), QImage::Format_RGB32);
    img.fill(background);

    p.begin(&img);
    p.setPen(edge);

    p.drawLine(0, 0, width() - SIZE_INF - 1, 0);
    p.drawLine(width() - SIZE_INF - 1, 0, width() - SIZE_INF - 1, height() - 1);
    p.drawLine(width() - SIZE_INF - 1, height() - 1, 0, height() - 1);
    p.drawLine(0, height() - 1, 0, 0);
    int xb = x[0], yb = y[0];
    for (int i = 0; i < n - 1; i++) {
        if (x[i + 1] < 0) {
            p.drawLine(x[i], y[i], xb, yb);
            xb = x[i + 2];
            yb = y[i + 2];
            i++;
        } else {
            p.drawLine(x[i], y[i], x[i + 1], y[i + 1]);
        }
    }
    for (int i = 0; i < nc; i++) {
        p.drawEllipse(xc[i], yc[i], ac[i], bc[i]);
    }
    pix.convertFromImage(img);
    scene.addPixmap(pix);

    if (-1 == zx) {
        p.end();
        return;
    }

    zx = ui->lineEdit->text().toInt();
    zy = ui->lineEdit_2->text().toInt();

    stack_x.push(zx);
    stack_y.push(zy);
    while (!stack_x.isEmpty()) {
        int xo = stack_x.pop();
        int yo = stack_y.pop();
        img.setPixel(xo, yo, fill.rgb());

        stack_x.push(xo);
        while (img.pixel(xo,yo) != edge.rgb()) {
            img.setPixel(xo, yo, fill.rgb());
            xo++;
        }
        int x_right = xo - 1;

        xo = stack_x.pop();
        while (img.pixel(xo,yo) != edge.rgb()) {
            img.setPixel(xo, yo, fill.rgb());
            xo--;
        }
        int x_left = xo + 1;

        if (ui->checkBox->isChecked()) {
            QPixmap pixn;
            pixn.convertFromImage(img);
            scene.addPixmap(pixn);
            pause(time);
        }

        xo = x_left;
        yo++;
        while (xo <= x_right) {
            bool fl = 0;
            while (img.pixel(xo,yo) != edge.rgb() && img.pixel(xo,yo) != fill.rgb() && xo <= x_right) {
                fl = 1;
                xo++;
            }
            if (1 == fl) {
                if (xo == x_right && img.pixel(xo,yo) != edge.rgb() && img.pixel(xo,yo) != fill.rgb()) {
                    stack_x.push(xo);
                    stack_y.push(yo);
                } else {
                    stack_x.push(xo - 1);
                    stack_y.push(yo);
                }
            }
            fl = 0;
            int xt = xo;
            while ((img.pixel(xo,yo) == edge.rgb() || img.pixel(xo,yo) == fill.rgb()) && xo < x_right) {
                xo++;
            }
            if (xo == xt) {
                xo++;
            }
        }

        xo = x_left;
        yo -= 2;
        while (xo <= x_right) {
            bool fl = 0;
            while (img.pixel(xo,yo) != edge.rgb() && img.pixel(xo,yo) != fill.rgb() && xo <= x_right) {
                fl = 1;
                xo++;
            }
            if (1 == fl) {
                if (xo == x_right && img.pixel(xo,yo) != edge.rgb() && img.pixel(xo,yo) != fill.rgb()) {
                    stack_x.push(xo);
                    stack_y.push(yo);
                } else {
                    stack_x.push(xo - 1);
                    stack_y.push(yo);
                }
            }
            fl = 0;
            int xt = xo;
            while ((img.pixel(xo,yo) == edge.rgb() || img.pixel(xo,yo) == fill.rgb()) && xo < x_right) {
                xo++;
            }
            if (xo == xt) {
                xo++;
            }
        }
    }

    QPixmap pixn;
    pixn.convertFromImage(img);
    scene.addPixmap(pixn);
    p.end();
}

void MainWindow::on_pushButton_8_clicked()
{
    x[n] = -1;
    y[n] = -1;
    n++;
    QString str = "end";
    ui->table->addItem(str);
    repaint();
}
