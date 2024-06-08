#include "uwblocalizationwindow.h"
#include "ui_uwblocalizationwindow.h"

UWBLocalizationWindow::UWBLocalizationWindow(QWidget *parent, const std::vector<QPointF>& anchorPositions)
    : QDialog(parent)
    , ui(new Ui::UWBLocalizationWindow)
{
    ui->setupUi(this);

    setFixedSize(450, 1000);
    mapScale = 50; // Use map scale to enlarge the view

    uwbLocalizationScene = new QGraphicsScene(this);
    uwbLocalizationScene->setItemIndexMethod(QGraphicsScene::NoIndex);

    uwbLocalizationView = new QGraphicsView(uwbLocalizationScene, this);

    // Center Grid at center of anchor figures (half of anchor rectangle side)
    QPointF shiftInPixels(10, 10);
    drawGrid(5, 15, shiftInPixels);

    for(const QPointF position: anchorPositions) {
        addAnchor(position);
    }

    uwbLocalizationView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    uwbLocalizationView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    uwbLocalizationView->setSceneRect(-100, -100, 450, 1000); // shift scene by -100 to see axes labels
    uwbLocalizationView->setMouseTracking(true);

    // Flip scene to see it in bottom-up view
    QTransform transform;
    transform.scale(1, -1);
    uwbLocalizationView->setTransform(transform);


}

UWBLocalizationWindow::~UWBLocalizationWindow()
{
    delete ui;
}


void UWBLocalizationWindow::addAnchor(const QPointF& position) {
    qreal x, y;

    x = position.x() * mapScale;
    y = position.y() * mapScale;
    CustomRectItem* anchor = new CustomRectItem(QRectF(0, 0, 20, 20), mapScale);
    anchor->setBrush(QBrush(Qt::red));
    anchor->setPen(QPen(Qt::black));
    uwbLocalizationScene->addItem(anchor);
    anchor->setPos(x, y);
}

void UWBLocalizationWindow::addTag(const QPointF &position, int tagID) {
    QPolygonF triangle;
    qreal x, y;
    triangle << QPointF(0, 20) << QPointF(20, 20) << QPointF(10, 0);
    CustomPolygonItem* tagItem = new CustomPolygonItem(triangle, mapScale);
    tagItem->setBrush(QBrush(Qt::blue));
    tagItem->setPen(QPen(Qt::black));
    uwbLocalizationScene->addItem(tagItem);
    x = position.x() * mapScale;
    y = position.y() * mapScale;
    tagItem->setPos(x, y);
    tagPositions[tagID] = tagItem;
}


void UWBLocalizationWindow::updateTagPosition(const QPointF& position, int tagID) {
    if (tagPositions.count(tagID) > 0) {
        qreal x, y;
        x = position.x() * mapScale;
        y = position.y() * mapScale;
        tagPositions[tagID]->setPos(x, y);
    } else {
        addTag(position, tagID);
    }
}

// void UWBLocalizationWindow::closeEvent(QCloseEvent *event) {
//     QDialog::closeEvent(event);
//     emit windowClosed();
// }

void UWBLocalizationWindow::drawGrid(int widthMeters, int heightMeters, const QPointF& shiftInPixels) {

    const int width = widthMeters * mapScale;
    const int height = heightMeters * mapScale;

    uwbLocalizationScene->setSceneRect(shiftInPixels.x(), shiftInPixels.y(), width + shiftInPixels.x(), height + shiftInPixels.y());

    for (int x = 0; x <= width; x += mapScale) {
        qreal xPos = x + shiftInPixels.x();
        uwbLocalizationScene->addLine(xPos, shiftInPixels.y(), xPos, height + shiftInPixels.y(), QPen(Qt::gray));
        if (x > 0) {
            QGraphicsTextItem* label = uwbLocalizationScene->addText(QString::number(x / mapScale) + "m");
            label->setPos(xPos - label->boundingRect().width()/2, shiftInPixels.y());
            QTransform transform;
            transform.scale(1, -1);
            label->setTransform(transform);
        }
    }

    for (int y = 0; y <= height; y += mapScale) {
        qreal yPos = y + shiftInPixels.y();
        uwbLocalizationScene->addLine(shiftInPixels.x(), yPos, width + shiftInPixels.x(), yPos, QPen(Qt::gray));
        if (y > 0) {
            QGraphicsTextItem* label = uwbLocalizationScene->addText(QString::number(y / mapScale) + "m");
            label->setPos(shiftInPixels.x() - label->boundingRect().width(), yPos + label->boundingRect().height()/2);
            QTransform transform;
            transform.scale(1, -1);
            label->setTransform(transform);
        }
    }


}
