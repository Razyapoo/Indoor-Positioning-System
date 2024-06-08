#include "dataanalysiswindow.h"
#include "ui_dataanalysiswindow.h"

DataAnalysisWindow::DataAnalysisWindow(QWidget *parent, IndoorPositioningSystemViewModel *viewModel)
    : QDialog(parent), ui(new Ui::DataAnalysisWindow), viewModel(viewModel)
{
    ui->setupUi(this);

    // layout = new QVBoxLayout(this);

    setWindowTitle("Data Analysis");
    // setAttribute(Qt::WA_DeleteOnClose);

    // dataAnalysisWindowThread = new QThread(this);
    // dataAnalysisWindowThread->start();

    mainLayout = new QVBoxLayout(this);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    chartsLayout = new QVBoxLayout();
    // scrollArea->setLayout(chartsLayout);

    int leftMargin = 25;
    int rightMargin = 25;
    int topMargin = 10;
    int bottomMargin = 10;
    chartsLayout->setContentsMargins(leftMargin, topMargin, rightMargin, bottomMargin);

    scrollAreaWidget = new QWidget(this);
    scrollAreaWidget->setLayout(chartsLayout);
    scrollArea->setWidget(scrollAreaWidget);
    mainLayout->addWidget(scrollArea);

    tagsAndAnchorsListsLayout = new QHBoxLayout();
    rollingDeviationInputLayout = new QHBoxLayout();
    thresholdInputLayout = new QHBoxLayout();
    segmentMeansLayout = new QVBoxLayout();


    analyzeDataButton = new QPushButton("Analyze Data", this);
    // addGroundTruthValuesButton = new QPushButton("Add Ground Truth Values");
    comboBoxAvailableTags = new QComboBox(this);
    comboBoxAvailableAnchors = new QComboBox(this);

    startAnalysisTime = new QTimeEdit(this);
    startAnalysisTime->setDisplayFormat("HH:mm:ss");
    endAnalysisTime = new QTimeEdit(this);
    endAnalysisTime->setDisplayFormat("HH:mm:ss");
    setAnalysisTimeRangeButton = new QPushButton("Set Time Range", this);
    QHBoxLayout *timeRangeLayout = new QHBoxLayout();
    timeRangeLayout->addWidget(startAnalysisTime);
    timeRangeLayout->addWidget(endAnalysisTime);
    timeRangeLayout->addWidget(setAnalysisTimeRangeButton);
    mainLayout->addLayout(timeRangeLayout);

    tagsAndAnchorsListsLayout->addWidget(comboBoxAvailableTags);
    tagsAndAnchorsListsLayout->addWidget(comboBoxAvailableAnchors);
    tagsAndAnchorsListsLayout->addWidget(analyzeDataButton);
    mainLayout->addLayout(tagsAndAnchorsListsLayout);

    chartViewDistancesVsTimestamps = nullptr;
    chartViewRollingDeviations = nullptr;
    chartViewThresholdInput = nullptr;
    chartViewOriginalVsAdjustedDistances = nullptr;
    calculatePolynomialRegressionButton = nullptr;
    updateOriginalWithAdjustedValuesButton = nullptr;
    exportSegementFramesForModelButton = nullptr;
    chartDistancesVsTimestamps = nullptr;
    chartRollingDeviations = nullptr;

    connect(viewModel, &IndoorPositioningSystemViewModel::requestShowAvailableTags, this, &DataAnalysisWindow::showAvailableTags);
    connect(viewModel, &IndoorPositioningSystemViewModel::requestShowAvailableAnchors, this, &DataAnalysisWindow::showAvailableAnchors);
    connect(viewModel, &IndoorPositioningSystemViewModel::requestShowPlotDistancesVsTimestamps, this, &DataAnalysisWindow::showPlotDistancesVsTimestamps);
    connect(viewModel, &IndoorPositioningSystemViewModel::requestShowPlotRollingDeviations, this, &DataAnalysisWindow::showPlotRollingDeviations);
    connect(viewModel, &IndoorPositioningSystemViewModel::requestShowDatasetSegments, this, &DataAnalysisWindow::showDatasetSegments);
    connect(viewModel, &IndoorPositioningSystemViewModel::requestShowOriginalVsAdjustedDistances, this, &DataAnalysisWindow::showOriginalVsAdjustedDistances);

    connect(comboBoxAvailableTags, &QComboBox::currentTextChanged, viewModel, &IndoorPositioningSystemViewModel::collectDataForTag);
    connect(setAnalysisTimeRangeButton, &QPushButton::clicked, this, &DataAnalysisWindow::initDataAnalysis);
    connect(analyzeDataButton, &QPushButton::clicked, this, &DataAnalysisWindow::startDataAnalysis);

    connect(this, &DataAnalysisWindow::requestAnalyseData, viewModel, &IndoorPositioningSystemViewModel::setRangeForDataAnalysis);
    connect(this, &DataAnalysisWindow::requestCollectDataForPlotDistancesVsTimestamps, viewModel, &IndoorPositioningSystemViewModel::collectDataForPlotDistancesVsTimestamps);
    connect(this, &DataAnalysisWindow::requestCalculateRollingDeviation, viewModel, &IndoorPositioningSystemViewModel::calculateRollingDeviation);
    connect(this, &DataAnalysisWindow::requestSplitDataset, viewModel, &IndoorPositioningSystemViewModel::splitDataset);
    connect(this, &DataAnalysisWindow::requestCalculatePolynomialRegression, viewModel, &IndoorPositioningSystemViewModel::calculatePolynomialRegression);
    connect(this, &DataAnalysisWindow::requestUpdateOriginalWithAdjustedValues, viewModel, &IndoorPositioningSystemViewModel::updateOriginalWithAdjustedValues);
    connect(this, &DataAnalysisWindow::requestSegmentFramesExport, viewModel, &IndoorPositioningSystemViewModel::onSegmentFramesExport);
}

