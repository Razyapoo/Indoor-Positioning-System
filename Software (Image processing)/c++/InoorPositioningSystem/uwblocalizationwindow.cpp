#include "uwblocalizationwindow.h"
#include "ui_uwblocalizationwindow.h"

UWBLocalizationWindow::UWBLocalizationWindow(QWidget *parent, DataProcessor* dataProcessor)
    : QDialog(parent)
    , ui(new Ui::UWBLocalizationWindow)
    , dataProcessor(dataProcessor)
{
    ui->setupUi(this);

    uwbLocalizationMapLayout = new QVBoxLayout(this);
    uwbLocalizationView = new QGraphicsView(this);
    uwbLocalizationScene = new QGraphicsScene(this);

    setAttribute(Qt::WA_DeleteOnClose);
}

UWBLocalizationWindow::~UWBLocalizationWindow()
{
    delete ui;
}
