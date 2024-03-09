#include "uwblocalizationwindow.h"
#include "ui_uwblocalizationwindow.h"

UWBLocalizationWindow::UWBLocalizationWindow(QWidget *parent, DataProcessor* dataProcessor)
    : QDialog(parent)
    , ui(new Ui::UWBLocalizationWindow)
    , dataProcessor(dataProcessor)
{
    ui->setupUi(this);

    // uwbLocalizationMapLayout = new QVBoxLayout(this);
    uwbLocalizationScene = new QGraphicsScene(this);
    uwbLocalizationView = new QGraphicsView(uwbLocalizationScene, this);

    // uwbLocalizationMapLayout->addWidget(uwbLocalizationScene);
    drawRect();

    // setAttribute(Qt::WA_DeleteOnClose);
}

UWBLocalizationWindow::~UWBLocalizationWindow()
{
    delete ui;
}


void UWBLocalizationWindow::drawRect() {
    QGraphicsRectItem *rect = new QGraphicsRectItem();
    rect->setRect(50, 50, 100, 100);
    rect->setBrush(Qt::blue);
    rect->setPos(50, 50);

    uwbLocalizationScene->addItem(rect);
}
