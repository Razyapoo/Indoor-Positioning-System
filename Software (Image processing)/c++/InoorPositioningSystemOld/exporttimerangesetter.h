#ifndef EXPORTTIMERANGESETTER_H
#define EXPORTTIMERANGESETTER_H

#include <QDialog>
#include <QTimeEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>

namespace Ui {
class ExportTimeRangeSetter;
}

class ExportTimeRangeSetter : public QDialog
{
    Q_OBJECT

public:
    explicit ExportTimeRangeSetter(QWidget *parent = nullptr);
    ~ExportTimeRangeSetter();

    QTimeEdit* startTimeEdit;
    QTimeEdit* endTimeEdit;
    // QTime getStartTime();
    // QTime getEndTime();


// private slots:
//     void onConfirm();

private:
    Ui::ExportTimeRangeSetter *ui;

    // void closeEvent(QCloseEvent *event) override;
};

#endif // EXPORTTIMERANGESETTER_H
