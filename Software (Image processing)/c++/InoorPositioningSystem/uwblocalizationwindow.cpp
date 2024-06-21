#include "uwblocalizationwindow.h"
#include "ui_uwblocalizationwindow.h"

UWBLocalizationWindow::UWBLocalizationWindow(QWidget *parent, const std::vector<AnchorPosition>& anchorPositions)
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
    drawGrid(5, 18, shiftInPixels);

    QPointF cameraPosition;

    for(const AnchorPosition& position: anchorPositions) {
        addAnchor(position);
        if (position.isOrigin) {
            cameraPosition = {position.x + 1.25, position.y - 2.08}; // as for now hard set
        }
    }

    addCamera(cameraPosition);

    uwbLocalizationView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    uwbLocalizationView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    uwbLocalizationView->setSceneRect(-100, -50, 450, 1000); // shift scene by -100 to see axes labels
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


void UWBLocalizationWindow::addAnchor(const AnchorPosition& position) {
    qreal x, y;

    x = position.x * mapScale;
    y = position.y * mapScale;
    CustomRectItem* anchor = new CustomRectItem(QRectF(0, 0, 20, 20), mapScale);
    anchor->setBrush(QBrush(Qt::red));
    anchor->setPen(QPen(Qt::black));
    uwbLocalizationScene->addItem(anchor);
    anchor->setPos(x, y);

    QGraphicsTextItem* label = new QGraphicsTextItem(QString("Anchor %1").arg(position.anchorID));
    uwbLocalizationScene->addItem(label);
    QTransform transform;
    transform.scale(1, -1);
    label->setTransform(transform);
    anchor->setLabel(label, -25, 0);
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

    QGraphicsTextItem* label = new QGraphicsTextItem(QString("Tag %1").arg(tagID));
    uwbLocalizationScene->addItem(label);
    QTransform transform;
    transform.scale(1, -1);
    label->setTransform(transform);
    tagItem->setLabel(label, -10, 0);

}

void UWBLocalizationWindow::addCamera(const QPointF& cameraPosition) {
    QPolygonF triangle;
    qreal x, y;
    triangle << QPointF(0, 20) << QPointF(20, 20) << QPointF(10, 0);
    CustomPolygonItem* camera = new CustomPolygonItem(triangle, mapScale);
    camera->setBrush(QBrush(Qt::green));
    camera->setPen(QPen(Qt::black));
    uwbLocalizationScene->addItem(camera);
    x = cameraPosition.x() * mapScale;
    y = cameraPosition.y() * mapScale;
    camera->setPos(x, y);

    QGraphicsTextItem* label = new QGraphicsTextItem(QString("Camera"));
    uwbLocalizationScene->addItem(label);
    QTransform transform;
    transform.scale(1, -1);
    label->setTransform(transform);
    camera->setLabel(label, -20, 40);
}

void UWBLocalizationWindow::updateTagPosition(const QPointF& position, int tagID) {
    if (tagPositions.count(tagID) > 0) {
        qreal x, y;
        x = position.x() * mapScale;
        y = position.y() * mapScale;
        tagPositions[tagID]->setPos(x, y);
        tagPositions[tagID]->updateLabelPosition(-10, 0);
    } else {
        addTag(position, tagID);
    }
}

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
