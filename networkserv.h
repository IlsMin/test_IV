#ifndef NETWORKSERV_H
#define NETWORKSERV_H

#include <QObject>
#include <QMap>
#include <QSharedPointer>

#include "common_parts.h"

class QTcpSocket;
class QTcpServer;
class networkServ : public QObject
{
        Q_OBJECT
    public:
        explicit networkServ(QObject * parent = nullptr);
        ~networkServ();

        bool initServer(quint16 portN);
        QString getLastError();
        void finishAll();

    private:

        QSharedPointer <QTcpServer> tcpServer;
        QMap <QTcpSocket *, QString> socketData;

    private slots:
        void onNewConnection();
        void onSocketReadyRead();
        void onSocketError();
        void onSocketConnectionClosed();
    public slots:
        void onAnswerCommands(const QVector<CmdParts> & responses);

    signals:
        void stateMessage(const QString & msg);
        void newCommandsArived(QTcpSocket * sender, const QStringList & cmds);

};

#endif // NETWORKSERV_H
