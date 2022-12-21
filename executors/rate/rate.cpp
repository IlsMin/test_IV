#include "rate.h"
//Q_EXPORT_PLUGIN2(rate, TestIV_ExecutorInterface);

rate::rate()
{
}

bool rate::checkCommandArgs(const CmdParts & cmnd)
{
    bool rez = TestIV_ExecutorInterface::checkCommandArgs(cmnd);
    if (rez)
    {
        int herz = cmnd.arg.toInt();
        rez = herz >= 0 && herz < 6;
    }
    return rez;
}

void rate::doSetter(const CmdParts &cmnd)
{
    timer.stop();
   int herz = cmnd.arg.toInt();
   if(herz > 0)
   {
        timer.setInterval(1000/herz);
        timer.start();
   }
}

void rate::doGetter(CmdParts &cmnd)
{
    cmnd.rezult = QString::number(1000/timer.interval());
}

void rate::init()
{
    timer.setInterval(10010); // more that 1000 ->gettter will return 0
    connect(&timer, &QTimer::timeout, this ,&rate::onTimer);
 }

void rate::onTimer()
{

   qDebug() <<"timer!";
   show = !show;
   pluginWidget->setVisible(show);
}
