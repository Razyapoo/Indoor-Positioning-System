#ifndef UWBLOCALIZATIONWINDOW_H
#define UWBLOCALIZATIONWINDOW_H

#include <QDialog>
#include <QWidget>
#include <QLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QPainter>

#include "dataprocessor.h"

namespace Ui {
class UWBLocalizationWindow;
}

class UWBLocalizationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UWBLocalizationWindow(QWidget *parent = nullptr, DataProcessor* dataProcessor = nullptr);
    ~UWBLocalizationWindow();


private:
    Ui::UWBLocalizationWindow *ui;
    DataProcessor* dataProcessor;

    QLayout* uwbLocalizationMapLayout;
    QGraphicsView* uwbLocalizationView;
    QGraphicsScene* uwbLocalizationScene;

    void drawRect();

};

#endif // UWBLOCALIZATIONWINDOW_H
