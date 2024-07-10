#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QtCore>
#include <cassert>
#include <clocale>

#include "calculator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // set the local to the minumum, so that strtod take '.'
    // as the decimal point
    setlocale(LC_NUMERIC, "C");
    QMainWindow w;
    QPushButton button;
    Calculator calculator(&w);

    w.setCentralWidget(&calculator);
    w.show();

    return app.exec();
}
