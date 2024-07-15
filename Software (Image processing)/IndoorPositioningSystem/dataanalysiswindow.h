#ifndef DATAANALYSISWINDOW_H
#define DATAANALYSISWINDOW_H

/**************************************** Data Analysis Window ******************************************************
 * This window is responsible for Data Analysis. It helps to adjust the discrepancy in UWB distances.
 * This class communicates with Data Processor though the ViewModel.
 * After distances are corrected, this window allows to export data segments for easier model training and evaluation
 *  of Pixel-to-Real and Optical methods.
*********************************************************************************************************************/

#include <QDialog>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QComboBox>
#include <QPushButton>
#include <QTimeEdit>
#include <QDateTimeAxis>
#include <QtWidgets/QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QScrollBar>
#include <vector>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "customchartview.h"
#include "indoorpositioningsystemviewmodel.h"

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
    explicit DataAnalysisWindow(QWidget *parent = nullptr, IndoorPositioningSystemViewModel* viewModel = nullptr);
    ~DataAnalysisWindow();

public slots:
    void initDataAnalysis();
    void startDataAnalysis();
    void showAvailableTags(const std::vector<int>& availableTagIDs);
    void showAvailableAnchors(const std::vector<int>& availableAnchorIDs);
    void showPlotDistancesVsTimestamps(const std::vector<long long>& timestamps, std::vector<double*> distances);
    void showPlotRollingDeviations(const std::vector<long long>& timestamps, const std::vector<double>& deviations);
    void validateRollingDeviationInput();
    void validateThresholdInput();
    void showDatasetSegments(const std::vector<double>& datasetSegmentMeans);
    void showOriginalVsAdjustedDistances(const std::vector<long long>& timestampsToAnalyze, std::vector<double*> distancesToAnalyzeOriginal, const std::vector<double>& distancesToAnalyzeAdjusted);
    void uploadReferenceValues();

signals:
    void requestAnalyseData(const long long startTimeSec, const long long endTimeSec);
    void requestCollectDataForPlotDistancesVsTimestamps(const int anchorID);
    void requestCalculateRollingDeviation(const int windowSize);
    void requestSplitDataset(const double threshold);
    void requestCalculatePolynomialRegression(const std::vector<double>& referenceValues);
    void requestUpdateOriginalWithAdjustedValues();
    void requestSegmentFramesExport();

private:
    Ui::DataAnalysisWindow *ui;
    IndoorPositioningSystemViewModel* viewModel; // Communication with DataProcessor through the ViewModel, not directly

    int sizeOfProcessingData;
    qreal maxStdDeviation;
    qreal minStdDeviation;
    std::vector<double> referenceValues;

    // Layouts
    QVBoxLayout* mainLayout;
    QVBoxLayout* chartsLayout;
    QHBoxLayout *tagsAndAnchorsListsLayout;

    QTimeEdit *startAnalysisTime, *endAnalysisTime;
    QPushButton *setAnalysisTimeRangeButton;

    QScrollArea *scrollArea;
    QWidget* scrollAreaWidget;

    QPushButton *analyzeDataButton;

    QComboBox* comboBoxAvailableTags;
    QComboBox* comboBoxAvailableAnchors;

    QChart *chartDistancesVsTimestamps;
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

    std::vector<double> segmentMeans;
    QVBoxLayout* segmentMeansLayout;
    QVector<QLabel*> segmentMeansLabels;

    QVector<QPushButton*> adjustReferenceValueButtons;
    QPushButton* uploadReferenceValuesButton;
    QPushButton* calculatePolynomialRegressionButton;

    QChart *chartOriginalVsAdjustedDistances;
    CustomChartView *chartViewOriginalVsAdjustedDistances;

    QPushButton* updateOriginalWithAdjustedValuesButton;
    QPushButton* exportSegementFramesForModelButton;

    void rollingDeviationInit();
    void initThresholdSetting();
};

#endif // DATAANALYSISWINDOW_H
