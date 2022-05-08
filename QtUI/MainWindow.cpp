#include "MainWindow.hpp"

#define _USE_MATH_DEFINES

#include <thread>
#include <math.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

// Dispersion Equation Wavelength Calculator.
#include "DispersionEquationWavelengthCalculator.hpp"

// Elliptic Cosine Wave Wavelength Calculator.
#include "EllipticIntegral.hpp"
//#include "EllipticCosineWaveWavelengthCalculator.hpp"
#include "WaveRefractionCalc.h"

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
    // log.
    connect(this, &MainWindow::log, this, &MainWindow::logAppendMethod, Qt::AutoConnection);
    
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

            if (Series.Name.size() > 0)
            {
                series->setName(QString::fromStdString(Series.Name));
            }

            if (Series.Color != nullptr)
            {
                series->setColor(*(Series.Color));
            }
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
    connect(ui.WR_Calc, &QPushButton::clicked, this, &MainWindow::runWaveRefractionCalculator);
    
    // Output author information.
    log(Info, "Author: h13.");
    log(Info, "Student number: 631902090521.");
    log(Info, R"(Project repository: https://github.com/h13-0/CoastalHydrodynamicsCalculator)");

    // Create a new thread to continue initialization.
    log(Info, "Initing Wave Refraction Calculator ...");
    std::thread(
        [this](void) -> void
        {
            // Init wave refraction calculator.
            bool ret = WaveRefractionCalcInitialize();
            if (ret)
            {
                log(Info, "WaveRefractionCalcInitialize() successed.");
            }
            else {
                log(Fatal, "WaveRefractionCalcInitialize() failed.");
            }
        }
    ).detach();
}

MainWindow::~MainWindow()
{

}

bool MainWindow::tryParseDouble(QLineEdit* LineEdit, double &Value)
{
    using namespace std;
    try {
        std::string str = LineEdit->text().toStdString();
        Value = stod(str);
        return true;
    }
    catch (...)
    {
        log(Warning, "Unable to convert the current parameter to double, please check the parameter!");
        return false;
    }
}

bool MainWindow::tryParseInt(QLineEdit* LineEdit, int& Value)
{
    using namespace std;
    try {
        std::string str = LineEdit->text().toStdString();
        Value = stoi(str);
        return true;
    }
    catch (...)
    {
        log(Warning, "Unable to convert the current parameter to int, please check the parameter!");
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
            log(Info, "Dispersion Equation Calculator begins to calculate.");
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

            log(Info, "Dispersion Equation Calculator starts drawing.");

            // Output data.
            SetDispersionEquationCalculatorResult(waveLength);
            ClearChart();
            SetChartTitle("弥散方程求波长");
            AddSeriesToChart(series);

            log(Info, "Dispersion Equation Calculator ends the calculation.");

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
            log(Info, "Elliptic Cosine Wave Ergodic Calculator begins to calculate.");
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

            log(Info, "Elliptic Cosine Wave Ergodic Calculator starts drawing.");

            // Output data.
            SetEllipticCosineWaveErgodicCalculatorKappaResult(resultKappa);
            SetEllipticCosineWaveErgodicCalculatorWaveLengthResult(resultWaveLength);

            ClearChart();
            SetChartTitle("椭圆余弦波波长遍历计算");
            AddSeriesToChart(K_kappaSeries);
            AddSeriesToChart(E_kappaSeries);
            AddSeriesToChart(cycleTimeSeries);

            log(Info, "Elliptic Cosine Wave Ergodic Calculator ends the calculation.");

            return 0;
        }, period, depth, waveHeight, ergodicAccuracy
    ).detach();
}

