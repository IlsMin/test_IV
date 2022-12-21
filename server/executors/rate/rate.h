#ifndef RATE_H
#define RATE_H

#include <QObject>
#include <QTimer>
#include <QtWidgets/QWidget>

#include "rate_global.h"
#include "executorInterface.h"
#include "common_parts.h"


class RATE_EXPORT rate:  public TestIV_ExecutorInterface
{
        Q_OBJECT
        Q_PLUGIN_METADATA(IID T_IV_PLUGIN)
        Q_INTERFACES(TestIV_ExecutorInterface)


        QTimer timer;
        bool show = false;

    public:
         rate();
         ~rate() { timer.stop(); }
    protected:
        bool checkCommandArgs(const CmdParts & cmnd) override;
        void doSetter(const CmdParts & cmnd)  override;
        void doGetter(CmdParts & cmnd)  override;
        void init() override;
private slots:
        void onTimer();
};

#endif // RATE_H
