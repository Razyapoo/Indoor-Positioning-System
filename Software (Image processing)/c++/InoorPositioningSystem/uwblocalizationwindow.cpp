#include "uwblocalizationwindow.h"
#include "ui_uwblocalizationwindow.h"

UWBLocalizationWindow::UWBLocalizationWindow(QWidget *parent, const std::vector<QPointF>& anchorPositions)
    : QDialog(parent)
    , ui(new Ui::UWBLocalizationWindow)
{
    ui->setupUi(this);

    setFixedSize(600, 1200);

    uwbLocalizationScene = new QGraphicsScene(this);
    uwbLocalizationScene->setItemIndexMethod(QGraphicsScene::NoIndex);

    uwbLocalizationView = new QGraphicsView(uwbLocalizationScene, this);

    for(const QPointF position: anchorPositions) {
        addAnchor(position);
    }


    uwbLocalizationView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    uwbLocalizationView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    uwbLocalizationView->setSceneRect(0, 0, 600, 1200);

    QTransform transform;
    transform.scale(1, -1); // Flip the scene along both X and Y axes
    uwbLocalizationView->setTransform(transform);
}

UWBLocalizationWindow::~UWBLocalizationWindow()
{
    delete ui;
}


void UWBLocalizationWindow::addAnchor(const QPointF& position) {
    QGraphicsRectItem* anchor = uwbLocalizationScene->addRect(QRectF((position.x() * 100), (position.y() * 100), 30, 30), QPen(Qt::black), QBrush(Qt::blue));
    anchor->setPos(position.x() * 100, position.y() * 100);
}

void UWBLocalizationWindow::addTag(const QPointF &position, int tagID) {
    QPolygonF triangle;
    triangle << QPointF(0, 30) << QPointF(30, 30) << QPointF(15, 0);
    QGraphicsPolygonItem* tagItem = uwbLocalizationScene->addPolygon(triangle, QPen(Qt::black), QBrush(Qt::red));
    tagItem->setPos(position.x() * 100, position.y() * 100);
    tagPositions[tagID] = tagItem;
}


void UWBLocalizationWindow::updateTagPosition(const QPointF& position, int tagID) {
    if (tagPositions.count(tagID) > 0) {
        tagPositions[tagID]->setPos(position.x() * 100, position.y() * 100);
    } else {
        addTag(position, tagID);
    }
}

void UWBLocalizationWindow::closeEvent(QCloseEvent *event) {
    QDialog::closeEvent(event);
    emit windowClosed();
}
