#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>


#include "clientsocket.h"

int main(int argc, char * argv[])
{
    QGuiApplication app(argc, argv);

    //  QQmlApplicationEngine engine;
    QQmlEngine engine;
    clientSocket socket;

    QQmlContext * context = new QQmlContext(engine.rootContext());
    context->setContextProperty("clientSocket", &socket);
    //  qDebug() << context->contextProperty("clientSocket");

    QQmlComponent component(&engine);
    // QQuickWindow::setDefaultAlphaBuffer(true);
    component.loadUrl(QUrl("qrc:/client_IV/main.qml"));
    if ( component.isReady() )
    {
        QObject * obj = component.create(context);
        socket.setQmlObject(obj);
    }
    else
        qWarning() << component.errorString();

    return app.exec();
}
