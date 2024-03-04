#include "dataanalysiswindow.h"
#include "ui_dataanalysiswindow.h"
#include "dataprocessor.h"

DataAnalysisWindow::DataAnalysisWindow(QWidget *parent, DataProcessor* dataProcessor, double fps)
    : QDialog(parent)
    , ui(new Ui::DataAnalysisWindow)
    , dataProcessor(dataProcessor)
    , fps(fps)
{
    ui->setupUi(this);

    // layout = new QVBoxLayout(this);



    setWindowTitle("Multiple Charts");
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



    // connect(dataProcessor, &DataProcessor::requestShowPlot, this, &DataAnalysisWindow::showPlot, Qt::DirectConnection);
    connect(dataProcessor, &DataProcessor::requestShowAvailableTags, this, &DataAnalysisWindow::showAvailableTags, Qt::DirectConnection);
    connect(dataProcessor, &DataProcessor::requestShowAvailableAnchors, this, &DataAnalysisWindow::showAvailableAnchors, Qt::DirectConnection);


    // connect(this, &DataAnalysisWindow::requestCollectDataForTag, dataProcessor, &DataProcessor::collectDataForTag, Qt::DirectConnection);
    connect(comboBoxAvailableTags, &QComboBox::currentTextChanged, dataProcessor, &DataProcessor::collectDataForTag, Qt::DirectConnection);
    // connect(comboBoxAvailableAnchors, &QComboBox::currentTextChanged, dataProcessor, &DataProcessor::collectDataForAnchor, Qt::DirectConnection);

    connect(setAnalysisTimeRangeButton, &QPushButton::clicked, this, &DataAnalysisWindow::initDataAnalysis, Qt::DirectConnection);
    connect(this, &DataAnalysisWindow::requestAnalyseData, dataProcessor, &DataProcessor::setRangeForDataAnalysis, Qt::DirectConnection);
    connect(analyzeDataButton, &QPushButton::clicked, this, &DataAnalysisWindow::plotManager, Qt::DirectConnection);
    // connect(this, &DataAnalysisWindow::requestCollectDataForAnchor, dataProcessor, &DataProcessor::collectDataForAnchor, Qt::DirectConnection);
    connect(this, &DataAnalysisWindow::requestCollectDataForPlotDistancesVsTimestamps, dataProcessor, &DataProcessor::collectDataForPlotDistancesVsTimestamps, Qt::DirectConnection);
    connect(this, &DataAnalysisWindow::requestCalculateRollingDeviation, dataProcessor, &DataProcessor::calculateRollingDeviation, Qt::DirectConnection);

    connect(dataProcessor, &DataProcessor::requestShowPlotDistancesVsTimestamps, this, &DataAnalysisWindow::showPlotDistancesVsTimestamps, Qt::DirectConnection);
    connect(dataProcessor, &DataProcessor::requestShowPlotRollingDeviations, this, &DataAnalysisWindow::showPlotRollingDeviations, Qt::DirectConnection);

    connect(this, &DataAnalysisWindow::requestSplitDataset, dataProcessor, &DataProcessor::splitDataset, Qt::DirectConnection);
    connect(dataProcessor, &DataProcessor::requestShowDatasetSegments, this, &DataAnalysisWindow::showDatasetSegments, Qt::DirectConnection);


}

DataAnalysisWindow::~DataAnalysisWindow()
{
    delete ui;
    delete chartDistancesVsTimestamps;
    delete chartRollingDeviations;
    delete tagsAndAnchorsListsLayout;
    delete rollingDeviationInputLayout;
    delete thresholdInputLayout;
    delete segmentMeansLayout;
}

void DataAnalysisWindow::initDataAnalysis() {
    QTime startTime = startAnalysisTime->time();
    QTime endTime = endAnalysisTime->time();
    long long startFrameIndex = (startTime.hour()*3600 + startTime.minute()*60 + startTime.second()) * fps;
    long long endFrameIndex = (endTime.hour()*3600 + endTime.minute()*60 + endTime.second()) * fps;
    startFrameIndex = ((startFrameIndex - 1) < 0) ? 0 : startFrameIndex - 1;
    endFrameIndex = ((endFrameIndex - 1) < 0) ? 0 : endFrameIndex - 1;
    emit requestAnalyseData(startFrameIndex, endFrameIndex);
}

