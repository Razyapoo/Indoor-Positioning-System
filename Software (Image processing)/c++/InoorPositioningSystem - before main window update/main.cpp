#include "indoorpositioningsystem.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "InoorPositioningSystem_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    IndoorPositioningSystem mainWindow;

    QObject::connect(&mainWindow, &QMainWindow::destroyed, &app, &QApplication::quit);
    mainWindow.show();
    return app.exec();
}
