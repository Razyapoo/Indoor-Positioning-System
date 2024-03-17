#include "customgraphicitems.h"

CustomPolygonItem::CustomPolygonItem(const QPolygonF& polygon, int mapScale) : QGraphicsPolygonItem(polygon), mapScale(mapScale) {
    setAcceptHoverEvents(true);
}

void CustomPolygonItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    setToolTip(QString("x: %1, y: %2").arg(pos().x() / mapScale).arg(pos().y() / mapScale));
    QGraphicsPolygonItem::hoverEnterEvent(event);
}

CustomRectItem::CustomRectItem(const QRectF& rect, int mapScale) : QGraphicsRectItem(rect), mapScale(mapScale) {
    setAcceptHoverEvents(true);
}

void CustomRectItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    setToolTip(QString("x: %1, y: %2").arg(pos().x() / mapScale).arg(pos().y() / mapScale));
    QGraphicsRectItem::hoverEnterEvent(event);
}