// Data are collected for tag every time it is selected in combox via connect
void DataAnalysisWindow::showAvailableTags(const std::vector<int>& availableTagIDs) {

    if (comboBoxAvailableTags->count()) {
        for (int i = 0; i < comboBoxAvailableTags->count(); ++i) {
            comboBoxAvailableTags->removeItem(i);
        }
    }

    for (const int &tagID : availableTagIDs) {
        comboBoxAvailableTags->addItem(QString::number(tagID));
    }

}

// This only shows available anchors for the given tag. Does not presellected anchor data before analysis starts. Anchor data is selected at the time "Analyze" button is clicked
void DataAnalysisWindow::showAvailableAnchors(const std::vector<int>& availableAnchorIDs) {

    if (comboBoxAvailableAnchors->count()) {
        for (int i = 0; i < comboBoxAvailableAnchors->count(); ++i) {
            comboBoxAvailableAnchors->removeItem(i);
        }
    }

    for (const int& anchorID: availableAnchorIDs) {
        comboBoxAvailableAnchors->addItem(QString::number(anchorID));
    }

}

void DataAnalysisWindow::plotManager() {
    int currentAnchorID = comboBoxAvailableAnchors->currentText().toInt();
    // emit requestCollectDataForAnchor(currentAnchorID);
    emit requestCollectDataForPlotDistancesVsTimestamps(currentAnchorID);
    rollingDeviationInit();
}

void DataAnalysisWindow::rollingDeviationInit() {


    rollingDeviationInputLabel = new QLabel(this);
    rollingDeviationInputLabel->setText("Please input window size: ");
    rollingDeviationInputLabel->setFixedWidth(180);
    rollingDeviationInputLayout->addWidget(rollingDeviationInputLabel);
    rollingDeviationInputLayout->setAlignment(rollingDeviationInputLabel, Qt::AlignLeft);

    rollingDeviationInputText = new QLineEdit(this);
    rollingDeviationInputText->setFixedWidth(100);
    rollingDeviationInputLayout->addWidget(rollingDeviationInputText);
    rollingDeviationInputLayout->setAlignment(rollingDeviationInputText, Qt::AlignLeft);

    calculateRollingDeviationButton = new QPushButton("Calculate", this);
    calculateRollingDeviationButton->setFixedWidth(100);
    rollingDeviationInputLayout->addWidget(calculateRollingDeviationButton);
    rollingDeviationInputLayout->setAlignment(calculateRollingDeviationButton, Qt::AlignLeft);
    rollingDeviationInputLayout->addStretch(0);

    // rollingDeviationInputLayout->addSpacerItem(spacer);
    chartsLayout->addLayout(rollingDeviationInputLayout);

    connect(calculateRollingDeviationButton, &QPushButton::clicked, this, &DataAnalysisWindow::validateRollingDeviationInput, Qt::DirectConnection);
}

void DataAnalysisWindow::validateRollingDeviationInput() {
    QString inputText = rollingDeviationInputText->text();

    bool ok;
    int inputValue = inputText.toInt(&ok);

    if (!ok) {
        QMessageBox::critical(nullptr, "Error", "Invalid input: not an integer");
    }

    if (inputValue < 0 || inputValue > sizeOfProcessingData) {
        QMessageBox::critical(nullptr, "Error", "Invalid input: window size is out of dataset size");
    } else {
        emit requestCalculateRollingDeviation(inputValue);
    }

}


void DataAnalysisWindow::validateThresholdInput() {

    QString inputText = thresholdInputText->text();

    bool ok;
    double inputValue = inputText.toDouble(&ok);

    if (!ok) {
        QMessageBox::critical(nullptr, "Error", "Invalid input: not an double");
    }

    if (inputValue < 0 || inputValue > maxStdDeviation) {
        QMessageBox::critical(nullptr, "Error", "Invalid input: window size is out of dataset size");
    } else {
        emit requestSplitDataset(inputValue);
    }

}

