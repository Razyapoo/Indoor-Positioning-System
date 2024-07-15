#ifndef CUSTOMGRAPHICITEMS_H
#define CUSTOMGRAPHICITEMS_H

/**************************************** Custom Graphic Items ******************************************************
 * This is a helping class that allows to overwrite the Qt::QGraphicsItem for dynamic drawing of graphic items e.g.:
 * Blue triangle for Tags
 * Green triangle for camera
 * Red squares for anchors
 * + handling hovering over to show their coordinates
*********************************************************************************************************************/

#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsTextItem>

//-------------------------------- Declaration of CustomPolygonItem --------------------------------
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

//-------------------------------- Declaration of CustomRectItem --------------------------------
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
