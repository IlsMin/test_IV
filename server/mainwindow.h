#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "common_parts.h"
#include "executorInterface.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE


class QTcpSocket;
class networkServ;
class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(quint16 portN, QWidget * parent = nullptr);
        ~MainWindow();
        bool initAll();
    private:
        quint16 port = 12345;
        Ui::MainWindow * ui;
        QSharedPointer <networkServ> netServer;
        QThread * servThread = nullptr;
        QMap <QString, TestIV_ExecutorInterface *> executors;

        bool initServer(quint16 portN);
        bool initExecutors();
        void finishThread();

        bool checkEquipmentState(CmdParts & cmdPart);
        bool execCommand(/*const*/ CmdParts & cmdPart);
        void sayAboutBadCommands(const QVector<CmdParts> & cmnds);

    private slots:
        void fixState(const QString & mes);
        void parseNewCommands(QTcpSocket * sock, const QStringList & cmnds);

        //signals:
};
#endif // MAINWINDOW_H
