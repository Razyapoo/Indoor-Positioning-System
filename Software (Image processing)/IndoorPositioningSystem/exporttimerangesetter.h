#ifndef EXPORTTIMERANGESETTER_H
#define EXPORTTIMERANGESETTER_H
/*********************************************** Export TimeRange Setter ***************************************
 * This is a small window, whihc allows to set a time range to be exported
****************************************************************************************************************/

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

private:
    Ui::ExportTimeRangeSetter *ui;
};

#endif // EXPORTTIMERANGESETTER_H
