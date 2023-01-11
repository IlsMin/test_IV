#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
class clientSocket : public QObject
{
        Q_OBJECT
    private:
        QSharedPointer<QTcpSocket> tcpSocket;
        QObject * qmlObject = nullptr;
        void initSignals();

    public:
        explicit clientSocket(QObject * parent = nullptr);
        void setQmlObject(QObject * obj)
        {
            qmlObject = obj;
            Q_ASSERT( qmlObject != nullptr);
        }
    protected:
        bool sendCmd(const QString & data);

        QString statetoString(QAbstractSocket::SocketState socketState);
    protected slots:
        void readyRead();
        void stateChanged();
        void onErrorOccurred();

    public slots:

        void init(const QString & ip, quint16 port);
        void sendToServer(const QString & data);

    signals:
        void dataFromServer(const QString & data);
        void socketStateChanged(QAbstractSocket::SocketState, const QString & mes);
        void socketStateError(  QAbstractSocket::SocketState, const QString & mes);
};

#endif // CLIENTSOCKET_H
