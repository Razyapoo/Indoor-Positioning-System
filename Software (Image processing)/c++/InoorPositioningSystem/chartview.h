#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>
#include <QMouseEvent>
#include <QValueAxis>

class ChartView : public QChartView {
    Q_OBJECT
public:
    ChartView(QChart *chart);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private:
    bool isPanning;
    QPoint lastMousePos;
};

#endif // CHARTVIEW_H

