#include "MainWindow.hpp"

#define _USE_MATH_DEFINES

#include <thread>
#include <math.h>
#include <string>
#include <iostream>

// Dispersion Equation Wavelength Calculator.
#include "DispersionEquationWavelengthCalculator.hpp"

// Elliptic Cosine Wave Wavelength Calculator.
#include "EllipticIntegral.hpp"
//#include "EllipticCosineWaveWavelengthCalculator.hpp"

using namespace h13;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Setup ui first.
    ui.setupUi(this);

    //ui.WaveView->setChart(&chart);
    
    ui.WaveView->setRenderHint(QPainter::Antialiasing);

    // Connect signal functions.
    /**
     * @brief: Connect result output signals.
     */
    // MainWindow::SetDispersionEquationCalculatorResult
    connect(this, &MainWindow::SetDispersionEquationCalculatorResult, this, 
        [this](double Result) { ui.DE_Output -> setText(QString::number(Result)); }
    );
    
    // MainWindow::SetEllipticCosineWaveErgodicCalculatorWaveLengthResult
    connect(this, &MainWindow::SetEllipticCosineWaveErgodicCalculatorWaveLengthResult, this,
        [this](double Result) { ui.ECWE_WaveLengthOutput->setText(QString::number(Result)); }
    );

    // MainWindow::SetEllipticCosineWaveErgodicCalculatorKappaResult
    connect(this, &MainWindow::SetEllipticCosineWaveErgodicCalculatorKappaResult, this, 
        [this](double Result) { ui.ECWE_KappaOutput -> setText(QString::number(Result));  }
    );



    // Chart.
    // 
    connect(this, &MainWindow::SetChartTitle, this, [this](std::string Title) { chart.setTitle(QString::fromStdString(Title)); });

    // MainWindow::AddSeriesToChart
    connect(this, &MainWindow::AddSeriesToChart, this,
        [this](ChartSeries_t& Series)
        {
            QLineSeries* series = new QLineSeries();
            // Only 1000 points are displayed.
            if (Series.Points.size() <= 1000)
            {
                for (auto& point : Series.Points)
                {
                    series->append(point.X, point.Y);
                }
            }
            else {
                float samplingRate = Series.Points.size() / 1000.0;
                int lastSamplingPoint = 0;
                for (int index = 0; index < Series.Points.size(); index++)
                {
                    if (index - lastSamplingPoint >= samplingRate)
                    {
                        series->append(Series.Points[index].X, Series.Points[index].Y);
                        lastSamplingPoint = index;
                    }
                }
            }

            series->setName(QString::fromStdString(Series.Name));
            seriesList.push_back(series);
            chart.addSeries(series);
            ui.WaveView->setChart(&chart);
            chart.createDefaultAxes();
        }
    );
    // MainWindow::ClearChart
    connect(this, &MainWindow::ClearChart, this,
        [this](void)
        {
            for (auto series : seriesList)
            {
                series -> ~QLineSeries();
            }
            seriesList.clear();
        }
    );

    // Connection slot function.
    /**
     * @brief: Connect buttons.
     */
    // Dispersion Equation Calculator.
    connect(ui.DE_Calc, &QPushButton::clicked, this, &MainWindow::runDispersionEquationCalculator);
    connect(ui.ECWE_Calc, &QPushButton::clicked, this, &MainWindow::runEllipticCosineWaveErgodicCalculator);

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
        [this](double Period, double Depth, double InitialWavelength, double IterationAccuracy, int MaximumIterations) -> int
        {
            int iterations = 0;
            
            double waveLength = InitialWavelength;
            double lastWaveLength = waveLength;
            ChartSeries_t series;
            series.Name = "WaveLength";
            series.Points.push_back(Points(iterations + 1, InitialWavelength));

            while (iterations < MaximumIterations)
            {
                lastWaveLength = waveLength;

                // Calculate next iteration.
                waveLength = DispersionEquationWavelengthCalcForward(Period, Depth, lastWaveLength);

                // Add point to chart.
                series.Points.push_back(Points(iterations + 1, waveLength));

                if (fabs(waveLength - lastWaveLength) < IterationAccuracy)
                {
                    break;
                }
                iterations ++;
            };

            // Output data.
            SetDispersionEquationCalculatorResult(waveLength);
            ClearChart();
            SetChartTitle("弥散方程求波长");
            AddSeriesToChart(series);

            return 0;
        }, cycle, depth, initialWaveLength, iterationAccuracy, maximumIterations
    ).detach();
}