DataAnalysisWindow::~DataAnalysisWindow()
{
    delete ui;
    if (chartDistancesVsTimestamps != nullptr) {
        delete chartDistancesVsTimestamps;
    }
    if (chartRollingDeviations != nullptr) {
        delete chartRollingDeviations;
    }
    if (tagsAndAnchorsListsLayout != nullptr) {
        delete tagsAndAnchorsListsLayout;
    }
    if (rollingDeviationInputLayout != nullptr) {
        delete rollingDeviationInputLayout;
    }
    if (thresholdInputLayout != nullptr) {
        delete thresholdInputLayout;
    }
    if (segmentMeansLayout != nullptr) {
        delete segmentMeansLayout;
    }

}


void DataAnalysisWindow::initDataAnalysis()
{
    QTime startTime = startAnalysisTime->time();
    QTime endTime = endAnalysisTime->time();
    long long startTimeSec = (startTime.hour() * 3600 + startTime.minute() * 60 + startTime.second());
    long long endTimeSec = (endTime.hour() * 3600 + endTime.minute() * 60 + endTime.second());
    emit requestAnalyseData(startTimeSec, endTimeSec);
}

// Data are collected for tag every time it is selected in combox via connect
void DataAnalysisWindow::showAvailableTags(const std::vector<int> &availableTagIDs)
{

    if (comboBoxAvailableTags->count())
    {
        for (int i = 0; i < comboBoxAvailableTags->count(); ++i)
        {
            comboBoxAvailableTags->removeItem(i);
        }
    }

    for (const int &tagID : availableTagIDs)
    {
        comboBoxAvailableTags->addItem(QString::number(tagID));
    }
}

// This only shows available anchors for the given tag. Does not presellected anchor data before analysis starts. Anchor data is selected at the time "Analyze" button is clicked
void DataAnalysisWindow::showAvailableAnchors(const std::vector<int> &availableAnchorIDs)
{

    if (comboBoxAvailableAnchors->count())
    {
        for (int i = 0; i < comboBoxAvailableAnchors->count(); ++i)
        {
            comboBoxAvailableAnchors->removeItem(i);
        }
    }

    for (const int &anchorID : availableAnchorIDs)
    {
        comboBoxAvailableAnchors->addItem(QString::number(anchorID));
    }
}

void DataAnalysisWindow::startDataAnalysis()
{
    int currentAnchorID = comboBoxAvailableAnchors->currentText().toInt();
    emit requestCollectDataForPlotDistancesVsTimestamps(currentAnchorID);
}