void h13::MainWindow::runWaveRefractionCalculator()
{
    // Get Values from QTextEdit.
    // incidentAngle
    double incidentAngle = 0.0;
    if (!tryParseDouble(ui.WR_IncidentAngle, incidentAngle))
    {
        return;
    }

    // depth.
    double depth = 0.0;
    if (!tryParseDouble(ui.WR_Depth, depth))
    {
        return;
    }

    // radius.
    double radius = 0.0;
    if (!tryParseDouble(ui.WR_Radius, radius))
    {
        return;
    }

    // tolerance.
    double tolerance = 0.0;
    if (!tryParseDouble(ui.WR_Tolerance, tolerance))
    {
        return;
    }

    // period.
    double period = 0.0;
    if (!tryParseDouble(ui.WR_Period, period))
    {
        return;
    }

    // Create calculate thread.
    std::thread(
        [this](double IncidentAngle, double Depth, double Radius, double Tolerance, double Period) -> int
        {
            log(Info, "Wave Refraction Calculator begins to calculate.");
            mxArray* incidentAngle = mxCreateDoubleScalar(IncidentAngle);
            mxArray* depth         = mxCreateDoubleScalar(Depth);
            mxArray* radius        = mxCreateDoubleScalar(Radius);
            mxArray* tolerance     = mxCreateDoubleScalar(Tolerance);
            mxArray* period     = mxCreateDoubleScalar(Period);
            mxArray* params[] = { incidentAngle, depth, radius, tolerance, period };
            
            mxArray* boundary_x     = mxCreateDoubleScalar(0);
            mxArray* boundary_y     = mxCreateDoubleScalar(0);
            mxArray* boundary_angle = mxCreateDoubleScalar(0);
            mxArray* internal_x     = mxCreateDoubleScalar(0);
            mxArray* internal_y     = mxCreateDoubleScalar(0);
            mxArray* internal_angle = mxCreateDoubleScalar(0);
            mxArray* results[] = { boundary_x, boundary_y, boundary_angle, 
                internal_x, internal_y, internal_angle };

            bool ret = mlxWaveRefractionCalc(6, results, 5, params);

            log(Info, "Wave Refraction Calculator starts drawing.");

            if (ret)
            {
                int boundaryPoints = mxGetN(results[0]);
                int internalPoints = mxGetN(results[3]);

                double* boundaryX_ptr = mxGetPr(results[0]);
                double* boundaryY_ptr = mxGetPr(results[1]);
                double* boundaryAngle_ptr = mxGetPr(results[2]);
                
                double* internalX_ptr = mxGetPr(results[3]);
                double* internalY_ptr = mxGetPr(results[4]);
                double* internalAngle_ptr = mxGetPr(results[5]);

                ClearChart();
                SetChartTitle("圆锥底坡波浪折射计算");

                // Add boundary points.
                for (int i = 0; i < boundaryPoints; i++)
                {
                    ChartSeries boundarySeries;
                    boundarySeries.Points.push_back(Points(*(boundaryX_ptr + i), *(boundaryY_ptr + i)));
                    boundarySeries.Points.push_back(Points(*(boundaryX_ptr + i) + sin(*(boundaryAngle_ptr + i)) * Radius * 0.15, *(boundaryY_ptr + i) + cos(*(boundaryAngle_ptr + i)) * Radius * 0.15));
                    QColor* boundaryColor = new QColor(255, 0, 0);
                    boundarySeries.Color = boundaryColor;
                    AddSeriesToChart(boundarySeries);
                }

                // Add internal points.
                for (int i = 0; i < internalPoints; i++)
                {
                    ChartSeries internalSeries;
                    internalSeries.Points.push_back(Points(*(internalX_ptr + i), *(internalY_ptr + i)));
                    internalSeries.Points.push_back(Points(*(internalX_ptr + i) + sin(*(internalAngle_ptr + i)) * Radius * 0.15, *(internalY_ptr + i) + cos(*(internalAngle_ptr + i)) * Radius * 0.15));
                    QColor* internalColor = new QColor(0, 0, 255);
                    internalSeries.Color = internalColor;
                    AddSeriesToChart(internalSeries);
                }
            }
            else {
                log(Info, "Wave Refraction Calculator: The current parameter has no solution.");
            }

            // Release variables.
            for (auto result : results)
            {
                mxDestroyArray(result);
            }
            for (auto param : params)
            {
                mxDestroyArray(param);
            }

            log(Info, "Wave Refraction Calculator ends the calculation.");

            return 0;
        }, incidentAngle, depth, radius, tolerance, period
    ).detach();
}

void h13::MainWindow::logAppendMethod(logLevel level, std::string log)
{
    auto timeStamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream stringStream;
    stringStream << std::put_time(std::localtime(&timeStamp), "[%H:%M:%S]");
    QString Qlog;

    switch (level)
    {
    case logLevel::Fatal:
        Qlog = QString("<font color=\"#990000\">%1").arg(QString::fromStdString(stringStream.str() + ": " + "[Fatal]: " + log));
        break;
    case logLevel::Error:
        Qlog = QString("<font color=\"#ff3300\">%1").arg(QString::fromStdString(stringStream.str() + ": " + "[Error]: " + log));
        break;
    case logLevel::Warning:
        Qlog = QString("<font color=\"#FF0000\">%1").arg(QString::fromStdString(stringStream.str() + ": " + "[Warning]: " + log));
        break;
    case logLevel::Info:
        Qlog = QString("<font color=\"#000000\">%1").arg(QString::fromStdString(stringStream.str() + ": " + "[Info]: " + log));
        break;
    case logLevel::Debug:
        Qlog = QString("<font color=\"#000000\">%1").arg(QString::fromStdString(stringStream.str() + ": " + "[Debug]: " + log));
        break;
    default:
        break;
    }

    if (!Qlog.isEmpty()) {
        ui.log->append(Qlog);
    }
}

