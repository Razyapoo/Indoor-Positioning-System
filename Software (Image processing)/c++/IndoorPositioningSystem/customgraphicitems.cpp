#include "customgraphicitems.h"

CustomPolygonItem::CustomPolygonItem(const QPolygonF& polygon, int mapScale) : QGraphicsPolygonItem(polygon), mapScale(mapScale), label(nullptr) {
    setAcceptHoverEvents(true);
}

void CustomPolygonItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    setToolTip(QString("x: %1, y: %2").arg(pos().x() / mapScale).arg(pos().y() / mapScale));
    QGraphicsPolygonItem::hoverEnterEvent(event);
}

void CustomPolygonItem::setLabel(QGraphicsTextItem *labelItem, int xOffset, int yOffset)
{
    label = labelItem;
    updateLabelPosition(xOffset, yOffset);
}

void CustomPolygonItem::updateLabelPosition(int xOffset, int yOffset)
{
    if (label) {
        QPointF pos = this->pos();
        label->setPos(pos.x() + xOffset, pos.y() + yOffset);
    }
}

CustomRectItem::CustomRectItem(const QRectF& rect, int mapScale) : QGraphicsRectItem(rect), mapScale(mapScale), label(nullptr) {
    setAcceptHoverEvents(true);
}

void CustomRectItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    setToolTip(QString("x: %1, y: %2").arg(pos().x() / mapScale).arg(pos().y() / mapScale));
    QGraphicsRectItem::hoverEnterEvent(event);
}

void CustomRectItem::setLabel(QGraphicsTextItem *labelItem, int xOffset, int yOffset)
{
    label = labelItem;
    updateLabelPosition(xOffset, yOffset);
}

void CustomRectItem::updateLabelPosition(int xOffset, int yOffset)
{
    if (label) {
        QPointF pos = this->pos();
        label->setPos(pos.x() + xOffset, pos.y() + yOffset);
    }
}
