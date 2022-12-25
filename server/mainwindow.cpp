
#include <QObject>
#include <QLabel>
#include <QThread>
#include <QTcpSocket>
#include <QDir>
#include "QtCore/qdebug.h"
#include <QPluginLoader>

#include "networkserv.h"
#include "mainwindow.h"
//#include "./executors/state/state.h"
#include "executorInterface.h"
#include "common_parts.h"
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
    //TODO QMainWindow::setWindowIcon(...)


    qRegisterMetaType <QTcpSocket *> ("QTcpSocket*");       // need on MAC only

    qRegisterMetaType <CmdParts> ("CmdParts");
    qRegisterMetaType <QVector<CmdParts>> ("QVector<CmdParts>");

}

//----------------------------------------------------------------------------------
//
MainWindow::~MainWindow()
{
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
    QVector<CmdParts> commandsToDo;
    for (const auto & cmd : cmnds)
    {
        CmdParts cmdPart(sock);
        if (!cmdPart.parse(cmd) ||
            !checkEquipmentState(cmdPart))
        {
            badCommands.append(cmdPart);
            continue;
        }
        if (!executors.contains(cmdPart.name))
        {
            cmdPart.rezult += " No executor for command " + cmdPart.name;
            badCommands.append(cmdPart);
            continue;
        }
        commandsToDo.append(cmdPart);
    }
    for (auto & cmdPart : commandsToDo)
    {
        if (!execCommand(cmdPart))
            badCommands << cmdPart;
        // qDebug() << "write back:" << cmdPart.rezult;
        fixState(cmdPart.rezult); //for fixing in app log
        sock->write(cmdPart.rezult.toLocal8Bit() + "\n");
    }

    if (!badCommands.isEmpty())
        sayAboutBadCommands(badCommands);
}
//----------------------------------------------------------------------------------
//
void MainWindow::sayAboutBadCommands(const QVector<CmdParts> & responses)
{
    for (CmdParts const & answer : responses)
    {
        QTcpSocket * sock = answer.sock;
        sock->write(answer.rezult.toLocal8Bit() + "\n");
        fixState(answer.rezult);
    }
}

//----------------------------------------------------------------------------------
//
bool MainWindow::checkEquipmentState(CmdParts & /*cmdPart*/)
{
    return true;   // state is always On!

    //if (State::getEquipmentState())
    /*    qDebug() << State::equipmentState << & State::equipmentState;
           if ( State::equipmentState )
            return true;   // state is On

        //if (TestIV_ExecutorInterface::ifTurnOnCmd(cmdPart))   // trying to set state = On
        if("state" == cmdPart.name)
            return true;

        cmdPart.rezult = T_FAILID + "equipment is switched off" ;
        return false;
        */
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
    connect(this, &MainWindow::sayAboutBadCommands, servPtr, &networkServ::onAnswerCommands);
    //connect(this, &MainWindow::sendCommand, servPtr,  &networkServ::onSendCommand);

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
    QString appPath = QApplication::applicationDirPath();
#if __APPLE__
    int pos = appPath.indexOf(".app/Contents");
    if (pos > 0)
    {
        if (pos > 3)
            appPath.resize(pos);
        pos = appPath.lastIndexOf('/'); // start od app name
        if (pos > 0)
            appPath.resize(pos);        // use 'phisical' dir path - not Contents
    }
#endif
    QString pluginPath = appPath + QDir::separator()
                         + T_EXECUTORS + QDir::separator();

    QDir dir(pluginPath);
    if (!dir.exists())
        dir.mkpath(pluginPath); // or return error ?
    QString plug_pref, plug_ext;

#if _WIN64 || _WIN32
    plug_ext = ".dll";
#else
    plug_pref = "lib";
#if __APPLE__
    plug_ext = ".dylib";
#else
    plug_ext = ".so";
#endif
    // don't forget to rename plugin if your OS/file have another extension ;)
#endif

    QPluginLoader  loader(this);
    for (auto const & subdir : dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
    {
        QString fullName = pluginPath + subdir + QDir::separator() + plug_pref + subdir + plug_ext;
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
                {
                    interf->setWidget(ui->lcdNumber);
                    interf->init();
                    executors[subdir] = interf;
                }
            }
        }
    }
    qDebug() << "loaded executors" << executors.keys();
    return true;
}
//----------------------------------------------------------------------------------
//
bool MainWindow::execCommand(CmdParts & cmdPart)
{
    TestIV_ExecutorInterface * interf = nullptr;
    if (executors.contains(cmdPart.name))  // must be already done in checkers
        interf = executors[cmdPart.name];
    if (nullptr == interf)
    {
        cmdPart.rezult = T_FAILID + "No executer for command " + cmdPart.name;
        return false;
    }
    if (!interf->checkCommandArgs(cmdPart))
        return false;

    if (cmdPart.isSetter)
        interf->doSetter(cmdPart);
    else interf->doGetter(cmdPart);

    cmdPart.rezult = T_OK + cmdPart.arg.toString();
    return true;
}
