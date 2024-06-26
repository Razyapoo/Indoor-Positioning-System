/********************************************************************************
** Form generated from reading UI file 'exporttimerangesetter.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTTIMERANGESETTER_H
#define UI_EXPORTTIMERANGESETTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExportTimeRangeSetter
{
public:

    void setupUi(QWidget *ExportTimeRangeSetter)
    {
        if (ExportTimeRangeSetter->objectName().isEmpty())
            ExportTimeRangeSetter->setObjectName("ExportTimeRangeSetter");
        ExportTimeRangeSetter->resize(400, 300);

        retranslateUi(ExportTimeRangeSetter);

        QMetaObject::connectSlotsByName(ExportTimeRangeSetter);
    } // setupUi

    void retranslateUi(QWidget *ExportTimeRangeSetter)
    {
        ExportTimeRangeSetter->setWindowTitle(QCoreApplication::translate("ExportTimeRangeSetter", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExportTimeRangeSetter: public Ui_ExportTimeRangeSetter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTTIMERANGESETTER_H
