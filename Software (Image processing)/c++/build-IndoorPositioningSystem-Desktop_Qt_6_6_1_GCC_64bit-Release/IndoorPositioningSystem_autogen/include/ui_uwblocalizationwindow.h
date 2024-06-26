/********************************************************************************
** Form generated from reading UI file 'uwblocalizationwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UWBLOCALIZATIONWINDOW_H
#define UI_UWBLOCALIZATIONWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UWBLocalizationWindow
{
public:

    void setupUi(QWidget *UWBLocalizationWindow)
    {
        if (UWBLocalizationWindow->objectName().isEmpty())
            UWBLocalizationWindow->setObjectName("UWBLocalizationWindow");
        UWBLocalizationWindow->resize(985, 600);

        retranslateUi(UWBLocalizationWindow);

        QMetaObject::connectSlotsByName(UWBLocalizationWindow);
    } // setupUi

    void retranslateUi(QWidget *UWBLocalizationWindow)
    {
        UWBLocalizationWindow->setWindowTitle(QCoreApplication::translate("UWBLocalizationWindow", "UWB Localization", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UWBLocalizationWindow: public Ui_UWBLocalizationWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UWBLOCALIZATIONWINDOW_H
