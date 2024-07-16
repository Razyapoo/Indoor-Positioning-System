#include "anchorinputwindow.h"
#include "ui_anchorinputwindow.h"

AnchorInputWindow::AnchorInputWindow(QWidget *parent, IndoorPositioningSystemViewModel* viewModel)
    : QDialog(parent)
    , ui(new Ui::AnchorInputWindow)
    , viewModel(viewModel)
{

    ui->setupUi(this);
    setupLayout();
    setWindowTitle("Add Anchor");
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    connect(this, &AnchorInputWindow::requestStartTimer, viewModel, &IndoorPositioningSystemViewModel::onStartTimer);
}

AnchorInputWindow::~AnchorInputWindow()
{
    delete ui;
}

//-------------------------------- Window declaration --------------------------------
void AnchorInputWindow::setupLayout()
{
    mainLayout = new QVBoxLayout(this);

    // Anchor ID Input
    QHBoxLayout* anchorIdLayout = new QHBoxLayout;
    QLabel* anchorIdLabel = new QLabel("Anchor ID:", this);
    anchorIdInput = new QLineEdit(this);
    anchorIdInput->setValidator(new QIntValidator(0, 1000, this));
    anchorIdLayout->addWidget(anchorIdLabel);
    anchorIdLayout->addWidget(anchorIdInput);

    // X Position Input
    QHBoxLayout* xPosLayout = new QHBoxLayout;
    QLabel* xPosLabel = new QLabel("X Position:", this);
    xPosInput = new QLineEdit(this);
    QDoubleValidator* xPosValidator = new QDoubleValidator(this);
    xPosValidator->setLocale(QLocale::C);
    xPosInput->setValidator(xPosValidator);
    xPosLayout->addWidget(xPosLabel);
    xPosLayout->addWidget(xPosInput);

    // Y Position Input
    QHBoxLayout* yPosLayout = new QHBoxLayout;
    QLabel* yPosLabel = new QLabel("Y Position:", this);
    yPosInput = new QLineEdit(this);
    QDoubleValidator* yPosValidator = new QDoubleValidator(this);
    yPosValidator->setLocale(QLocale::C);
    yPosInput->setValidator(yPosValidator);
    yPosLayout->addWidget(yPosLabel);
    yPosLayout->addWidget(yPosInput);

    // Set if origin
    QHBoxLayout* isOriginLayout = new QHBoxLayout;
    QLabel* isOriginLabel = new QLabel("Is Origin:", this);
    isOriginInput = new QCheckBox(this);
    isOriginLayout->addWidget(isOriginLabel);
    isOriginLayout->addWidget(isOriginInput);

    // Add Anchor or Finish Adding
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    QPushButton* addButton = new QPushButton("Add Anchor", this);
    QPushButton* stopButton = new QPushButton("Finish Adding", this);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(stopButton);

    connect(addButton, &QPushButton::clicked, this, &AnchorInputWindow::onAddAnchorClicked);
    connect(stopButton, &QPushButton::clicked, this, &AnchorInputWindow::onStopAnchorAddingClicked);

    mainLayout->addLayout(anchorIdLayout);
    mainLayout->addLayout(xPosLayout);
    mainLayout->addLayout(yPosLayout);
    mainLayout->addLayout(isOriginLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

// Check if input is valid
void AnchorInputWindow::onAddAnchorClicked() {

    bool idOk, xOk, yOk;

    int anchorID = anchorIdInput->text().toInt(&idOk);
    qreal x = xPosInput->text().toDouble(&xOk);
    qreal y = yPosInput->text().toDouble(&yOk);
    bool isOrigin = isOriginInput->isChecked();

    if (idOk && xOk && yOk) {
        bool success = false;
        if (viewModel) {
            success = viewModel->addAnchorPosition(anchorID, x, y, isOrigin);
        }

        if (!success) {
            QMessageBox::StandardButton reply;

            reply = QMessageBox::question(this, "New Origin", "Do you want to reset origin?",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                viewModel->unsetAnchorOrigin();
                viewModel->addAnchorPosition(anchorID, x, y, isOrigin);
            } else {
                QMessageBox::warning(this, "Fail", "Anchor was not added. Please try again.");
            }
        }

        isOriginInput->setChecked(false);
        anchorIdInput->clear();
        xPosInput->clear();
        yPosInput->clear();
    } else {
        QMessageBox::warning(this, "Invalid Input", "Please enter valid values for all fields.");
    }
}

// Stop adding and go for playing video!
void AnchorInputWindow::onStopAnchorAddingClicked() {
    if (viewModel->isOriginExists()) { // Origin should always be set. Necessary for calculation of coordinates
        accept();
        emit requestStartTimer();
    } else {
        QMessageBox::warning(this, "Missing Origin", "You must set one anchor as the origin. If you forgot to check 'Is Origin' please re-enter the anchor's data and mark it as the origin.");
        return;
    }
}
