#include "dataanalysiswindow.h"
#include "ui_dataanalysiswindow.h"
#include "dataprocessor.h"

DataAnalysisWindow::DataAnalysisWindow(QWidget *parent, DataProcessor* dataProcessor)
    : QDialog(parent)
    , ui(new Ui::DataAnalysisWindow)
    , dataProcessor(dataProcessor)
{
    ui->setupUi(this);

    // layout = new QVBoxLayout(this);


    setWindowTitle("Multiple Charts");

    connect(dataProcessor, &DataProcessor::requestShowPlot, this, &DataAnalysisWindow::showPlot, Qt::DirectConnection);

}

DataAnalysisWindow::~DataAnalysisWindow()
{
    delete ui;
}


void DataAnalysisWindow::showPlot() {
    // // Create and add the first chart
    // QChartView* chartView1 = createChartView();
    // // ui->horizontalLayout->addWidget(chartView1);


    // // Create and add the second chart
    // QChartView* chartView2 = createChartView();


    // QHBoxLayout* myLayout = new QHBoxLayout;
    // QHBoxLayout* myLayout1 = new QHBoxLayout;
    // myLayout->addWidget(chartView1);
    // myLayout1->addWidget(chartView2);

    QHBoxLayout* myLayout = new QHBoxLayout(this);

    // ui->frame->setLayout(myLayout);
    // ui->frame_2->setLayout(myLayout1);
    QComboBox* comboBox = new QComboBox();
    comboBox->addItem("Option 1");
    comboBox->addItem("Option 2");
    comboBox->addItem("Option 3");
    myLayout->addWidget(comboBox);

    QScatterSeries *series = new QScatterSeries();
    series->setName("Tag Measurements");
    series->setMarkerShape(QScatterSeries::MarkerShapeCircle); // Optional
    series->setMarkerSize(10.0); // Visible size for scatter plot

    // Assuming you have a vector of measurements and corresponding timestamps
    // Here's dummy data for demonstration
    series->append(1, 5); // Append point at x=1, y=5
    series->append(2, 15);
    series->append(3, 8);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Scatter Plot of Tag Measurements");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    myLayout->addWidget(chartView);
    // ui->horizontalLayout->addWidget(chartView2);

    // chartView1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // chartView2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Add more charts as needed...
    // layout->update();
    // layout->activat
    // setLayout(layout);

    // ui->formFrame->addWidget(chartView1, 0, 0); // First row, first column
    // ui->formFrame->addWidget(chartView2, 0, 1);

}

QChartView* DataAnalysisWindow::createChartView() {
    // Example chart creation (customize as needed)
    QLineSeries* series = new QLineSeries();
    // *series << QPointF(0, 0) << QPointF(1, 100) << QPointF(100, 3);
    // auto series = new QLineSeries();
    for (int i = 0; i < 500; ++i) {
        qreal x = i;
        qreal y = std::sin(M_PI / 50 * i) * 100;
        y += QRandomGenerator::global()->bounded(20); // Add random noise
        series->append(x, y);
    }

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    // chartView->setParent(ui->horizontalLayout);
    return chartView;
}


