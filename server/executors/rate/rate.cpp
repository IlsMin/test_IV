#include <QLCDNumber>
#include "rate.h"
//Q_EXPORT_PLUGIN2(rate, TestIV_ExecutorInterface);

rate::rate()
{
}

bool rate::checkCommandArgs(CmdParts & cmnd)
{
    bool rez = TestIV_ExecutorInterface::checkCommandArgs(cmnd);
    if (rez)
    {
        int herz = cmnd.arg.toInt();
        rez = herz >= 0 && herz < 6;
        if(!rez)
        {
            cmnd.rezult = T_FAILID+ "bad rate";
        }
    }
    return rez;
}

void rate::doSetter(const CmdParts &cmnd)
{
    timer.stop();
    QLCDNumber * numb = qobject_cast<QLCDNumber *>(pluginWidget);
    if(nullptr == numb)
    {
       qCritical() << "LcdNumber NOT initializes!!!";
       return;
    }

   int herz = cmnd.arg.toInt();
   if(herz >= 0)
   {
        numb->display(herz);

        if(herz > 0) {
            timer.setInterval(1000/herz);
            timer.start();
        }
       // else //to emulate state = off OR ?

   }
}

void rate::doGetter(CmdParts &cmnd)
{
    QLCDNumber * numb = qobject_cast<QLCDNumber *>(pluginWidget);
    cmnd.rezult = (numb != nullptr) ? numb->intValue(): 0;
}

void rate::init()
{
    connect(&timer, &QTimer::timeout, this ,&rate::onTimer);
}

void rate::onTimer()
{
   //qDebug() <<"timer!";
   show = !show;
   pluginWidget->setVisible(show);
}
