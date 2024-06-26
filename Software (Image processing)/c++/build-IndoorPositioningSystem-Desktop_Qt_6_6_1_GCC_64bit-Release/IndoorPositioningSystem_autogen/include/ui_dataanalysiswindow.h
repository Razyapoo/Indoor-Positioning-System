/********************************************************************************
** Form generated from reading UI file 'dataanalysiswindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATAANALYSISWINDOW_H
#define UI_DATAANALYSISWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_DataAnalysisWindow
{
public:

    void setupUi(QDialog *DataAnalysisWindow)
    {
        if (DataAnalysisWindow->objectName().isEmpty())
            DataAnalysisWindow->setObjectName("DataAnalysisWindow");
        DataAnalysisWindow->resize(1353, 785);

        retranslateUi(DataAnalysisWindow);

        QMetaObject::connectSlotsByName(DataAnalysisWindow);
    } // setupUi

    void retranslateUi(QDialog *DataAnalysisWindow)
    {
        DataAnalysisWindow->setWindowTitle(QCoreApplication::translate("DataAnalysisWindow", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DataAnalysisWindow: public Ui_DataAnalysisWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATAANALYSISWINDOW_H