void DataAnalysisWindow::initThresholdSetting() {

    for (int i = 0; i < thresholdInputLayout->count(); ++i) {
        thresholdInputLayout->removeWidget(thresholdInputLayout->itemAt(i)->widget());
        delete rollingDeviationInputLayout->itemAt(i)->widget();
    }

    for (int i = 0; i < chartsLayout->count(); ++i) {
        if (chartsLayout->itemAt(i)->layout() == thresholdInputLayout) {
            chartsLayout->removeItem(thresholdInputLayout);
        }
    }

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

    chartsLayout->addLayout(thresholdInputLayout);

    connect(thresholdInputButton, &QPushButton::clicked, this, &DataAnalysisWindow::validateThresholdInput, Qt::DirectConnection);
}



void DataAnalysisWindow::showPlotDistancesVsTimestamps(const std::vector<long long> &timestamps, const std::vector<double> &distances) {
    
    for (int i = 0; i < chartsLayout->count(); ++i) {
        if (chartsLayout->itemAt(i)->widget() == chartViewDistancesVsTimestamps) {
            chartsLayout->removeWidget(chartViewDistancesVsTimestamps);
            delete chartViewDistancesVsTimestamps;
            break;
        } else if (chartsLayout->itemAt(i)->widget() == chartViewRollingDeviations) {
            chartsLayout->removeWidget(chartViewRollingDeviations);
            delete chartViewRollingDeviations;
            break;
        }
    }
    
    for (int i = 0; i < rollingDeviationInputLayout->count(); ++i) {
        rollingDeviationInputLayout->removeWidget(rollingDeviationInputLayout->itemAt(i)->widget());
        delete rollingDeviationInputLayout->itemAt(i)->widget();
    }

    for (int i = 0; i < thresholdInputLayout->count(); ++i) {
        thresholdInputLayout->removeWidget(thresholdInputLayout->itemAt(i)->widget());
        delete thresholdInputLayout->itemAt(i)->widget();
    }

    for (int i = 0; i < segmentMeansLayout->count(); ++i) {
        segmentMeansLayout->removeWidget(segmentMeansLayout->itemAt(i)->widget());
        delete segmentMeansLayout->itemAt(i)->widget();
    }

    for (int i = 0; i < chartsLayout->count(); ++i) {
        if (chartsLayout->itemAt(i)->layout() == rollingDeviationInputLayout) {
            chartsLayout->removeItem(rollingDeviationInputLayout);
        } else if (chartsLayout->itemAt(i)->layout() == thresholdInputLayout) {
            chartsLayout->removeItem(thresholdInputLayout);
        } else if (chartsLayout->itemAt(i)->layout() == segmentMeansLayout) {
            chartsLayout->removeItem(segmentMeansLayout);
        }
    }

    sizeOfProcessingData = timestamps.size();

    QLineSeries *seriesDistancesVsTimestamps = new QLineSeries(this);
    chartDistancesVsTimestamps = new QChart();

    seriesDistancesVsTimestamps->setName("Tag Measurements");
    seriesDistancesVsTimestamps->setMarkerSize(10.0);
    seriesDistancesVsTimestamps->setPen(QPen(Qt::blue));

    qreal maxDistance = std::numeric_limits<qreal>::min();

    for (int i = 0; i < timestamps.size(); ++i) {
        seriesDistancesVsTimestamps->append(timestamps[i], distances[i]);
        if (distances[i] > maxDistance) maxDistance = distances[i];
    }

    chartDistancesVsTimestamps->addSeries(seriesDistancesVsTimestamps);
    chartDistancesVsTimestamps->setTitle("Scatter Plot of Tag Measurements");

    QLegend *legend = chartDistancesVsTimestamps->legend();
    legend->setVisible(true);
    legend->setAlignment(Qt::AlignBottom);
    legend->setPen(QPen(Qt::black));

    QValueAxis *axisX = new QValueAxis(this);
    chartDistancesVsTimestamps->addAxis(axisX, Qt::AlignBottom);
    seriesDistancesVsTimestamps->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis(this);

    axisY->setTickInterval(1);
    axisY->setRange(0, maxDistance + 1);
    axisY->setTickCount(maxDistance + 2);
    chartDistancesVsTimestamps->addAxis(axisY, Qt::AlignLeft);
    seriesDistancesVsTimestamps->attachAxis(axisY);

    chartViewDistancesVsTimestamps = new ChartView(chartDistancesVsTimestamps);
    chartViewDistancesVsTimestamps->setRenderHint(QPainter::Antialiasing);
    chartViewDistancesVsTimestamps->setMinimumHeight(500);
    chartViewDistancesVsTimestamps->setMinimumWidth(1000);
    chartsLayout->addWidget(chartViewDistancesVsTimestamps);
}

