#ifndef COORDINATESWINDOW_H
#define COORDINATESWINDOW_H

/*********************************************** Coordinates Window ********************************************
 * This GUI window enables to show coordinates in a dedicated window for each estimation method separatelly.
****************************************************************************************************************/

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>

#include "structures.h"
namespace Ui {
    class CoordinatesWindow;
}

class CoordinatesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CoordinatesWindow(QWidget *parent = nullptr, const QString& windowName = "", CoordinateWindowObjectType type = CoordinateWindowObjectType::Person);
    ~CoordinatesWindow();

public slots:
    void updatePosition(const QPointF& coordinates, int objectID);

private:
    Ui::CoordinatesWindow *ui;

    QVBoxLayout *mainLayout;

    QLabel *object1Label;
    QLabel *object1XLabel;
    QLabel *object1YLabel;

    QLabel *object2Label;
    QLabel *object2XLabel;
    QLabel *object2YLabel;

    QLabel *object3Label;
    QLabel *object3XLabel;
    QLabel *object3YLabel;

    void setupLayout();
};

#endif // COORDINATESWINDOW_H
