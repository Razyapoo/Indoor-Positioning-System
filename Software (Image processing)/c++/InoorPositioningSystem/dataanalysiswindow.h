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
#include <QScrollBar>
#include <QInputDialog>
#include <QThread>

#include "dataprocessor.h"
#include "customchartview.h"
#include "custominputdialog.h"
#include <optional>
#include <vector>
#include <QGridLayout>

namespace Ui {
class DataAnalysisWindow;
}


enum WidgetType {
    DistancesVsTimestamps,
    RollingDeviation,
    datasetSegments
};

class DataAnalysisWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DataAnalysisWindow(QWidget *parent = nullptr, DataProcessor* dataProcessor = nullptr, double fps = 0.0);
    ~DataAnalysisWindow();

public slots:
    // void onDialogClosed();

    void showAvailableTags(const std::vector<int>& availableTagIDs);
    void showAvailableAnchors(const std::vector<int>& availableAnchorIDs);
    void showPlotDistancesVsTimestamps(const std::vector<long long>& timestamps, std::vector<double*> distances);
    void initDataAnalysis();
    void startDataAnalysis();
    void showPlotRollingDeviations(const std::vector<long long>& timestamps, const std::vector<double>& deviations);
    void validateRollingDeviationInput();
    void validateThresholdInput();
    void showDatasetSegments(const std::vector<double>& datasetSegmentMeans);
    void showOriginalVsAdjustedDistances(const std::vector<long long>& timestampsToAnalyze, std::vector<double*> distancesToAnalyzeOriginal, const std::vector<double>& distancesToAnalyzeAdjusted);
    // void attachReferenceValues();
    // void calculateLinearRegression();

signals:
    void requestAnalyseData(const long long startFrameIndex, const long long endFrameIndex);
    void requestCollectDataForTag(int tagID);
    void requestCollectDataForAnchor(const int anchorID);
    void requestCollectDataForPlotDistancesVsTimestamps(const int anchorID);
    void requestCalculateRollingDeviation(const int windowSize);
    void requestSplitDataset(const double threshold);
    void requestCalculatePolynomialRegression(const std::vector<double>& referenceValues);
    void requestUpdateOriginalWithAdjustedValues();
    void requestSegmentFramesExport();

    // void requestCalculateLinearRegression();



    // void requestPlotDistancesVsTimestamps();

private:
    Ui::DataAnalysisWindow *ui;
    DataProcessor* dataProcessor;
    QThread* dataAnalysisWindowThread;

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
    // QScrollBar *scrollBar;

    QPushButton *analyzeDataButton;
    // QPushButton *addGroundTruthValuesButton;

    QComboBox* comboBoxAvailableTags;
    QComboBox* comboBoxAvailableAnchors;

    // QLineSeries *seriesDistancesVsTimestamps;
    QChart *chartDistancesVsTimestamps;
    // QLineSeries *seriesRollingDeviations;
    CustomChartView *chartViewDistancesVsTimestamps;


    QChart *chartRollingDeviations;
    CustomChartView *chartViewRollingDeviations;
    QHBoxLayout *rollingDeviationInputLayout;
    QLabel *rollingDeviationInputLabel;
    QLineEdit *rollingDeviationInputText;
    QPushButton *calculateRollingDeviationButton;

    CustomChartView *chartViewThresholdInput;
    QHBoxLayout *thresholdInputLayout;
    QLabel *thresholdInputLabel;
    QLineEdit *thresholdInputText;
    QPushButton *thresholdInputButton;

    QVBoxLayout* segmentMeansLayout;
    std::vector<QLabel*> segmentMeansLabels;

    QPushButton* calculatePolynomialRegressionButton;

    QChart *chartOriginalVsAdjustedDistances;
    CustomChartView *chartViewOriginalVsAdjustedDistances;

    QPushButton* updateOriginalWithAdjustedValuesButton;
    QPushButton* exportSegementFramesForModelButton;

    void rollingDeviationInit();
    void initThresholdSetting();

    // void chartCleanup();

// protected:
//     void closeEvent(QCloseEvent *event) override;
};

#endif // DATAANALYSISWINDOW_H