void DataAnalysisWindow::rollingDeviationInit()
{

    if (!rollingDeviationInputLayout->count()) {
        rollingDeviationInputLabel = new QLabel(this);
        rollingDeviationInputText = new QLineEdit(this);
        calculateRollingDeviationButton = new QPushButton("Calculate", this);

        rollingDeviationInputLabel->setText("Please input window size: ");
        rollingDeviationInputLabel->setFixedWidth(180);
        rollingDeviationInputLayout->addWidget(rollingDeviationInputLabel);
        rollingDeviationInputLayout->setAlignment(rollingDeviationInputLabel, Qt::AlignLeft);


        rollingDeviationInputText->setFixedWidth(100);
        rollingDeviationInputLayout->addWidget(rollingDeviationInputText);
        rollingDeviationInputLayout->setAlignment(rollingDeviationInputText, Qt::AlignLeft);


        calculateRollingDeviationButton->setFixedWidth(100);
        rollingDeviationInputLayout->addWidget(calculateRollingDeviationButton);
        rollingDeviationInputLayout->setAlignment(calculateRollingDeviationButton, Qt::AlignLeft);
        rollingDeviationInputLayout->addStretch(0);
        connect(calculateRollingDeviationButton, &QPushButton::clicked, this, &DataAnalysisWindow::validateRollingDeviationInput);//, Qt::DirectConnection);
    }

    rollingDeviationInputText->setText( "");

    // rollingDeviationInputLayout->addSpacerItem(spacer);
    chartsLayout->addLayout(rollingDeviationInputLayout);

}

void DataAnalysisWindow::validateRollingDeviationInput()
{
    QString inputText = rollingDeviationInputText->text();

    bool ok;
    int inputValue = inputText.toInt(&ok);

    if (!ok)
    {
        QMessageBox::critical(nullptr, "Error", "Invalid input: not an integer");
    }

    if (inputValue < 0 || inputValue > sizeOfProcessingData)
    {
        QMessageBox::critical(nullptr, "Error", "Invalid input: window size is out of dataset size");
    }
    else
    {
        emit requestCalculateRollingDeviation(inputValue);
    }
}

void DataAnalysisWindow::validateThresholdInput()
{

    QString inputText = thresholdInputText->text();

    bool ok;
    double inputValue = inputText.toDouble(&ok);

    if (!ok)
    {
        QMessageBox::critical(nullptr, "Error", "Invalid input: not an double");
    }

    if (inputValue < 0 || inputValue > maxStdDeviation)
    {
        QMessageBox::critical(nullptr, "Error", "Invalid input: window size is out of dataset size");
    }
    else
    {
        emit requestSplitDataset(inputValue);
    }
}

void DataAnalysisWindow::initThresholdSetting()
{

    if (!thresholdInputLayout->count()) {
        thresholdInputLabel = new QLabel(this);
        thresholdInputLabel->setText("Please input threshold value: ");
        thresholdInputLabel->setFixedWidth(210);
        thresholdInputLayout->addWidget(thresholdInputLabel);
        thresholdInputLayout->setAlignment(thresholdInputLabel, Qt::AlignLeft);

        thresholdInputText = new QLineEdit(this);
        thresholdInputText->setFixedWidth(100);
        thresholdInputLayout->addWidget(thresholdInputText);
        thresholdInputLayout->setAlignment(thresholdInputText, Qt::AlignLeft);

        thresholdInputButton = new QPushButton("Calculate", this);
        thresholdInputButton->setFixedWidth(100);
        thresholdInputLayout->addWidget(thresholdInputButton);
        thresholdInputLayout->setAlignment(thresholdInputButton, Qt::AlignLeft);
        thresholdInputLayout->addStretch(0);
        connect(thresholdInputButton, &QPushButton::clicked, this, &DataAnalysisWindow::validateThresholdInput);//, Qt::DirectConnection);
    }

    thresholdInputText->setText("");
    chartsLayout->addLayout(thresholdInputLayout);

}

