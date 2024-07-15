#include "custominputdialog.h"

CustomInputDialog::CustomInputDialog(int segmentNumber, QWidget *parent) : QDialog(parent), segmentNumber(segmentNumber) {
    label = new QLabel(QString("Enter reference value for segment %1:").arg(segmentNumber), this);
    lineEdit = new QLineEdit(this);

    okButton = new QPushButton("OK", this);
    connect(okButton, &QPushButton::clicked, this, &CustomInputDialog::accept);
    cancelButton = new QPushButton("Cancel", this);
    connect(cancelButton, &QPushButton::clicked, this, &CustomInputDialog::reject);

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(okButton);
    hLayout->addWidget(cancelButton);

    vLayout->addWidget(label);
    vLayout->addWidget(lineEdit);
    vLayout->addLayout(hLayout);

    setLayout(vLayout);
}

QString CustomInputDialog::getText() const {
    return lineEdit->text();
}
