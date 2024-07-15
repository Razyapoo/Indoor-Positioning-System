#ifndef CUSTOMCHARTVIEW_H
#define CUSTOMCHARTVIEW_H

/*********************************************** Custom QChartView *********************************************
 * This is a helping class that allows to overwrite the Qt::QChartView for drawing charts e.g.:
 * Zoom in / out: ctrl + mouse wheel
 * Panning: Left mouse button
 * Tooltips: hovering by mouse
****************************************************************************************************************/

#include <QtCharts/QChartView>
#include <QMouseEvent>
#include <QValueAxis>
#include <QToolTip>
#include <QDateTime>

class CustomChartView : public QChartView {
    Q_OBJECT
public:
    CustomChartView(QChart *chart);
    void showTooltip(const QPointF& point, bool state);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    bool isPanning;
    QPoint lastMousePos;
};

#endif // CUSTOMCHARTVIEW_H

