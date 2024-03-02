#ifndef DATAANALYSISWINDOW_H
#define DATAANALYSISWINDOW_H

#include <QDialog>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
// #include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QRandomGenerator>
#include <QComboBox>
#include <QPushButton>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QDateTimeAxis>
#include <QTimeEdit>
#include <QtWidgets/QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

#include "dataprocessor.h"
#include "chartview.h"
#include <optional>
#include <vector>
#include <QGridLayout>

namespace Ui {
class DataAnalysisWindow;
}


enum InputType {
    RollingDeviation,
    Threshold
};

class DataAnalysisWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DataAnalysisWindow(QWidget *parent = nullptr, DataProcessor* dataProcessor = nullptr, double fps = 0.0);
    ~DataAnalysisWindow();


public slots:
    void showAvailableTags(const std::vector<int>& availableTagIDs);
    void showAvailableAnchors(const std::vector<int>& availableAnchorIDs);
    void showPlotDistancesVsTimestamps(const std::vector<long long>& timestamps, const std::vector<double>& distances);
    void initDataAnalysis();
    void plotManager();
    void showPlotRollingDeviations(const std::vector<long long>& timestamps, const std::vector<double>& deviations);
    void validateRollingDeviationInput();
    void validateThresholdInput();
    void showDatasetSegments(const std::vector<double>& datasetSegmentMeans);

signals:
    void requestAnalyseData(const long long startFrameIndex, const long long endFrameIndex);
    void requestCollectDataForTag(int tagID);
    void requestCollectDataForAnchor(const int anchorID);
    void requestCollectDataForPlotDistancesVsTimestamps(const int anchorID);
    void requestCalculateRollingDeviation(const int windowSize);
    void requestSplitDataset(const double threshold);


    // void requestPlotDistancesVsTimestamps();

private:
    Ui::DataAnalysisWindow *ui;
    DataProcessor* dataProcessor;
    double fps;
    int sizeOfProcessingData;
    int rollingDeviationWindowSize;
    qreal maxStdDeviation;
    std::vector<double> referenceValues;

    QTimeEdit *startAnalysisTime, *endAnalysisTime;
    QPushButton *setAnalysisTimeRangeButton;

    QVBoxLayout* mainLayout;
    QVBoxLayout* chartsLayout;
    QHBoxLayout *tagsAndAnchorsListsLayout;

    QScrollArea *scrollArea;
    QWidget* scrollAreaWidget;

    QPushButton *analyzeDataButton;
    // QPushButton *addGroundTruthValuesButton;

    QComboBox* comboBoxAvailableTags;
    QComboBox* comboBoxAvailableAnchors;

    QChart *chartDistancesVsTimestamps;
    ChartView *chartViewDistancesVsTimestamps;

    QChart *chartRollingDeviations;
    ChartView *chartViewRollingDeviations;
    QHBoxLayout *rollingDeviationInputLayout;
    QLabel *rollingDeviationInputLabel;
    QLineEdit *rollingDeviationInputText;
    QPushButton *calculateRollingDeviationButton;

    ChartView *chartViewThresholdInput;
    QHBoxLayout *thresholdInputLayout;
    QLabel *thresholdInputLabel;
    QLineEdit *thresholdInputText;
    QPushButton *thresholdInputButton;

    QVBoxLayout* segmentMeansLayout;
    std::vector<QLabel> segmentMeansLabels;

    void rollingDeviationInit();
    void initThresholdSetting();


};

#endif // DATAANALYSISWINDOW_H
