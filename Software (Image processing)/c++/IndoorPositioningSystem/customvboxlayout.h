#ifndef CUSTOMVBOXLAYOUT_H
#define CUSTOMVBOXLAYOUT_H

#include <QVBoxLayout>

class CustomVBoxLayout : public QVBoxLayout {
    Q_OBJECT

public:
    explicit CustomVBoxLayout(QWidget *parent = nullptr);

    void addWidget(QWidget *widget);

signals:
    void widgetAdded();
};

#endif // CUSTOMVBOXLAYOUT_H
