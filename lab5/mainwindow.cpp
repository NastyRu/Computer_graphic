#include "mainwindow.h"
#include "ui_mainwindow.h"

static int horisontal = 0;
static int vertical = 0;
static int click = 0;

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
    pix.convertFromImage(img);
    scene.addPixmap(pix);
    p.end();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
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
    }
    QString strx, stry;
    strx.setNum(x[n - 1]);
    stry.setNum(y[n - 1]);
    QString str = strx + "   " + stry;
    ui->table->addItem(str);
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
    on_pushButton_7_clicked();
}

void MainWindow::on_pushButton_5_clicked()
{
    QColorDialog *box = new QColorDialog;
    edge = box->getColor();
    ui->label_5->update();
    on_pushButton_7_clicked();
}

void MainWindow::on_pushButton_6_clicked()
{
    QColorDialog *box = new QColorDialog;
    fill = box->getColor();
    ui->label_4->update();
    on_pushButton_7_clicked();
}

void MainWindow::on_pushButton_clicked()
{
    bool ok = 0;
    QString str = QInputDialog::getText(nullptr, "Input", "Введите х и у \n(через пробел):", QLineEdit::Normal, nullptr, &ok);
    if (ok) {
         QStringList list = str.split(" ");
         if (list.size() != 2) {
             QMessageBox::critical(this,"ошибка","Неверно введены данные");
             return;
         }
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
    on_pushButton_7_clicked();
}

void MainWindow::on_pushButton_3_clicked()
{
    while (ui->table->count() > 0) {
        delete ui->table->takeItem(0);
    }
    n = 0;
    on_pushButton_7_clicked();
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
}

void MainWindow::on_pushButton_7_clicked()
{
    //scene.clear();
    QPixmap pix;
    QPainter p;
    QImage img(width() - SIZE_INF, height(), QImage::Format_RGB32);
    img.fill(background);

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
    pix.convertFromImage(img);
    scene.addPixmap(pix);

    int xt[100] = { 0 };
    int yt[100] = { 0 };
    int nt = 0;
    for (int i = 0; i < n; i++) {
        xt[nt] = x[i];
        yt[nt] = y[i];
        nt++;
        if (x[i + 1] < 0) {
            xt[nt] = xt[0];
            yt[nt] = yt[0];
            nt++;
            i++;
            int xp = mean_value(xt, nt);
            for (int j = 0; j < nt - 1; j++) {
                int xo, yo, xk, yk;
                if (xt[j] < xt[j + 1]) {
                    xo = xt[j];
                    yo = yt[j];
                    xk = xt[j + 1];
                    yk = yt[j + 1];
                } else {
                    xk = xt[j];
                    yk = yt[j];
                    xo = xt[j + 1];
                    yo = yt[j + 1];
                }

                int ky = yo < yk ? 1 : -1;
                double kx;

                if (yo != yk) {
                    kx = double(xk - xo) / abs(yo - yk);
                }
                else {
                    continue;
                }

                if (yo < yk) {
                    yk++;
                } else {
                    yo++;
                }

                for (double y = yo + ky, x = xo + kx; fabs(y - yk) > 0; x += kx, y += ky) {
                    for (double xn = x - kx; my_round(xn + 1) <= xp; xn++) {
                        QRgb rgb = img.pixel(my_round(xn), my_round(y));
                        if (rgb == fill.rgb()) {
                            img.setPixel(my_round(xn), my_round(y), background.rgb());
                        } else if (rgb == background.rgb()) {
                            img.setPixel(my_round(xn), my_round(y), fill.rgb());
                        }
                    }

                    for (double xn = xp; xn < x-1; xn++) {
                        QRgb rgb = img.pixel(my_round(xn), my_round(y-1));
                        if (rgb == fill.rgb()) {
                            img.setPixel(my_round(xn), my_round(y-1), background.rgb());
                        } else if (rgb == background.rgb()) {
                            img.setPixel(my_round(xn), my_round(y-1) , fill.rgb());
                        }
                    }
                    if (ui->checkBox->isChecked()) {
                        QPixmap pixn;
                        pixn.convertFromImage(img);
                        scene.addPixmap(pixn);
                        pause(time);
                    }
                 }

           }
           nt = 0;
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
