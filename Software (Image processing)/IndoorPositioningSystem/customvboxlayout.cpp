#include "customvboxlayout.h"

CustomVBoxLayout::CustomVBoxLayout(QWidget *parent) : QVBoxLayout(parent) {
}

void CustomVBoxLayout::addWidget(QWidget *widget) {
    QVBoxLayout::addWidget(widget);
    emit widgetAdded();
}
