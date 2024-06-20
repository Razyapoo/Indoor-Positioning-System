#ifndef CUSTOMGRAPHICITEMS_H
#define CUSTOMGRAPHICITEMS_H

#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneHoverEvent>

class CustomPolygonItem : public QGraphicsPolygonItem {
public:
    CustomPolygonItem(const QPolygonF& polygon, int mapScale);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

private:
    int mapScale;
};

class CustomRectItem : public QGraphicsRectItem {
public:
    CustomRectItem(const QRectF& rect, int mapScale);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

private:
    int mapScale;
};

#endif // CUSTOMGRAPHICITEMS_H
