#ifndef UWBLOCALIZATIONWINDOW_H
#define UWBLOCALIZATIONWINDOW_H

#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QVBoxLayout>
#include <QPointF>

#include "dataprocessor.h"

namespace Ui {
class UWBLocalizationWindow;
}

class UWBLocalizationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UWBLocalizationWindow(QWidget *parent = nullptr, const std::vector<QPointF>& anchorPositions = {});
    ~UWBLocalizationWindow();

public slots:
    void updateTagPosition(const QPointF& position, int tagID);

signals:
    void windowClosed();

private:
    Ui::UWBLocalizationWindow *ui;
    // std::vector<QPointF> anchors;
    std::map<int, QGraphicsPolygonItem*> tagPositions;

    QLayout* uwbLocalizationMapLayout;
    QGraphicsView* uwbLocalizationView;
    QGraphicsScene* uwbLocalizationScene;

    void addAnchor(const QPointF& position);
    void addTag(const QPointF& position, int tagID);
    void closeEvent(QCloseEvent *event) override;


};

#endif // UWBLOCALIZATIONWINDOW_H
