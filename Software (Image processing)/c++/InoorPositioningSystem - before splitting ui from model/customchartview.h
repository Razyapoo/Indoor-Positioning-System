#ifndef CUSTOMCHARTVIEW_H
#define CUSTOMCHARTVIEW_H

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

