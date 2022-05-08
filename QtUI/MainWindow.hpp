#pragma once

#include "QtWidgets/QMainWindow.h"
#include "QtWidgets/QLineEdit.h"
#include "QLineSeries"
#include "ui_CalculatorWindow.h"
#include <functional>
#include <vector>

namespace h13
{
    /// <summary>
    /// @brief: typedef struct of series.
    /// @param
    /// </summary>
    typedef struct Points
    {
        Points() {};
        Points(double X, double Y)
            : X(X), Y(Y) {};
        double X;
        double Y;
    } Points_t;

    typedef struct ChartSeries
    {
        ChartSeries() {};
        ChartSeries(std::string Name, std::vector<Points_t> Points)
            :Name(Name), Points(Points){ };
        std::string Name;
        std::vector<Points_t> Points;
    } ChartSeries_t;

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = Q_NULLPTR);
        MainWindow(const MainWindow&) = delete;
        ~MainWindow();

    signals:
        /// <summary>
        /// 
        /// </summary>
        void SetChartTitle(std::string Title);
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="Series"></param>
        void AddSeriesToChart(ChartSeries_t& Series);

        /// <summary>
        /// 
        /// </summary>
        /// <param name=""></param>
        void ClearChart(void);



        /// <summary>
        /// 
        /// </summary>
        /// <param name="Result"></param>
        void SetDispersionEquationCalculatorResult(double Result);

        void SetEllipticCosineWaveErgodicCalculatorKappaResult(double Result);

        void SetEllipticCosineWaveErgodicCalculatorWaveLengthResult(double Result);

    private:
        bool tryParseDouble(QLineEdit* LineEdit, double &Value);
        bool tryParseInt(QLineEdit* LineEdit, int &Value);

        Ui::CalculatorWindow ui;
        QChart chart;
        std::vector<QLineSeries*> seriesList;
        /* data */

    private slots:
        //void praseDouble(QLineEdit* LineEdit, double& Value);

        /// <summary>
        /// Run the dispersion equation calculator.
        /// </summary>
        void runDispersionEquationCalculator();

        /// <summary>
        /// Run the Elliptic cosine wave ergodic calculator.
        /// </summary>
        void runEllipticCosineWaveErgodicCalculator();
        
        /// <summary>
        /// Run the wave refraction calculator.
        /// </summary>
        void runWaveRefractionCalculator();

    };
} // namespace h13
