#include "clientsocket.h"
#include <QStandardPaths>

clientSocket::clientSocket(QObject * parent)
    : QObject{parent}
{
    tcpSocket.reset(new QTcpSocket(this));
    initSignals();
    qDebug() << QStandardPaths::CacheLocation;
}
//
//----------------------------------------------------------------------------------------------------------
void clientSocket::init(const QString & ip, quint16 port)
{
    QTcpSocket * ptr = tcpSocket.data();
    ptr->abort();
    ptr->connectToHost(ip, port) ;
    qDebug() << ip << port << ptr->isOpen();
}

void clientSocket::sendToServer(const QString & data)
{
    if (!sendCmd(data))
        onErrorOccurred();
}
//----------------------------------------------------------------------------------------------------------
void clientSocket::initSignals()
{

    Q_ASSERT(!tcpSocket.isNull());
    QTcpSocket * ptr = tcpSocket.data();

    connect(ptr, &QTcpSocket::readyRead,          this, &clientSocket::readyRead);
    connect(ptr, &QAbstractSocket::errorOccurred, this, &clientSocket::onErrorOccurred);
    connect(ptr, &QTcpSocket::stateChanged,       this, &clientSocket::stateChanged);
}
//----------------------------------------------------------------------------------------------------------
//
void clientSocket::onErrorOccurred()
{
    Q_ASSERT(!tcpSocket.isNull());
    QTcpSocket * ptr = tcpSocket.data();

    //emit socketStateError(ptr->state(), ptr->errorString());
    Q_ASSERT( qmlObject != nullptr);
    QMetaObject::invokeMethod(qmlObject, "onSocketError",
                              Q_ARG(QString, ptr->errorString()));

}
//----------------------------------------------------------------------------------------------------------
//
void clientSocket::stateChanged()
{
    QTcpSocket * socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket)
        qCritical() << "Not defined socket's state changed";
    else
    {
        int state = socket->state();
        qDebug() << "socket state:" << state;
        switch (state)
        {
            case QAbstractSocket::ConnectedState:
            case QAbstractSocket::ClosingState:
                qDebug() <<  "connected" << ((state == QAbstractSocket::ClosingState) ? "No" : "Yes");
                break;
                //case QAbstractSocket::

        }
        // emit socketStateChanged(socket->state(), "");
        Q_ASSERT( qmlObject != nullptr);
        QMetaObject::invokeMethod(qmlObject, "onSocketState",
                                  Q_ARG(QString, statetoString(socket->state())),
                                  Q_ARG(int, state)
                                 );

    }

}
//----------------------------------------------------------------------------------------------------------
//
QString  clientSocket::statetoString(QAbstractSocket::SocketState socketState)
{
    QString statestring;
    switch (socketState)
    {
        case QAbstractSocket::UnconnectedState :
            statestring = "the socket is not connected";
            break;
        case QAbstractSocket::HostLookupState :
            statestring = "the socket is performing a host name lookup";
            break;
        case QAbstractSocket::ConnectingState :
            statestring = "the socket has started establishing a connection";
            break;
        case QAbstractSocket::ConnectedState :
            statestring = "a connection is established";
            break;
        case QAbstractSocket::BoundState :
            statestring = "the socket is bound to an address and port";
            break;
        case QAbstractSocket::ClosingState :
            statestring = "the socket is about to close";
            break;
        case QAbstractSocket::ListeningState :
            statestring = "listening state";
            break;
        default:
            statestring = "unknown state";
    }
    return  statestring;
}
//----------------------------------------------------------------------------------------------------------
//
void clientSocket::readyRead()
{
    QTcpSocket * socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket)
    {
        qCritical() << "Not defined socket's state changed";
        return;
    }

    // char buf[2048];
    while (socket->bytesAvailable())
    {

        QByteArray ba = socket->readAll();
        //    int len = socket->readLine(buf + bufOffset, sizeof(buf));

        QString data(ba);
        //data = data.mid(0, data.size()-2);
        qDebug() << "read:" << data;

        //  emit dataFromServer(data);

        // int state = socket->state();
        Q_ASSERT( qmlObject != nullptr);
        QMetaObject::invokeMethod(qmlObject, "onSocketState",
                                  Q_ARG(QString, data),
                                  Q_ARG(int, socket->state())
                                 );


    }
}

//----------------------------------------------------------------------------------------------------------
//
bool clientSocket::sendCmd(const QString & data)
{
    QTcpSocket * socket = qobject_cast<QTcpSocket *>(tcpSocket.data());
    if (!socket)
    {
        qCritical() << "Not defined socket!";
        return false;
    }

    if (QAbstractSocket::ConnectedState == socket->state())
    {
        socket->write(data.toLocal8Bit());
        qDebug() << "sent:" << data;
        return  true;
    }
    else
    {
        qDebug() << "not sent, socket state:" << socket->state();
        return  false;
    }
}
