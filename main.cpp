#include "mainwindow.h"

#include <QStyleFactory>
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QTranslator translator;
    if (translator.load("://translations/DaveMillerNN_ru.qm")) {
        a.installTranslator(&translator);
    } else {
        qWarning() << "Не удалось загрузить файл перевода.";
    }


    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QPalette p;
    p = qApp->palette();
    p.setColor(QPalette::Window, QColor(53,53,53));
    p.setColor(QPalette::Button, QColor(63,63,63));
    p.setColor(QPalette::Disabled, QPalette::Button, QColor(43,43,43));
    p.setColor(QPalette::Highlight, QColor(242,111,115));
    p.setColor(QPalette::Disabled, QPalette::Highlight, QColor(43,43,43));
    p.setColor(QPalette::AlternateBase, QColor(43,43,43));
    qApp->setPalette(p);


    MainWindow w;
    w.show();
    return a.exec();
}
