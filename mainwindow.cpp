#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <regex>
#include <QMessageBox>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Добавляем в ComboBox знаки сложения и вычитания, чтобы пользователь мог выбирать
    ui->comboBox->addItem("-");
    ui->comboBox->addItem("+");
    ui->comboBox->setCurrentIndex(-1); // Отменяем установку значения по умолчанию
    ui->comboBox_3->addItem("-");
    ui->comboBox_3->addItem("+");
    ui->comboBox_3->setCurrentIndex(-1);
    ui->comboBox_5->addItem("-");
    ui->comboBox_5->addItem("+");
    ui->comboBox_5->setCurrentIndex(-1);
    ui->comboBox_6->addItem("-");
    ui->comboBox_6->addItem("+");
    ui->comboBox_6->setCurrentIndex(-1);

    // Заполняем ComboBox значениями от 1 до 20, соответствующими номерам в списке группы
    for (int i = 1; i <= 20; ++i) {
        ui->comboBox_2->addItem(QString::number(i));
        ui->comboBox_4->addItem(QString::number(i));
        ui->comboBox_7->addItem(QString::number(i));
    }
    ui->comboBox_2->setCurrentIndex(-1);
    ui->comboBox_4->setCurrentIndex(-1);
    ui->comboBox_7->setCurrentIndex(-1);

    ui->horizontalFrame->setVisible(false);
    drawGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Функция для отрисовки графиков
void MainWindow::drawGraph() {
    QSplineSeries *series1 = new QSplineSeries();
    QSplineSeries *series2 = new QSplineSeries();
    QSplineSeries *series3 = new QSplineSeries();
    QSplineSeries *series4 = new QSplineSeries();

    for (float a = -10; a < 13; a += 0.1) {
        series1->append(a, 13 / (13 - a));
    }

    for (float a = 13.1; a <= 20; a += 0.1) {
        series2->append(a, 13 / (13 - a));
    }

    for (float a = -10; a <= 20; a += 0.1) {
        series3->append(a, sqrt(a * a + 13));
    }

    for (float a = -10; a <= 20; a += 0.1) {
        series4->append(a, -sqrt(a * a + 13));
    }

    series1->setColor(QColorConstants::Blue);
    series2->setColor(QColorConstants::Blue);
    series3->setColor(QColorConstants::Red);
    series4->setColor(QColorConstants::Red);

    // Рисуем первую точку пересечения графиков
    QScatterSeries *series5 = new QScatterSeries();
    series5->append(11.959, 12.491);
    series5->setMarkerSize(8);
    series5->setBorderColor(QColorConstants::Black);
    series5->setColor(QColorConstants::Black);

    // Рисуем вторую точку пересечения графиков
    QScatterSeries *series6 = new QScatterSeries();
    series6->append(13.905, -14.365);
    series6->setMarkerSize(8);
    series6->setBorderColor(QColorConstants::Black);
    series6->setColor(QColorConstants::Black);

    QChart *chart = new QChart();

    // Делаем линии графиков функций толще
    chart->legend()->hide();
    chart->addSeries(series1);
    QPen pen1 = series1->pen();
    pen1.setWidth(2);
    series1->setPen(pen1);

    chart->addSeries(series2);
    QPen pen2 = series2->pen();
    pen2.setWidth(2);
    series2->setPen(pen2);

    chart->addSeries(series3);
    QPen pen3 = series3->pen();
    pen3.setWidth(2);
    series3->setPen(pen3);

    chart->addSeries(series4);
    QPen pen4 = series4->pen();
    pen4.setWidth(2);
    series4->setPen(pen4);

    chart->addSeries(series5);
    chart->addSeries(series6);

    // Именуем оси и устанавливаем их интервалы
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).back()->setRange(-10, 20);
    chart->axes(Qt::Vertical).back()->setRange(-20, 20);
    chart->axes(Qt::Horizontal).back()->setTitleText("X1");
    chart->axes(Qt::Vertical).back()->setTitleText("X2");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->horizontalFrame);
}

double d, x1, x2, y_1, y_2, e;

// Подсчет значений функций
void calcFuncValues() {
    double f1, f2;
    f1 = x2 * (x1 - 13) + 13;
    f2 = x1 * x1 - x2 * x2 + 13;
    if (x2 > 0) {
        // Координаты первой точки пересечения
        y_1 = x1 - f1 / 24 - 23 * f2 / 1200;
        y_2 = x2 - f1 / 24 + f2 / 48;
    } else {
        // Координаты второй точки пересечения
        y_1 = x1 + 58 * f1 / 1569 - 26 * f2 / 1569;
        y_2 = x2 - 56 * f1 / 1569 - 29 * f2 / 1569;
    }
}

// Вывод результата
void MainWindow::outputOfResult() {
    ui->plainTextEdit->appendPlainText("x1 = " + QString::number(y_1));
    ui->plainTextEdit->appendPlainText("x2 = " + QString::number(y_2));
    ui->plainTextEdit->appendPlainText("Точность = " + QString::number(d, 'f', 8) + "\n");
}

// Обработчик нажатия на кнопку "Решить систему"
void MainWindow::on_pushButton_clicked() {
    ui->plainTextEdit->setPlainText("");
    // Проверка на правильность введенных данных
    if (ui->comboBox->currentText() == "-" && ui->comboBox_2->currentText() == "13"
            && ui->comboBox_3->currentText() == "+" && ui->comboBox_4->currentText() == "13"
            && ui->comboBox_5->currentText() == "-" && ui->comboBox_6->currentText() == "+"
            && ui->comboBox_7->currentText() == "13") {
        // Проверка на ввод начального приближения и точности
        regex patternForX("^((-?((0|[1-9][0-9]*)|((0|[1-9][0-9]*)\\.[0-9]+))))$");
        regex patternForEps("^(0|[1-9][0-9]*)\\.[0-9]+$");
        if (regex_match((ui->textEdit->toPlainText()).toStdString(), patternForX) && regex_match((ui->textEdit_2->toPlainText()).toStdString(), patternForX) && regex_match((ui->textEdit_3->toPlainText()).toStdString(), patternForEps)) {
            x1 = ui->textEdit->toPlainText().toFloat();
            x2 = ui->textEdit_2->toPlainText().toFloat();
            e = ui->textEdit_3->toPlainText().toDouble();

            int count = 1;
            do {
                calcFuncValues();
                d = fabs(y_1 - x1);
                if (fabs(y_2 - x2 > d)) {
                    d = fabs(y_2 - x2);
                }
                ui->plainTextEdit->appendPlainText("(итерация " + QString::number(count) + ")");
                outputOfResult();
                x1 = y_1;
                x2 = y_2;
                ++count;
            } while (d >= e);
        } else {
            QMessageBox::warning(this, "Ошибка", "Начальное приближение и точность не введены или введены некорректно!");
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Система составлена неверно!");
    }
}

// Обработчик нажатия на кнопку "Построить график"
void MainWindow::on_pushButton_2_clicked()
{
    // Проверка на правильность введенных данных
    if (ui->comboBox->currentText() == "-" && ui->comboBox_2->currentText() == "13"
            && ui->comboBox_3->currentText() == "+" && ui->comboBox_4->currentText() == "13"
            && ui->comboBox_5->currentText() == "-" && ui->comboBox_6->currentText() == "+"
            && ui->comboBox_7->currentText() == "13") {
        ui->horizontalFrame->setVisible(true);
    } else {
        QMessageBox::warning(this, "Ошибка", "Данные введены неверно!");
        ui->horizontalFrame->setVisible(false);
    }
}

