#ifndef DATAANALYSISWINDOW_H
#define DATAANALYSISWINDOW_H

#include <QDialog>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QRandomGenerator>

#include "dataprocessor.h"
#include <optional>
#include <QGridLayout>

namespace Ui {
class DataAnalysisWindow;
}

class DataAnalysisWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DataAnalysisWindow(QWidget *parent = nullptr, DataProcessor* dataProcessor = nullptr);
    ~DataAnalysisWindow();


public slots:
    void showPlot();

private:
    Ui::DataAnalysisWindow *ui;
    DataProcessor* dataProcessor;
    QVBoxLayout* layout;
    // std::unique_ptr<QScatterSeries> series;

    QChartView* createChartView();



};

#endif // DATAANALYSISWINDOW_H
