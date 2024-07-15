#ifndef UWBLOCALIZATIONWINDOW_H
#define UWBLOCALIZATIONWINDOW_H

/**************************************** UWB Localization Window *******************************************************
 * This window shows a top-down view (schema) of the scene.
 * Anchors are shown as red rectangles
 * Tags are shown as blue triangles
 * Camera is shown as the green triangle
 * It allows to hover over the shapes to see their positions!!!
*************************************************************************************************************************/

#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include <QGraphicsTextItem>

#include "dataprocessor.h"
#include "customgraphicitems.h"

namespace Ui {
class UWBLocalizationWindow;
}

class UWBLocalizationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UWBLocalizationWindow(QWidget *parent = nullptr, const std::vector<AnchorPosition>& anchorPositions = {});
    ~UWBLocalizationWindow();

public slots:
    void updateTagPosition(const QPointF& position, int tagID);

private:
    Ui::UWBLocalizationWindow *ui;
    std::map<int, CustomPolygonItem*> tagPositions; // Dynamic update of tag positions
    int mapScale;

    QLayout* uwbLocalizationMapLayout;
    QGraphicsView* uwbLocalizationView;
    QGraphicsScene* uwbLocalizationScene;

    void addAnchor(const AnchorPosition& position);
    void addTag(const QPointF& position, int tagID);
    void addCamera(const QPointF& cameraPosition);
    void drawGrid(int widthMeters, int heightMeters, const QPointF& shiftInPixels);
};

#endif // UWBLOCALIZATIONWINDOW_H
