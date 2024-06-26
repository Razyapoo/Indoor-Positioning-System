#ifndef CHARTLAYOUT_H
#define CHARTLAYOUT_H

class ChartLayout : public QVBoxLayout {
    Q_OBJECT

public:
    using QVBoxLayout::QVBoxLayout; // Inherit constructors

    void addWidget(QWidget *widget) override {
        QVBoxLayout::addWidget(widget);
        emit widgetAdded(widget);
    }

signals:
    void widgetAdded(QWidget* widget);
};

#endif // CHARTLAYOUT_H