void DataAnalysisWindow::showPlotRollingDeviations(const std::vector<long long>& timestamps, const std::vector<double>& deviations) {

    for (int i = 0; i < chartsLayout->count(); ++i) {
        if (chartsLayout->itemAt(i)->widget() == chartViewRollingDeviations) {
            chartsLayout->removeWidget(chartViewRollingDeviations);
            delete chartViewRollingDeviations;
            break;
        }
    }

    for (int i = 0; i < thresholdInputLayout->count(); ++i) {
        thresholdInputLayout->removeWidget(thresholdInputLayout->itemAt(i)->widget());
        delete thresholdInputLayout->itemAt(i)->widget();
    }

    for (int i = 0; i < segmentMeansLayout->count(); ++i) {
        segmentMeansLayout->removeWidget(segmentMeansLayout->itemAt(i)->widget());
        delete segmentMeansLayout->itemAt(i)->widget();
    }

    for (int i = 0; i < chartsLayout->count(); ++i) {
        if (chartsLayout->itemAt(i)->layout() == thresholdInputLayout) {
            chartsLayout->removeItem(thresholdInputLayout);
        } else if (chartsLayout->itemAt(i)->layout() == segmentMeansLayout) {
            chartsLayout->removeItem(segmentMeansLayout);
        }
    }

    QLineSeries *seriesRollingDeviations = new QLineSeries(this);
    chartRollingDeviations = new QChart();

    seriesRollingDeviations->setName("Rolling Standard Deviation");
    seriesRollingDeviations->setMarkerSize(10.0);
    seriesRollingDeviations->setPen(QPen(Qt::blue));

    maxStdDeviation = std::numeric_limits<qreal>::min();

    for (int i = 0; i < deviations.size(); ++i) {
        seriesRollingDeviations->append(timestamps[i], deviations[i]);
        if (deviations[i] > maxStdDeviation) maxStdDeviation = deviations[i];
    }

    chartRollingDeviations->addSeries(seriesRollingDeviations);
    chartRollingDeviations->setTitle("Rolling Standard Deviation of Distance Measurements Over Time");

    QLegend *legend = chartRollingDeviations->legend();
    legend->setVisible(true);
    legend->setAlignment(Qt::AlignBottom);
    legend->setPen(QPen(Qt::black));

    QValueAxis *axisX = new QValueAxis(this);
    chartRollingDeviations->addAxis(axisX, Qt::AlignBottom);
    seriesRollingDeviations->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis(this);

    axisY->setTickInterval(0.1);
    axisY->setRange(0, maxStdDeviation + 1);
    chartRollingDeviations->addAxis(axisY, Qt::AlignLeft);
    seriesRollingDeviations->attachAxis(axisY);

    chartViewRollingDeviations = new ChartView(chartRollingDeviations);
    chartViewRollingDeviations->setRenderHint(QPainter::Antialiasing);
    chartViewRollingDeviations->setMinimumHeight(500);
    chartViewRollingDeviations->setMinimumWidth(1000);
    chartsLayout->addWidget(chartViewRollingDeviations);

    initThresholdSetting();
}

void DataAnalysisWindow::showDatasetSegments(const std::vector<double> &datasetSegmentMeans) {

    int segmentNumber = 1;
    for (const double mean: datasetSegmentMeans) {
        QLabel *segmentMeanLabel = new QLabel(this);
        segmentMeanLabel->setText(QString("Mean value of segment %1: %2").arg(segmentNumber).arg(mean, 0, 'f', 2));
        segmentMeansLayout->addWidget(segmentMeanLabel);
        segmentNumber++;
    }

    chartsLayout->addLayout(segmentMeansLayout);
}
