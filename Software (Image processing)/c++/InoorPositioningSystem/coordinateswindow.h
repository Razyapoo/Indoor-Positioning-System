#ifndef COORDINATESWINDOW_H
#define COORDINATESWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>

namespace Ui {
class CoordinatesWindow;
}

class CoordinatesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CoordinatesWindow(QWidget *parent = nullptr, const QString& windowName = "");
    ~CoordinatesWindow();

public slots:
    void updatePosition(const QPointF& coordinates, int objectID);

private:
    Ui::CoordinatesWindow *ui;

    QVBoxLayout *mainLayout;

    QLabel *person1Label;
    QLabel *person1XLabel;
    QLabel *person1YLabel;

    QLabel *person2Label;
    QLabel *person2XLabel;
    QLabel *person2YLabel;

    QLabel *person3Label;
    QLabel *person3XLabel;
    QLabel *person3YLabel;

    void setupLayout();
};

#endif // COORDINATESWINDOW_H
