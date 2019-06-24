#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete [] x;
    delete [] y;
    delete ui;
};

double MainWindow::length(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int max(double *array, int n, double R) {
    int max = 1;
    for (int i = 0; i < n ; i++) {
        if (max < (abs(array[i]) + R)) {
            max = int(abs(array[i]) + R);
        }
    }
    return max + max / 6;
}

void MainWindow::paintEvent( QPaintEvent *)
{
    QPainter p(this);

    p.setPen(QPen(Qt::white, 2));
    p.drawLine(0, 181, width(), 181);
    p.drawLine(width() / 2, 181, width() / 2, height());
    p.drawLine(0, (height() + 181) / 2, width(), (height() + 181) / 2);

    double k_x = 1, k_y = 1, k = 1;

    k_x = double(width()) / 2 / max(x, n, R);
    k_y = double((height() - 181)) / 2 / max(y, n, R);

    if (k_y > k_x) {
        k = k_x;
    } else {
        k = k_y;
    }

    if (1 == flag) {

        double a = length(x_1, y_1, x_2, y_2);
        double b = length(x_1, y_1, x_3, y_3);
        double c = length(x_2, y_2, x_3, y_3);

        double s = fabs((x_2 - x_3) * (y_1 - y_3) - (x_1 - x_3) * (y_2 - y_3)) / 2;

        double r = 2 * s / (a + b + c);
        R = a * b * c / 4 / s;
        double d = 2 * (x_1 * (y_2 - y_3) + x_2 * (y_3 - y_1) + x_3 * (y_1 - y_2));

        double xR = ((x_1 * x_1 + y_1 * y_1) * (y_2 - y_3) + (x_2 * x_2 + y_2 * y_2) * (y_3 - y_1) + (x_3 * x_3 + y_3 * y_3) * (y_1 - y_2)) / d;
        double yR = ((x_1 * x_1 + y_1 * y_1) * (x_3 - x_2) + (x_2 * x_2 + y_2 * y_2) * (x_1 - x_3) + (x_3 * x_3 + y_3 * y_3) * (x_2 - x_1)) / d;;

        double xr = (c * x_1 + b * x_2 + a * x_3) / (a + b + c);
        double yr = (c * y_1 + b * y_2 + a * y_3) / (a + b + c);

        k_x = double(width()) / 2 / max(x, n, R);
        k_y = double((height() - 181)) / 2 / max(y, n, R);

        if (k_y > k_x) {
            k = k_x;
        } else {
            k = k_y;
        }

        p.setPen(QPen(Qt::green, 4));
        p.drawEllipse(int((xr - r) * k + paramx), int(-(yr + r) * k + paramy), int(2 * r * k), int(2 * r * k));
        p.setPen(QPen(Qt::yellow, 4));
        p.drawEllipse(int((xR - R) * k + paramx), int(-(yR + R) * k + paramy), int(2 * R * k), int(2 * R * k));
        QPolygon triangle;
        p.setPen(QPen(Qt::red, 4));
        triangle << QPoint(int(x_1 * k + paramx), int(-y_1 * k + paramy)) << QPoint(int(x_2 * k + paramx), int(-y_2 * k + paramy)) << QPoint(int(x_3 * k + paramx), int(-y_3 * k + paramy));
        p.drawPolygon(triangle);

        ui->label_4->setGeometry(int(x_1 * k + paramx), int(-y_1 * k + paramy), 100, 15);
        ui->label_4->setText("(" + QString::number(x_1) + ";" + QString::number(y_1) + ")");
        ui->label_5->setGeometry(int(x_2 * k + paramx), int(-y_2 * k + paramy), 100, 15);
        ui->label_5->setText("(" + QString::number(x_2) + ";" + QString::number(y_2) + ")");
        ui->label_6->setGeometry(int(x_3 * k + paramx), int(-y_3 * k + paramy), 100, 15);
        ui->label_6->setText("(" + QString::number(x_3) + ";" + QString::number(y_3) + ")");

    }
    p.setPen(QPen(Qt::blue, 1));
    p.setBrush(QBrush(Qt::blue));

    for (int i = 0; i < n; i++) {
        QPoint point = QPoint(int(x[i] * k + paramx), int(-y[i] * k + paramy));
        p.drawEllipse(point, 3, 3);
    }
    p.setBrush((QBrush(Qt::NoBrush)));
}

void MainWindow::on_pushButton_clicked()
{
    QString text = ui->textEdit->toPlainText();
    QStringList strList = text.split('\n');
    QRegExp rxp = QRegExp("[0-9]");

    n  = 0;
    x = new double[100];
    y = new double[100];

    for (int i = 0; i < strList.size(); i++)
    {
        QString row = strList[i].trimmed();
        QStringList cols = row.split(QRegExp("\\s+"));
        if (cols.size() != 2) {
            QMessageBox::critical(this,"ошибка","Неверное количество аругментов");
            ui->label_4->setText("");
            ui->label_5->setText("");
            ui->label_6->setText("");
            n = 0;
            error = 1;
            flag = 0;
            this->update();
            break;
        } else if ((abs(cols[0].toDouble()) < 0.0001 && QString::compare(cols[0], "0") != 0) || (abs(cols[1].toDouble()) < 0.0001 && QString::compare(cols[1], "0") != 0)){
            QMessageBox::critical(this,"ошибка","Неверно введены данные");
            ui->label_4->setText("");
            ui->label_5->setText("");
            ui->label_6->setText("");
            n = 0;
            error = 2;
            flag = 0;
            this->update();
            break;
        } else {
            x[n] = cols[0].toDouble();
            y[n] = cols[1].toDouble();
            this->update();
            n++;
        }
    }
    if (error == 0) {
        if (n < 3) {
            QMessageBox::critical(this,"ошибка","Недостаточное количество точек");
            ui->label_4->setText("");
            ui->label_5->setText("");
            ui->label_6->setText("");
            flag = 0;
            this->update();
        } else {
            function();
        }
    } else {
        error = 0;
        delete [] x;
        delete [] y;
    }
}

void MainWindow::function() {
    double min = -1;

    for (int i = 0; i < (n - 2); i++) {
        for (int j = i + 1; j < (n - 1); j++) {
            for (int k = j + 1; k < n; k++) {
                double a = length(x[i], y[i], x[j], y[j]);
                double b = length(x[i], y[i], x[k], y[k]);
                double c = length(x[j], y[j], x[k], y[k]);

                if ((a + b <= c) || (a + c <= b) || (b + c <= a)) {
                    continue;
                }

                double s = fabs((x[j] - x[i]) * (y[k] - y[i]) - (x[k] - x[i]) * (y[j] - y[i])) / 2;

                double r = 2 * s / (a + b + c);
                double R = a * b * c / 4 / s;
                double diff = 4 * pi * fabs(r * r - R * R);

                if ((diff < min) || (min < 0)) {
                    min = diff;
                    x_1 = x[i];
                    y_1 = y[i];
                    x_2 = x[j];
                    y_2 = y[j];
                    x_3 = x[k];
                    y_3 = y[k];
                }
            }
        }
    }

    if (min < 0) {
        QMessageBox::critical(this,"ошибка","Невозможно получить треугольник");
        ui->label_4->setText("");
        ui->label_5->setText("");
        ui->label_6->setText("");
        flag = 0;
    } else {
        flag = 1;
        this->update();
    }
}