void h13::MainWindow::runEllipticCosineWaveErgodicCalculator()
{
    // Get Values from QTextEdit.
    // period.
    double period = 0.0;
    if (!tryParseDouble(ui.ECWE_WavePeriod, period))
    {
        return;
    }

    // depth.
    double depth = 0.0;
    if(!tryParseDouble(ui.ECWE_Depth, depth))
    {
        return;
    }

    // waveHeight.
    double waveHeight = 0.0;
    if(!tryParseDouble(ui.ECWE_WaveHeight, waveHeight))
    {
        return;
    }

    // ergodicAccuracy
    double ergodicAccuracy = 0.0;
    if (!tryParseDouble(ui.ECWE_ErgodicAccuracy, ergodicAccuracy))
    {
        return;
    }

    // Create calculate thread.
    std::thread(
        [this](double Period, double Depth, double WaveHeight, double ErgodicAccuracy) -> int
        {
            // Create series.
            ChartSeries K_kappaSeries;
            K_kappaSeries.Name = "K(κ)";

            ChartSeries E_kappaSeries;
            E_kappaSeries.Name = "E(κ)";

            ChartSeries cycleTimeSeries;
            cycleTimeSeries.Name = "Cycle Time";

            // Find the best result.
            double betterCycleTime = 0.0;

            // Results.
            double resultKappa = 0.0;
            double resultK_kappa = 0.0;
            double resultE_kappa = 0.0;
            double resultWaveLength = 0.0;

            // Calculate K(κ) and E(κ).
            for(double kappa = ErgodicAccuracy; kappa < 1; kappa += ErgodicAccuracy)
            {                
                // Calculate K(κ) and E(κ).
                double K_kappa = 0.0;
                double E_kappa = 0.0;
                EllipticIntegralCalc(kappa, K_kappa, E_kappa);
                K_kappaSeries.Points.push_back(Points(kappa, K_kappa));
                E_kappaSeries.Points.push_back(Points(kappa, E_kappa));

                // Calculate cycleTime.
                double cycleTime = 0.0;
                /**
                 * 
                 */
                cycleTime = (4 * Depth / sqrt(3 * 9.8 * WaveHeight)) * ((kappa * K_kappa) / (sqrt(1 + (WaveHeight / Depth) * (1 / pow(kappa, 2) - 0.5 - ((3 * E_kappa) / (2 * pow(kappa, 2) * K_kappa))))));
                cycleTimeSeries.Points.push_back(Points(kappa, cycleTime));

                // Find the best kappa.
                if (fabs(cycleTime - Period) < fabs(betterCycleTime - Period))
                {
                    resultKappa = kappa;
                    resultK_kappa = K_kappa;
                    resultE_kappa = E_kappa;
                    betterCycleTime = cycleTime;
                }
            }

            // Calculate wavelength.
            resultWaveLength = sqrt((16 * pow(Depth, 3) ) / (3 * WaveHeight)) * resultKappa * resultK_kappa;

            // Output data.
            SetEllipticCosineWaveErgodicCalculatorKappaResult(resultKappa);
            SetEllipticCosineWaveErgodicCalculatorWaveLengthResult(resultWaveLength);

            ClearChart();
            SetChartTitle("椭圆余弦波波长遍历计算");
            AddSeriesToChart(K_kappaSeries);
            AddSeriesToChart(E_kappaSeries);
            AddSeriesToChart(cycleTimeSeries);

            return 0;
        }, period, depth, waveHeight, ergodicAccuracy
    ).detach();
}

