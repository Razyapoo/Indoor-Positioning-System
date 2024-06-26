#ifndef CUSTOMINPUTDIALOG_H
#define CUSTOMINPUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>

class CustomInputDialog : public QDialog {
    Q_OBJECT

public:
    explicit CustomInputDialog(int segmentNumber, QWidget *parent = nullptr);
    QString getText() const;

private:
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
    int segmentNumber;
};

#endif // CUSTOMINPUTDIALOG_H
