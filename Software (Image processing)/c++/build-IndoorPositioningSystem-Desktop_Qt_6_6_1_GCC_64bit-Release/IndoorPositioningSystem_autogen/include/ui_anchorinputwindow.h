/********************************************************************************
** Form generated from reading UI file 'anchorinputwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANCHORINPUTWINDOW_H
#define UI_ANCHORINPUTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_AnchorInputWindow
{
public:

    void setupUi(QDialog *AnchorInputWindow)
    {
        if (AnchorInputWindow->objectName().isEmpty())
            AnchorInputWindow->setObjectName("AnchorInputWindow");
        AnchorInputWindow->resize(400, 300);

        retranslateUi(AnchorInputWindow);

        QMetaObject::connectSlotsByName(AnchorInputWindow);
    } // setupUi

    void retranslateUi(QDialog *AnchorInputWindow)
    {
        AnchorInputWindow->setWindowTitle(QCoreApplication::translate("AnchorInputWindow", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AnchorInputWindow: public Ui_AnchorInputWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANCHORINPUTWINDOW_H
