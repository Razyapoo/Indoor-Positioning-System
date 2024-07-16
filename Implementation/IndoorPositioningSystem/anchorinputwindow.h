#ifndef ANCHORINPUTWINDOW_H
#define ANCHORINPUTWINDOW_H

/*********************************************** Anchor Input Window ***********************************************
 * This GUI window enables to set up anchors and their positions when video (project) is selected
****************************************************************************************************************/

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QCheckBox>
#include <QMessageBox>

#include "structures.h"
#include "indoorpositioningsystemviewmodel.h"

namespace Ui {
class AnchorInputWindow;
}

class AnchorInputWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AnchorInputWindow(QWidget *parent = nullptr, IndoorPositioningSystemViewModel* viewModel = nullptr);
    ~AnchorInputWindow();

private slots:
    void onAddAnchorClicked();
    void onStopAnchorAddingClicked();

signals:
    void requestStartTimer();
private:
    Ui::AnchorInputWindow *ui;
    IndoorPositioningSystemViewModel *viewModel;

    QLineEdit* anchorIdInput;
    QCheckBox* isOriginInput;
    QLineEdit* xPosInput;
    QLineEdit* yPosInput;
    QVBoxLayout* mainLayout;

    void setupLayout();
};

#endif // ANCHORINPUTWINDOW_H
