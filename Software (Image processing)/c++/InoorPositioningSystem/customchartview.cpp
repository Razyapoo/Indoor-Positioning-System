#include "chartview.h"

ChartView::ChartView(QChart *chart) : QChartView(chart) {
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::NoDrag);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMouseTracking(true);
    isPanning = false;
}

void ChartView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePos = event->pos();
        isPanning = true;
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else {
        QChartView::mousePressEvent(event);
    }
}

void ChartView::mouseMoveEvent(QMouseEvent *event) {
    if (isPanning) {
        QPoint delta = event->pos() - lastMousePos;
        chart()->scroll(-delta.x(), delta.y());
        lastMousePos = event->pos();
        event->accept();
    } else {
        QChartView::mouseMoveEvent(event);
    }
}

void ChartView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isPanning) {
        isPanning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    } else {
        QChartView::mouseReleaseEvent(event);
    }
}

void ChartView::wheelEvent(QWheelEvent *event) {
    // Calculate the zoom factor based on the wheel delta
    int numDegrees = event->angleDelta().y() / 8;
    int numSteps = numDegrees / 15; // 15 degrees per step

    // Zoom in or out based on the wheel delta direction
    if (numSteps > 0) {
        for (int i = 0; i < numSteps; ++i)
            chart()->zoomIn();
    } else {
        for (int i = 0; i < -numSteps; ++i)
            chart()->zoomOut();
    }

    event->accept();
}
