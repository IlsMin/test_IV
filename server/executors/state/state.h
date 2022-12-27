#ifndef STATE_H
#define STATE_H

#include "state_global.h"
#include "../../common_parts.h"
#include "../../executorInterface.h"

class STATE_EXPORT State: public TestIV_ExecutorInterface
{
        Q_OBJECT
        Q_PLUGIN_METADATA(IID T_IV_PLUGIN)
        Q_INTERFACES(TestIV_ExecutorInterface)

    protected:
        bool checkCommandArgs(CmdParts & cmnd) override;
        void doSetter(const CmdParts & cmnd)  override;
        void doGetter(CmdParts & cmnd)  override;
    public:
        void init() override
        {
            qDebug() << "init_equipmentState" << &equipmentState ;
        }

};

#endif // STATE_H
