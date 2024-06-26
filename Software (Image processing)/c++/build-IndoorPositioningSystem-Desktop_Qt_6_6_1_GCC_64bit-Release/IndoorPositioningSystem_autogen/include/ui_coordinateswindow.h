/********************************************************************************
** Form generated from reading UI file 'coordinateswindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COORDINATESWINDOW_H
#define UI_COORDINATESWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_CoordinatesWindow
{
public:

    void setupUi(QDialog *CoordinatesWindow)
    {
        if (CoordinatesWindow->objectName().isEmpty())
            CoordinatesWindow->setObjectName("CoordinatesWindow");
        CoordinatesWindow->resize(400, 300);

        retranslateUi(CoordinatesWindow);

        QMetaObject::connectSlotsByName(CoordinatesWindow);
    } // setupUi

    void retranslateUi(QDialog *CoordinatesWindow)
    {
        CoordinatesWindow->setWindowTitle(QCoreApplication::translate("CoordinatesWindow", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CoordinatesWindow: public Ui_CoordinatesWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COORDINATESWINDOW_H
