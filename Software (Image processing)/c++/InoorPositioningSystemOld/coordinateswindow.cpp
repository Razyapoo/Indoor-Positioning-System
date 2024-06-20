#include "coordinateswindow.h"
#include "ui_coordinateswindow.h"

CoordinatesWindow::CoordinatesWindow(QWidget *parent, const QString& windowName)
    : QDialog(parent)
    , ui(new Ui::CoordinatesWindow)
{


    mainLayout = new QVBoxLayout(this);
    person1Label = new QLabel("Person 1", this);
    person1XLabel = new QLabel("X: 0", this);
    person1YLabel = new QLabel("Y: 0", this);
    person2Label = new QLabel("Person 2", this);
    person2XLabel = new QLabel("X: 0", this);
    person2YLabel = new QLabel("Y: 0", this);
    person3Label = new QLabel("Person 3", this);
    person3XLabel = new QLabel("X: 0", this);
    person3YLabel = new QLabel("Y: 0", this);

    ui->setupUi(this);
    setWindowTitle(windowName);
    setupLayout();
}

CoordinatesWindow::~CoordinatesWindow()
{
    delete ui;
}

void CoordinatesWindow::setupLayout() {

    QGridLayout *person1Layout = new QGridLayout;
    person1Layout->addWidget(person1Label, 0, 0, 1, 2);
    person1Layout->addWidget(person1XLabel, 1, 0);
    person1Layout->addWidget(person1YLabel, 1, 1);

    QGridLayout *person2Layout = new QGridLayout;
    person2Layout->addWidget(person2Label, 0, 0, 1, 2);
    person2Layout->addWidget(person2XLabel, 1, 0);
    person2Layout->addWidget(person2YLabel, 1, 1);

    QGridLayout *person3Layout = new QGridLayout;
    person3Layout->addWidget(person3Label, 0, 0, 1, 2);
    person3Layout->addWidget(person3XLabel, 1, 0);
    person3Layout->addWidget(person3YLabel, 1, 1);

    mainLayout->addLayout(person1Layout);
    mainLayout->addLayout(person2Layout);
    mainLayout->addLayout(person3Layout);

    setLayout(mainLayout);
}

void CoordinatesWindow::updatePosition(const QPointF& position, int objectID) {

    QString xText = "x: " + QString::number(position.x(), 'f', 4);
    QString yText = "y: " + QString::number(position.y(), 'f', 4);
    switch (objectID)
    {
    case 1:
        person1XLabel->setText(xText);
        person1YLabel->setText(yText);
        break;
    case 2:
        person2XLabel->setText(xText);
        person2YLabel->setText(yText);
        break;
    case 3:
        person3XLabel->setText(xText);
        person3YLabel->setText(yText);
        break;
    }
}
