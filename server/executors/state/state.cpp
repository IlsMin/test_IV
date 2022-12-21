#include <QLCDNumber>
#include "state.h"
#include "common_parts.h"


bool State::checkCommandArgs(const CmdParts &cmnd)
{
    bool rez = TestIV_ExecutorInterface::checkCommandArgs(cmnd);
    if (rez)
        rez = LED_STATES.contains(cmnd.arg.toString(), Qt::CaseInsensitive);
    return rez;
}

void State::doSetter(const CmdParts &cmnd)
{
    state = cmnd.arg.toBool();
    QLCDNumber * numb = qobject_cast<QLCDNumber *>(pluginWidget);
    if(numb != nullptr)
       numb->setSegmentStyle( state ? QLCDNumber::Filled : QLCDNumber::Outline);
    else
     //   if( pluginWidget != nullptr)
            pluginWidget->setVisible(state);
}

void State::doGetter(CmdParts &cmnd)
{
    cmnd.rezult = state;
}
