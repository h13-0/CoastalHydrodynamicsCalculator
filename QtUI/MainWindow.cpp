#include "MainWindow.hpp"

#define _USE_MATH_DEFINES

#include <thread>
#include <math.h>
#include <string>

#include <QLineSeries>
using namespace h13;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Setup ui first.
    ui.setupUi(this);

    //ui.WaveView->setChart(&chart);

    // Connection slot function.
    
    /**
     * @brief: Connect signal functions.
     */
    // MainWindow::SetDispersionEquationCalculatorResult
    connect(this, &MainWindow::SetDispersionEquationCalculatorResult, this, [this](double Result) { ui.DE_Output->setText(QString::number(Result)); });
    connect(this, &MainWindow::AddSeriesToChart, this,
        [this](ChartSeries_t& Series)
        {
            QLineSeries *series = new QLineSeries();
            for (auto& point : Series.Points)
            {
                series->append(point.X, point.Y);
            }
            chart.addSeries(series);
            ui.WaveView->setChart(&chart);
        }
    );


    /**
     * @brief: Connect buttons.
     */
    // Dispersion Equation Calculator.
    connect(ui.DE_Calc, &QPushButton::clicked, this, &MainWindow::runDispersionEquationCalculator);


}

MainWindow::~MainWindow()
{

}

bool MainWindow::tryParseDouble(QLineEdit* LineEdit, double &Value)
{
    using namespace std;
    try {
        string str = LineEdit->text().toStdString();
        Value = stod(str);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool MainWindow::tryParseInt(QLineEdit* LineEdit, int& Value)
{
    using namespace std;
    try {
        string str = LineEdit->text().toStdString();
        Value = stoi(str);
        return true;
    }
    catch (...)
    {
        return false;
    }
}


/// <summary>
/// Run the dispersion equation calculator.
/// </summary>
void MainWindow::runDispersionEquationCalculator()
{
    using namespace std;

    // Get Values from QTextEdit.
    // cycle.
    double cycle = 0.0;
    if (!tryParseDouble(ui.DE_Cycle, cycle))
    {
        return;
    }

    // depth.
    double depth = 0.0;
    if(!tryParseDouble(ui.DE_Depth, depth))
    {
        return;
    }

    // iterationAccuracy.
    double iterationAccuracy = 0.0;
    if (!tryParseDouble(ui.DE_IterationAccuracy, iterationAccuracy))
    {
        return;
    }

    // maximumIterations
    int maximumIterations = 0.0;
    if (!tryParseInt(ui.DE_MaximumIterations, maximumIterations))
    {
        return;
    }

    // initialWaveLength.
    double initialWaveLength = 0.0;
    initialWaveLength = 9.8 * pow(cycle, 2) / (2 * M_PI);
    ui.DE_InitialWaveLength->setText(QString::number(initialWaveLength));

    
    // Create calculate thread.
    std::thread(
        [this](double CycleTime, double Depth, double InitialWavelength, double IterationAccuracy, int MaximumIterations) -> int {
            int iterations = 0;
            
            double waveLength = InitialWavelength;
            double lastWaveLength = waveLength;
            ChartSeries_t series;
            series.Points.push_back(Points(iterations + 1, InitialWavelength));

            while (iterations < MaximumIterations)
            {
                lastWaveLength = waveLength;
                waveLength = ((9.8 * pow(CycleTime, 2)) / (2 * M_PI)) * tanh((2 * M_PI / waveLength) * Depth);
                series.Points.push_back(Points(iterations + 1, waveLength));

                if (fabs(waveLength - lastWaveLength) < IterationAccuracy)
                {
                    break;
                }
                iterations ++;
            };

            // Output data.
            SetDispersionEquationCalculatorResult(waveLength);
            AddSeriesToChart(series);

            return 0;
        }, cycle, depth, initialWaveLength, iterationAccuracy, maximumIterations
    ).detach();
}
