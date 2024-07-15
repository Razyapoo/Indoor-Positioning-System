#include "customchartview.h"

CustomChartView::CustomChartView(QChart *chart) : QChartView(chart) {
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::NoDrag);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMouseTracking(true);
    isPanning = false;
}

void CustomChartView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePos = event->pos();
        isPanning = true;
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else {
        QChartView::mousePressEvent(event);
    }
}

void CustomChartView::mouseMoveEvent(QMouseEvent *event) {
    if (isPanning) {
        QPoint delta = event->pos() - lastMousePos;
        chart()->scroll(-delta.x(), delta.y());
        lastMousePos = event->pos();
        event->accept();
    } else {
        QChartView::mouseMoveEvent(event);
    }
}

void CustomChartView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isPanning) {
        isPanning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    } else {
        QChartView::mouseReleaseEvent(event);
    }
}

// Adjusting zoom speed
void CustomChartView::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        int numDegrees = event->angleDelta().y() / 8;
        int numSteps = numDegrees / 15;

        if (numSteps > 0) {
            for (int i = 0; i < numSteps; ++i)
                chart()->zoomIn();
        } else {
            for (int i = 0; i < -numSteps; ++i)
                chart()->zoomOut();
        }

        event->accept();
    } else {
        QChartView::wheelEvent(event);
    }
}

void CustomChartView::showTooltip(const QPointF& point, bool state) {
    if (state) {
        QDateTime timePoint = QDateTime::fromMSecsSinceEpoch(point.x());
        QString tooltipText = timePoint.toString("hh:mm:ss");

        QToolTip::showText(QCursor::pos(), QString("(%1, %2)").arg(tooltipText).arg(point.y()));
    } else {
        QToolTip::hideText();
    }
}