void DataAnalysisWindow::showPlotDistancesVsTimestamps(const std::vector<long long> &timestamps, std::vector<double*> distances)
{

    int i = 0;
    while (i < chartsLayout->count()) {
        QLayoutItem* item = chartsLayout->itemAt(i);
        QWidget* widget = item->widget();
        QLayout* itemLayout = item->layout();

        if (widget) {
            if (widget == chartViewDistancesVsTimestamps) {
                chartsLayout->removeWidget(widget);
                delete widget;
                chartViewDistancesVsTimestamps = nullptr;
                continue;
            } else if (widget == chartViewRollingDeviations) {
                chartsLayout->removeWidget(widget);
                delete widget;
                chartViewRollingDeviations = nullptr;
                continue;
            } else if (widget == chartViewOriginalVsAdjustedDistances) {
                chartsLayout->removeWidget(widget);
                delete widget;
                chartViewOriginalVsAdjustedDistances = nullptr;
                continue;
            } else if (widget == calculatePolynomialRegressionButton) {
                chartsLayout->removeWidget(widget);
                continue;
            } else if (widget == updateOriginalWithAdjustedValuesButton) {
                chartsLayout->removeWidget(widget);
                continue;
            } else if (widget == exportSegementFramesForModelButton) {
                chartsLayout->removeWidget(widget);
                continue;
            }
        }

        if (itemLayout) {
            if (itemLayout == rollingDeviationInputLayout || itemLayout == thresholdInputLayout || itemLayout == segmentMeansLayout) {
                chartsLayout->removeItem(item);
                continue;
            }
        }
        ++i;
    }

    if (segmentMeansLayout != nullptr) {
        while (segmentMeansLayout->count()) {
            QWidget* widgetToDelete = segmentMeansLayout->itemAt(0)->widget();
            segmentMeansLayout->removeWidget(widgetToDelete);
            delete widgetToDelete;
        }
    }

    sizeOfProcessingData = timestamps.size();

    QLineSeries* seriesDistancesVsTimestamps = new QLineSeries(this);
    chartDistancesVsTimestamps = new QChart();

    seriesDistancesVsTimestamps->setName("Tag Measurements");
    seriesDistancesVsTimestamps->setMarkerSize(10.0);
    seriesDistancesVsTimestamps->setPen(QPen(Qt::blue));

    qreal maxDistance = std::numeric_limits<qreal>::min();

    for (int i = 0; i < timestamps.size(); ++i)
    {
        QDateTime time = QDateTime::fromMSecsSinceEpoch(timestamps[i]);
        auto offsetMSeconds = time.offsetFromUtc() * 1000;
        seriesDistancesVsTimestamps->append(time.addMSecs(-offsetMSeconds).toMSecsSinceEpoch(), *distances[i]);
        if (*distances[i] > maxDistance)
            maxDistance = *distances[i];
    }

    chartDistancesVsTimestamps->addSeries(seriesDistancesVsTimestamps);
    chartDistancesVsTimestamps->setTitle("Scatter Plot of Tag Measurements");

    QLegend *legend = chartDistancesVsTimestamps->legend();
    legend->setVisible(true);
    legend->setAlignment(Qt::AlignBottom);
    legend->setPen(QPen(Qt::black));

    QDateTimeAxis *axisX = new QDateTimeAxis(this);
    axisX->setFormat("hh:mm:ss");
    chartDistancesVsTimestamps->addAxis(axisX, Qt::AlignBottom);
    seriesDistancesVsTimestamps->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis(this);

    axisY->setTickInterval(1);
    axisY->setRange(0, maxDistance + 1);
    axisY->setTickCount(maxDistance + 2);
    chartDistancesVsTimestamps->addAxis(axisY, Qt::AlignLeft);
    seriesDistancesVsTimestamps->attachAxis(axisY);

    chartViewDistancesVsTimestamps = new CustomChartView(chartDistancesVsTimestamps);
    chartViewDistancesVsTimestamps->setRenderHint(QPainter::Antialiasing);
    chartViewDistancesVsTimestamps->setMinimumHeight(500);
    chartViewDistancesVsTimestamps->setMinimumWidth(1000);
    chartsLayout->addWidget(chartViewDistancesVsTimestamps);

    connect(seriesDistancesVsTimestamps, &QLineSeries::hovered, chartViewDistancesVsTimestamps, &CustomChartView::showTooltip);

    rollingDeviationInit();

}

