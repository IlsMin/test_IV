#include "color.h"


bool Color::checkCommandArgs(const CmdParts &cmnd)
{
    bool rez = TestIV_ExecutorInterface::checkCommandArgs(cmnd);
    if (rez)
        rez = LED_COLORS.contains(cmnd.arg.toString(), Qt::CaseInsensitive);
    return rez;
}

void Color::doSetter(const CmdParts &cmnd)
{
   Q_ASSERT(pluginWidget != nullptr);
   pluginWidget->setStyleSheet("color: " + cmnd.arg.toString());
}

void Color::doGetter(CmdParts &cmnd)
{
    Q_ASSERT(pluginWidget != nullptr);

    QPalette palette = pluginWidget->palette();
    QColor clr =  palette.color(palette.WindowText);
    cmnd.rezult = clr.name(); //  real color or sent value?
}
