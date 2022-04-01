#include <stdio.h>

#include <QApplication>
#include <MainWindow.hpp>

#include "EllipticCosineWaveWavelengthCalculator.hpp"


int main(int argc, char** argv)
{
    using namespace h13;
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