void DataAnalysisWindow::showPlotRollingDeviations(const std::vector<long long> &timestamps, const std::vector<double> &deviations)
{

    int i = 0;
    while (i < chartsLayout->count()) {
        QLayoutItem* item = chartsLayout->itemAt(i);
        QWidget* widget = item->widget();
        QLayout* itemLayout = item->layout();

        if (widget) {
            if (widget == chartViewRollingDeviations) {
                chartsLayout->removeWidget(widget);
                delete widget;
                chartViewRollingDeviations = nullptr;
                continue;
            } else if (widget == chartViewOriginalVsAdjustedDistances) {
                chartsLayout->removeWidget(widget);
                delete widget;
                chartViewOriginalVsAdjustedDistances = nullptr;
                continue;
            } else if (widget == calculatePolynomialRegressionButton) {
                chartsLayout->removeWidget(widget);
                continue;
            } else if (widget == updateOriginalWithAdjustedValuesButton) {
                chartsLayout->removeWidget(widget);
                continue;
            } else if (widget == exportSegementFramesForModelButton) {
                chartsLayout->removeWidget(widget);
                continue;
            }
        }

        if (itemLayout) {
            if (itemLayout == thresholdInputLayout || itemLayout == segmentMeansLayout) {
                chartsLayout->removeItem(item);
                continue;
            }
        }
        ++i;
    }

    if (segmentMeansLayout != nullptr) {
        while (segmentMeansLayout->count()) {
            QWidget* widgetToDelete = segmentMeansLayout->itemAt(0)->widget();
            segmentMeansLayout->removeWidget(widgetToDelete);
            delete widgetToDelete;
        }
    }

    QLineSeries* seriesRollingDeviations = new QLineSeries(this);
    chartRollingDeviations = new QChart();

    seriesRollingDeviations->setName("Rolling Standard Deviation");
    seriesRollingDeviations->setMarkerSize(10.0);
    seriesRollingDeviations->setPen(QPen(Qt::blue));

    maxStdDeviation = std::numeric_limits<qreal>::min();

    for (int i = 0; i < deviations.size(); ++i)
    {
        QDateTime time = QDateTime::fromMSecsSinceEpoch(timestamps[i]);
        auto offsetMSeconds = time.offsetFromUtc() * 1000;
        seriesRollingDeviations->append(time.addMSecs(-offsetMSeconds).toMSecsSinceEpoch(), deviations[i]);
        if (deviations[i] > maxStdDeviation)
            maxStdDeviation = deviations[i];
    }

    chartRollingDeviations->addSeries(seriesRollingDeviations);
    chartRollingDeviations->setTitle("Rolling Standard Deviation of Distance Measurements Over Time");

    QLegend *legend = chartRollingDeviations->legend();
    legend->setVisible(true);
    legend->setAlignment(Qt::AlignBottom);
    legend->setPen(QPen(Qt::black));

    QDateTimeAxis *axisX = new QDateTimeAxis(this);
    axisX->setFormat("hh:mm:ss");
    chartRollingDeviations->addAxis(axisX, Qt::AlignBottom);
    seriesRollingDeviations->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis(this);

    axisY->setTickInterval(0.1);
    axisY->setRange(0, maxStdDeviation + 1);
    chartRollingDeviations->addAxis(axisY, Qt::AlignLeft);
    seriesRollingDeviations->attachAxis(axisY);

    chartViewRollingDeviations = new CustomChartView(chartRollingDeviations);
    chartViewRollingDeviations->setRenderHint(QPainter::Antialiasing);
    chartViewRollingDeviations->setMinimumHeight(500);
    chartViewRollingDeviations->setMinimumWidth(1000);
    chartsLayout->addWidget(chartViewRollingDeviations);

    connect(seriesRollingDeviations, &QLineSeries::hovered, chartViewRollingDeviations, &CustomChartView::showTooltip);

    initThresholdSetting();
}

