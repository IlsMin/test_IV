#ifndef COLOR_H
#define COLOR_H

#include <QObject>
#include  <QtWidgets/QWidget>

#include "Color_global.h"
#include "executorInterface.h"
#include "common_parts.h"

class COLOR_EXPORT Color:  public TestIV_ExecutorInterface
{
        Q_OBJECT
        Q_PLUGIN_METADATA(IID T_IV_PLUGIN)
        Q_INTERFACES(TestIV_ExecutorInterface)

    protected:
        // QWidget * pluginWidget  = nullptr;

        bool checkCommandArgs(const CmdParts & cmnd) override;
        void doSetter(const CmdParts & cmnd)  override;
        void doGetter(CmdParts & cmnd)  override;

};

#endif // COLOR_H
