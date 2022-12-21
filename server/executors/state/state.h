#ifndef STATE_H
#define STATE_H

#include "state_global.h"
#include "executorInterface.h"

class STATE_EXPORT State: public TestIV_ExecutorInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID T_IV_PLUGIN)
    Q_INTERFACES(TestIV_ExecutorInterface)


public:
   // State();
protected:
    bool checkCommandArgs(const CmdParts & cmnd) override;
    void doSetter(const CmdParts & cmnd)  override;
    void doGetter(CmdParts & cmnd)  override;
    //void init() override;
};

#endif // STATE_H
