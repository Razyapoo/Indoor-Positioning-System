#ifndef CUSTOMGRAPHICITEMS_H
#define CUSTOMGRAPHICITEMS_H

#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsTextItem>

class CustomPolygonItem : public QGraphicsPolygonItem {
public:
    CustomPolygonItem(const QPolygonF& polygon, int mapScale);

    void setLabel(QGraphicsTextItem *label, int xOffset, int yOffset);
    void updateLabelPosition(int xOffset, int yOffset);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

private:
    int mapScale;
    QGraphicsTextItem *label;
};

class CustomRectItem : public QGraphicsRectItem {
public:
    CustomRectItem(const QRectF& rect, int mapScale);

    void setLabel(QGraphicsTextItem *label, int xOffset, int yOffset);
    void updateLabelPosition(int xOffset, int yOffset);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

private:
    int mapScale;
    QGraphicsTextItem *label;
};

#endif // CUSTOMGRAPHICITEMS_H
