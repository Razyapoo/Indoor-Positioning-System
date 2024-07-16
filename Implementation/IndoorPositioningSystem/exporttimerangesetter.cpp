#include "exporttimerangesetter.h"
#include "ui_exporttimerangesetter.h"

ExportTimeRangeSetter::ExportTimeRangeSetter(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ExportTimeRangeSetter)
{
    ui->setupUi(this);

    QVBoxLayout* TimeExportLayout = new QVBoxLayout(this);

    QLabel* startLabel = new QLabel("Start Time:", this);
    startTimeEdit = new QTimeEdit(this);
    startTimeEdit->setDisplayFormat("HH:mm:ss");
    TimeExportLayout->addWidget(startLabel);
    TimeExportLayout->addWidget(startTimeEdit);

    QLabel* endLabel = new QLabel("End Time:", this);
    endTimeEdit = new QTimeEdit(this);
    endTimeEdit->setDisplayFormat("HH:mm:ss");
    TimeExportLayout->addWidget(endLabel);
    TimeExportLayout->addWidget(endTimeEdit);

    QDialogButtonBox *confirmButton = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    TimeExportLayout->addWidget(confirmButton);
    connect(confirmButton, &QDialogButtonBox::accepted, this, &ExportTimeRangeSetter::accept);
    connect(confirmButton, &QDialogButtonBox::rejected, this, &ExportTimeRangeSetter::reject);

    setLayout(TimeExportLayout);
}

ExportTimeRangeSetter::~ExportTimeRangeSetter()
{
    delete ui;
}