void DataAnalysisWindow::showDatasetSegments(const std::vector<double> &datasetSegmentMeans)
{
    int i = 0;
    while (i < chartsLayout->count()) {
        QLayoutItem* item = chartsLayout->itemAt(i);
        QWidget* widget = item->widget();
        QLayout* itemLayout = item->layout();

        if (widget) {
            if (widget == chartViewOriginalVsAdjustedDistances) {
                chartsLayout->removeWidget(widget);
                delete widget;
                chartViewOriginalVsAdjustedDistances = nullptr;
                continue;
            } else if (widget == calculatePolynomialRegressionButton) {
                chartsLayout->removeWidget(widget);
                continue;
            } else if (widget == updateOriginalWithAdjustedValuesButton) {
                chartsLayout->removeWidget(widget);
                continue;
            } else if (widget == exportSegementFramesForModelButton) {
                chartsLayout->removeWidget(widget);
                continue;
            }
        }
        if (itemLayout) {
            if (itemLayout == segmentMeansLayout) {
                chartsLayout->removeItem(item);
                continue;
            }
        }
        ++i;
    }

    if (segmentMeansLayout != nullptr) {
        while (segmentMeansLayout->count()) {
            QWidget* widgetToDelete = segmentMeansLayout->itemAt(0)->widget();
            segmentMeansLayout->removeWidget(widgetToDelete);
            delete widgetToDelete;
        }
    }

    referenceValues.clear();
    referenceValues.reserve(datasetSegmentMeans.size());

    int segmentNumber = 1;
    for (const double mean : datasetSegmentMeans)
    {
       referenceValues.push_back(mean);

        QWidget *meanValueContainer = new QWidget(this);
        QHBoxLayout *meanValueLayout = new QHBoxLayout(meanValueContainer);

        QLabel *label = new QLabel(QString("Mean value of segment %1: %2").arg(segmentNumber).arg(mean, 0, 'f', 2), this);
        QPushButton *adjustReferenceValueButton = new QPushButton("Set Reference Value", this);
        adjustReferenceValueButton->setFixedWidth(200);

        meanValueLayout->addWidget(label);
        meanValueLayout->addWidget(adjustReferenceValueButton);
        meanValueLayout->setAlignment(adjustReferenceValueButton, Qt::AlignLeft);
        meanValueLayout->addStretch(0);

        segmentMeansLayout->addWidget(meanValueContainer);

        connect(adjustReferenceValueButton, &QPushButton::clicked, this, [this, label, mean, segmentNumber, adjustReferenceValueButton]() {
            bool ok;
            double referenceValue = QInputDialog::getDouble(this, "Set Reference Value",
                                                   QString("Enter reference value for segment %1:").arg(segmentNumber),
                                                   0.0, 0.0, 10000.0, 2, &ok);
            if (ok) {
                label->setText(QString("Mean value of segment %1: %2 \t\t\t Reference value is set to:  %3").arg(segmentNumber).arg(mean, 0, 'f', 2).arg(referenceValue));
                adjustReferenceValueButton->setText("Change Reference Value");
                referenceValues[segmentNumber - 1] = referenceValue;
            }
        });

        segmentNumber++;
    }

    chartsLayout->addLayout(segmentMeansLayout);

    if (calculatePolynomialRegressionButton == nullptr) {
        calculatePolynomialRegressionButton = new QPushButton("Calculate Linear Regression", this);
        connect(calculatePolynomialRegressionButton, &QPushButton::clicked, this, [this]() {
            emit requestCalculatePolynomialRegression(referenceValues);
        });
    }

    calculatePolynomialRegressionButton->setFixedWidth(200);
    chartsLayout->addWidget(calculatePolynomialRegressionButton);
    chartsLayout->setAlignment(calculatePolynomialRegressionButton, Qt::AlignLeft);

}

