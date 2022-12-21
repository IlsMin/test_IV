
#include <QObject>
#include <QLabel>
#include <QThread>
#include <QTcpSocket>
#include <QDir>
#include "QtCore/qdebug.h"
#include <QPluginLoader>

#include "networkserv.h"
#include "mainwindow.h"
#include "executorInterface.h"
#include "./ui_mainwindow.h"

//----------------------------------------------------------------------------------
//
MainWindow::MainWindow(quint16 portN, QWidget * parent)
    : QMainWindow(parent)
    , port (portN)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->lcdNumber);

    QLabel * lbl_Port = new QLabel();
    lbl_Port->setText(QString("Port: %1  ").arg(portN));
    ui->statusbar->addPermanentWidget(lbl_Port);
}

//----------------------------------------------------------------------------------
//
MainWindow::~MainWindow()
{
  //  finishThread();
    delete ui;
}

//----------------------------------------------------------------------------------
//
bool MainWindow::initServer(quint16 portN)
{
    QString msg;
    netServer.reset(new networkServ());// whithout parent!
    if (!netServer->initServer(portN))
    {
        msg = QString("Sever not inited. %1").arg(netServer.isNull() ?
                "netServer not created" : netServer->getLastError());
        fixState(msg);
        return false;
    }
    fixState("Sever inited.");
    servThread = new QThread(); //check result!
    netServer->moveToThread(servThread);
    bool rez (false);
    {
        // servThread start/finish
        QAtomicInt serverStarted;
        // *INDENT-OFF*
        if (!(  connect(servThread, &QThread::started,  [&]() { serverStarted = +1;}) &&
              connect(servThread, &QThread::finished, [&]() { serverStarted = -1; /*netServer->finishAll();*/})
            ))  return false;
        // *INDENT-ON*

        servThread->start();

        // wait server thread started or finished!
        // servThread->waitForStarted(?
        while (0 == serverStarted)
        {
            QThread::msleep(50);
            QCoreApplication::processEvents();
        }
        rez =  serverStarted > 0;
    }
    fixState(QString("Sever thread %1 started.").arg(rez ? "" : "NOT"));

    if (!rez)
        finishThread();

    return  rez;
}

//----------------------------------------------------------------------------------
//
void MainWindow::fixState(const QString & msg)
{
    //   ui->statusbar->setToolTip(msg);
    ui->statusbar->setStatusTip(msg);
    //TODO: write to log - LOGs not included into this project!
    qDebug() << msg;
}

void MainWindow::parseNewCommands(QTcpSocket * sock, const QStringList & cmnds)
{
    QVector<CmdParts> badCommands;
    for (const auto & cmd : cmnds)
    {
        CmdParts cmdPart(sock);
        if (cmdPart.parse(cmd) &&
            executors.contains(cmdPart.name) &&
            checkEquipmentState(cmdPart)    )
            commandsToDo.append(cmdPart);
        else
        {
            if (!executors.contains(cmdPart.name))
                cmdPart.rezult += " no executor";
            badCommands.append(cmdPart);

        }
    }
    if (!badCommands.isEmpty())
        emit answerCommands(badCommands);

}

 //----------------------------------------------------------------------------------
 //
 bool MainWindow::checkEquipmentState(CmdParts & cmdPart)
 {
     if (TestIV_ExecutorInterface::getEquipState())
         return true;   // state is On

     if (TestIV_ExecutorInterface::ifTurnOnCmd(cmdPart))
         return true;  // trying to set state into On

     cmdPart.rezult = T_FAILID + "equipment is switched off";
     return false;
 }

//----------------------------------------------------------------------------------
//
bool MainWindow::initAll()
{
    if (!initServer(port))
        return false;
    networkServ * servPtr = netServer.data();

    // network -> main thread signals
    connect(servPtr, &networkServ::stateMessage, this, &MainWindow::fixState );
    connect(servPtr, &networkServ::newCommandsArived, this, &MainWindow::parseNewCommands);

    // main -> network  thread signals
    connect(this, &MainWindow::answerCommands, servPtr, &networkServ::onAnswerCommands);
    //    connect(this, &MainWindow::, servPtr,  &networkServ::);

    if (!initExecutors())
        return false;
    return true;
}
//----------------------------------------------------------------------------------
//
void MainWindow::finishThread()
{
    if (servThread != nullptr)
    {
        qDebug() << "closing server thread";
        servThread->quit();
        servThread->wait();
        if (!netServer.isNull())
            netServer.reset();

        qDebug() << "deleting server thread";
        delete servThread;
        servThread = nullptr;
    }
}
//----------------------------------------------------------------------------------
//
bool MainWindow::initExecutors()
{
    /* Name sheme used here:
            appdir/executors/method_name/method_name.so
        1. app have 'executors' subdir
        2. each executor file must be located in own subdir (with the same name)
             - so each plugin can save any additional files in own directory
        3. we check all 'executors' subdirs, so if some new/unregistred plugin will be found,
           it will be 'registred' in common map.
    */

    QString pluginPath = QApplication::applicationDirPath() + QDir::separator()
                         + T_EXECUTORS + QDir::separator();
    QDir dir(pluginPath);
    if (!dir.exists())
        dir.mkpath(pluginPath); // or return error ?
    QString plug_pref, plug_ext;

#if _WIN64 || _WIN32
    plug_ext = ".dll";
#else
    plug_ext = ".so";  // don't forget to rename plugin if your OS/file have another extension ;)
    plug_pref = "lib";
#endif

    QPluginLoader  loader(this);
    for (auto const & subdir : dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
    {
        QString fullName = pluginPath + subdir + QDir::separator() +plug_pref+ subdir + plug_ext;
        if (QFile::exists(fullName))
        {
            loader.setFileName(fullName);
            if (!loader.load())
                qWarning() << "unloaded plugin " << fullName << loader.errorString();
            else
            {
                TestIV_ExecutorInterface * interf = qobject_cast<TestIV_ExecutorInterface *>(loader.instance());
                if (nullptr == interf)
                    qDebug () << "not app plugin" << fullName;
                else
                    executors[subdir] = interf;
            }
        }
    }
    qDebug() << "loaded executors" << executors.keys();
    return true;
}
