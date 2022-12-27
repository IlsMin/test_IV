#include <QLCDNumber>
#include "state.h"
#include "common_parts.h"


bool State::checkCommandArgs(CmdParts & cmnd)
{
    bool rez = TestIV_ExecutorInterface::checkCommandArgs(cmnd);
    if (rez)
    {
        if (cmnd.isSetter)
        {
            rez = LED_STATES.contains(cmnd.arg.toString(), Qt::CaseInsensitive);
            if (!rez)
                cmnd.rezult = T_FAILID + "wrong state";
        }
    }
    return rez;
}

void State::doSetter(const CmdParts & cmnd)
{
    equipmentState = isStateOn(cmnd);   // on : off
    qDebug() << "state" << cmnd.arg << equipmentState << &equipmentState;
    QLCDNumber * numb = qobject_cast<QLCDNumber *>(pluginWidget);
    if (numb != nullptr)
        numb->setSegmentStyle( equipmentState ? QLCDNumber::Filled : QLCDNumber::Outline);
    else if ( pluginWidget != nullptr)
        pluginWidget->setVisible(equipmentState);
}

void State::doGetter(CmdParts & cmnd)
{
    cmnd.arg = equipmentState ? "on" : "off";
}

