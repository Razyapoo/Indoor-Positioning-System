#ifndef UWBLOCALIZATIONWINDOW_H
#define UWBLOCALIZATIONWINDOW_H

#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QVBoxLayout>
#include <QPointF>
#include <QLabel>
#include <QGraphicsSceneHoverEvent>


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

// signals:
//     void windowClosed();

private:
    Ui::UWBLocalizationWindow *ui;
    // std::vector<QPointF> anchors;
    std::map<int, QGraphicsPolygonItem*> tagPositions;
    // std::map<int, QGraphicsRectItem*> tagPositions;

    int mapScale;

    // const int rectangleHeight;
    // const int rectangleWidth;
    // const int trianglePoint1, trianglePoint2, trianglePoint3;

    QLayout* uwbLocalizationMapLayout;
    QGraphicsView* uwbLocalizationView;
    QGraphicsScene* uwbLocalizationScene;

    void addAnchor(const AnchorPosition& position);
    void addTag(const QPointF& position, int tagID);
    void addCamera(const QPointF& cameraPosition);
    // void closeEvent(QCloseEvent *event) override;
    void drawGrid(int widthMeters, int heightMeters, const QPointF& shiftInPixels);


};

#endif // UWBLOCALIZATIONWINDOW_H
