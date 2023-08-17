#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "src/MainWindow/mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale: uiLanguages) {
        const QString baseName = "Backup_Utility_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow mainWindow;
    mainWindow.show();
    return a.exec();
}
