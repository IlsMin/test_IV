#include "mainwindow.h"


#include <iostream>
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char * argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("IVideon test server");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    QCommandLineOption portNumberOption(QStringList() << "p" << "port",
                                        QCoreApplication::translate("main", "Server port <number>."),
                                        QCoreApplication::translate("main", "number"));
    parser.addOption(portNumberOption);
    parser.process(a);
    unsigned short port = 12345;
    if (parser.isSet(portNumberOption))
        port = parser.value(portNumberOption).toUShort();

    MainWindow w(port);
    w.setWindowTitle(a.applicationName());
    if (!w.initAll())
    {
        std::cout << "Application not well inited (see log for details)" << std::endl;
        return -1;
    }
    w.show();
    return a.exec();
}
