#include "customlabel.h"

CustomLabel::CustomLabel(QWidget *parent) : QLabel(parent) {
    setMouseTracking(true);
}


void CustomLabel::mouseMoveEvent(QMouseEvent *event) {
    QSize imageSize = QSize(640, 640);
    QSize labelSize = this->size();

    // Calculate scale factors while keeping aspect ratio
    double scaleW = static_cast<double>(labelSize.width()) / imageSize.width();
    double scaleH = static_cast<double>(labelSize.height()) / imageSize.height();
    double scale = qMin(scaleW, scaleH);

    // Calculate the original image coordinates
    int originalX = static_cast<int>(event->pos().x() / scale);
    int originalY = static_cast<int>(event->pos().y() / scale);

    // Ensure coordinates are within the original image bounds
    originalX = qMin(originalX, imageSize.width() - 1);
    originalY = qMin(originalY, imageSize.height() - 1);

    // Display tooltip with original image coordinates
    QToolTip::showText(event->globalPos(), QString("X: %1, Y: %2").arg(originalX).arg(originalY), this);
}
