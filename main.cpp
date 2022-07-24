#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("МПИ для системы нелинейных уравнений");
    w.show();
    return a.exec();
}
