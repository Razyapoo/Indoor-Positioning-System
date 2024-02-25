#ifndef DATAANALYSISWINDOW_H
#define DATAANALYSISWINDOW_H

#include <QDialog>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QRandomGenerator>
#include <QComboBox>

#include "dataprocessor.h"
#include <optional>
#include <vector>
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
    void showAvailableTags(const std::vector<int>& availableTagIDs);
    void showPlotDistancesVsTimestamps(const std::vector<UWBData>& uwbData);

signals:
    void requestAnalyzeDataForTag(const int tagID);

private:
    Ui::DataAnalysisWindow *ui;
    DataProcessor* dataProcessor;
    QVBoxLayout* mainLayout;
    // std::unique_ptr<QScatterSeries> series;
    QComboBox* comboBoxAvailableTags;
    QComboBox* comboBoxAvailableAnchors;

    QChartView* createChartView();



};

#endif // DATAANALYSISWINDOW_H
