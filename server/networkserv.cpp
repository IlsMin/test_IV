#include <QTcpSocket>
#include <QTcpServer>

#include "networkserv.h"
#include "common_parts.h"
//----------------------------------------------------------------------------------
//
networkServ::networkServ( QObject * parent)
    : QObject{parent}
{
    tcpServer.reset(new QTcpServer(parent));
}

//----------------------------------------------------------------------------------
//
networkServ::~networkServ()
{
    finishAll();
}
//----------------------------------------------------------------------------------
//
void networkServ::finishAll()
{
    qDebug() << "finish serv" << tcpServer.data() << socketData.size();
    if (!tcpServer.isNull())
    {

        tcpServer->blockSignals(true);

        tcpServer.clear();
        //tcpServer->close();
        tcpServer.reset();
    }

    for ( QTcpSocket * sock : socketData.keys())
        sock->deleteLater();
    socketData.clear();
}
//----------------------------------------------------------------------------------
//
QString  networkServ::getLastError()

{
    return tcpServer.isNull() ? "tcpServer is null" : tcpServer->errorString();
}
//----------------------------------------------------------------------------------
//
bool networkServ::initServer(quint16 portN)
{
    QString msg;
    bool rez = false;
    QTcpSocket sock;
    //    qDebug() << sock.socketOption(QAbstractSocket::ShareAddress) << sock.socketOption(
    //                 QAbstractSocket::ReuseAddressHint);
    //<< sock.socketOption(QAbstractSocket::ShareAddress);
    //sock.setSocketOption();
    if (!tcpServer->listen(QHostAddress::Any, portN))
        msg = QString("Can't listen port %1. %2").arg(portN).arg(tcpServer->errorString());
    else
    {
        rez = true;
        msg = "server inited";
        connect( tcpServer.data(), &QTcpServer::newConnection, this, &networkServ::onNewConnection);
    }

    emit stateMessage(msg);
    return rez;
}
//----------------------------------------------------------------------------------
//
void networkServ::onNewConnection()
{
    QTcpSocket * socket = tcpServer->nextPendingConnection();
    socketData.insert(socket, QString());
    connect(socket, &QTcpSocket::readyRead,    this, &networkServ::onSocketReadyRead       );
    connect(socket, &QTcpSocket::disconnected, this, &networkServ::onSocketConnectionClosed);
    //  connect(socket, &QTcpSocket::bytesWritten(qint64)), SLOT(socketBytesWritten(qint64)));
    emit stateMessage(QString("new connection %1").arg(socketData.size()));

}
//----------------------------------------------------------------------------------
//
void networkServ::onSocketReadyRead()
{

    QTcpSocket * sock = qobject_cast<QTcpSocket *>(sender());
    if (nullptr == sock)
    {
        emit stateMessage("Unknown object sent signal about socket data");
        return;
    }

    QString data = socketData[sock];
    while (sock->bytesAvailable())
    {
        char buf[1024];   // max packet length?
        int len = sock->readLine(buf, sizeof(buf));
        if (len > 0)
            data.append( QString::fromUtf8(buf, len));
        //else break;
    }
    QStringList commands = data.split(END_COMMAND, Qt::SkipEmptyParts);
    const auto lastCmd = commands.last();
    if (lastCmd.endsWith(END_COMMAND)) // full command
        socketData[sock] = QString();
    else
    {
        socketData[sock] = lastCmd;
        commands.removeLast();     // not full command string
    }
    emit newCommandsArived(sock, commands);
}
//----------------------------------------------------------------------------------
//
void networkServ::onSocketError()
{
    QString msg;
    QTcpSocket * sock = qobject_cast<QTcpSocket *>(sender());
    if (nullptr == sock)
        msg  = "Unknown object sent signal about socket error";
    else msg = QString("Socket error:%1").arg(sock->errorString());

    emit stateMessage(msg);
}
//----------------------------------------------------------------------------------
//
void networkServ::onSocketConnectionClosed()
{
    QString msg;
    QTcpSocket * sock = qobject_cast<QTcpSocket *>(sender());
    if (nullptr == sock)
        msg  = "Unknown object sent signal about socket closing";
    else msg = "Socket closed: " + sock->errorString();
    emit stateMessage(msg);

    socketData.remove(sock);
    //TODO or not TODO - some emergency operations with server?
}

//----------------------------------------------------------------------------------
//
void networkServ::onAnswerCommands(const QVector<CmdParts> & responses)
{
    for (CmdParts const & answer : responses)
    {
        QTcpSocket * sock = answer.sock;
        sock->write(answer.rezult.toLocal8Bit());
        //TODO: send responces to set_/OK commands to ALL connected clients
        // (all clients need to know current/changed state)
    }
}
//----------------------------------------------------------------------------------