void DataAnalysisWindow::showOriginalVsAdjustedDistances(const std::vector<long long>& timestampsToAnalyze, std::vector<double*> distancesToAnalyzeOriginal, const std::vector<double>& distancesToAnalyzeAdjusted){

    int i = 0;
    while (i < chartsLayout->count()) {
        QWidget* widget = chartsLayout->itemAt(i)->widget();

        if (widget) {
            if (widget == chartViewOriginalVsAdjustedDistances) {
                chartsLayout->removeWidget(widget);
                delete widget;
                chartViewOriginalVsAdjustedDistances = nullptr;
                break;
            } else if (widget == updateOriginalWithAdjustedValuesButton) {
                chartsLayout->removeWidget(widget);
                continue;
            } else if (widget == exportSegementFramesForModelButton) {
                chartsLayout->removeWidget(widget);
                continue;
            }
        }
        ++i;
    }

    QLineSeries* seriesOriginalDistancesVsTimestamps = new QLineSeries(this);
    QLineSeries* seriesAdjustedDistancesVsTimestamps = new QLineSeries(this);

    chartOriginalVsAdjustedDistances = new QChart();

    seriesOriginalDistancesVsTimestamps->setName("Original Measurements");
    seriesOriginalDistancesVsTimestamps->setMarkerSize(10.0);
    seriesOriginalDistancesVsTimestamps->setPen(QPen(Qt::blue));

    seriesAdjustedDistancesVsTimestamps->setName("Adjusted Measurements");
    seriesAdjustedDistancesVsTimestamps->setMarkerSize(10.0);
    seriesAdjustedDistancesVsTimestamps->setPen(QPen(Qt::green));

    qreal maxDistance = std::numeric_limits<qreal>::min();

    for (int i = 0; i < timestampsToAnalyze.size(); ++i)
    {

        QDateTime time = QDateTime::fromMSecsSinceEpoch(timestampsToAnalyze[i]);
        auto offsetMSeconds = time.offsetFromUtc() * 1000;
        seriesOriginalDistancesVsTimestamps->append(time.addMSecs(-offsetMSeconds).toMSecsSinceEpoch(), *distancesToAnalyzeOriginal[i]);
        if (*distancesToAnalyzeOriginal[i] > maxDistance)
        {
            maxDistance = *distancesToAnalyzeOriginal[i];
        }

        seriesAdjustedDistancesVsTimestamps->append(time.addMSecs(-offsetMSeconds).toMSecsSinceEpoch(), distancesToAnalyzeAdjusted[i]);
        if (distancesToAnalyzeAdjusted[i] > maxDistance)
        {
            maxDistance = distancesToAnalyzeAdjusted[i];
        }
    }

    chartOriginalVsAdjustedDistances->addSeries(seriesOriginalDistancesVsTimestamps);
    chartOriginalVsAdjustedDistances->addSeries(seriesAdjustedDistancesVsTimestamps);
    chartOriginalVsAdjustedDistances->setTitle("Comparison of original and adjusted UWB measurements");

    QLegend *legend = chartOriginalVsAdjustedDistances->legend();
    legend->setVisible(true);
    legend->setAlignment(Qt::AlignBottom);
    legend->setPen(QPen(Qt::black));

    QDateTimeAxis *axisX = new QDateTimeAxis(this);
    axisX->setFormat("hh:mm:ss");
    chartOriginalVsAdjustedDistances->addAxis(axisX, Qt::AlignBottom);
    seriesOriginalDistancesVsTimestamps->attachAxis(axisX);
    seriesAdjustedDistancesVsTimestamps->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis(this);

    axisY->setTickInterval(1);
    axisY->setRange(0, maxDistance + 1);
    axisY->setTickCount(maxDistance + 2);
    chartOriginalVsAdjustedDistances->addAxis(axisY, Qt::AlignLeft);
    seriesOriginalDistancesVsTimestamps->attachAxis(axisY);
    seriesAdjustedDistancesVsTimestamps->attachAxis(axisY);

    chartViewOriginalVsAdjustedDistances= new CustomChartView(chartOriginalVsAdjustedDistances);
    chartViewOriginalVsAdjustedDistances->setRenderHint(QPainter::Antialiasing);
    chartViewOriginalVsAdjustedDistances->setMinimumHeight(500);
    chartViewOriginalVsAdjustedDistances->setMinimumWidth(1000);
    chartsLayout->addWidget(chartViewOriginalVsAdjustedDistances);

    connect(seriesOriginalDistancesVsTimestamps, &QLineSeries::hovered, chartViewOriginalVsAdjustedDistances, &CustomChartView::showTooltip);
    connect(seriesAdjustedDistancesVsTimestamps, &QLineSeries::hovered, chartViewOriginalVsAdjustedDistances, &CustomChartView::showTooltip);

    if (updateOriginalWithAdjustedValuesButton == nullptr) {
        updateOriginalWithAdjustedValuesButton = new QPushButton("Set adjusted values", this);
        connect(updateOriginalWithAdjustedValuesButton, &QPushButton::clicked, this, [this]() {
            emit requestUpdateOriginalWithAdjustedValues();
        });
    }

    if (exportSegementFramesForModelButton == nullptr) {
        exportSegementFramesForModelButton = new QPushButton("Export segment means for data model", this);
        connect(exportSegementFramesForModelButton, &QPushButton::clicked, this, &DataAnalysisWindow::requestSegmentFramesExport);
    }

    updateOriginalWithAdjustedValuesButton->setFixedWidth(200);
    chartsLayout->addWidget(updateOriginalWithAdjustedValuesButton);
    chartsLayout->setAlignment(updateOriginalWithAdjustedValuesButton, Qt::AlignLeft);

    exportSegementFramesForModelButton->setFixedWidth(200);
    chartsLayout->addWidget(exportSegementFramesForModelButton);
    chartsLayout->setAlignment(exportSegementFramesForModelButton, Qt::AlignLeft);

}

