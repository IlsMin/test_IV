#include  <QLCDNumber>
#include "color.h"


bool Color::checkCommandArgs(CmdParts & cmnd)
{
    bool rez = TestIV_ExecutorInterface::checkCommandArgs(cmnd);
    if ( rez)
    {
        rez = LED_COLORS.contains(cmnd.arg.toString(), Qt::CaseInsensitive);
        if (!rez)
            cmnd.rezult = T_FAILID + "wrong color";
    }
    return rez;
}

void Color::doSetter(const CmdParts & cmnd)
{
    Q_ASSERT(pluginWidget != nullptr);
    pluginWidget->setStyleSheet("color: " + cmnd.arg.toString());
    QLCDNumber * numb = qobject_cast<QLCDNumber *>(pluginWidget);
    if (numb != nullptr)
        numb->setSegmentStyle(QLCDNumber::Filled);
}

void Color::doGetter(CmdParts & cmnd)
{
    Q_ASSERT(pluginWidget != nullptr);

    QPalette palette = pluginWidget->palette();
    QColor clr =  palette.color(palette.WindowText);
    cmnd.arg = clr.name();  //  real color or sent value?
}
