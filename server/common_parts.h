#pragma once

#include "QtCore/qstringlist.h"
#include "QtCore/qdebug.h"
#include <QVariant>
#include <QStringLiteral>


class QTcpSocket;

// it's possible to read all these constants from some configuration file - for protocol tuning...
//commands
const inline QString T_SET_LED_  = QStringLiteral("set-led-");
const inline QString T_GET_LED_  = QStringLiteral("get-led-");
const inline QString T_EXECUTORS = QStringLiteral("executors");
const inline QString T_STATE     = QStringLiteral("state");
const inline QString END_COMMAND = QStringLiteral("\n");

const inline int CMD_PREFIX_LEN  = T_GET_LED_.length();

/*
// arguments
enum CommandTypesEnum
{
    ect_STATE, ect_COLOR, ect_RATE
};
const inline QStringList CMD_TYPES  = {"state", "color", "rate"};
*/

const inline QStringList LED_COLORS = {"red", "green", "blue"};
const inline QStringList LED_STATES = {"on", "off"};
// fregs -?

const inline QString T_rate    =  QStringLiteral("rate");

// results
const inline QString T_OK      =  QStringLiteral("OK ");
const inline QString T_FAILID  =  QStringLiteral("FAILED. ");
const inline QString T_BAD_CMD = QStringLiteral("Bad command type");


struct CmdParts
{
    QTcpSocket * sock = nullptr;
    //CommandTypesEnum type
    QString name;
    bool isSetter;
    QVariant arg;    // is protocol allows "FAILED reason" answer ?!?
    QString  rezult;
    CmdParts() = default;
    CmdParts(QTcpSocket * s)
    {
        sock = s;
    }
    QString getString()
    {
        return QString("is setter:%1, name:%2, arg:%3, result: %4").arg(isSetter).arg(name).arg(
                   arg.toString()).arg(rezult);
    }
    bool parse(const QString cmd)
    {
        isSetter = cmd.startsWith(T_SET_LED_);
        if (!isSetter &&
            !cmd.startsWith(T_GET_LED_))
        {
            rezult = T_FAILID + T_BAD_CMD;
            return false;
        }

        if (isSetter)
        {
            arg = cmd.section(' ', 1, 1).trimmed();
            if (arg.isNull())
            {
                rezult = T_FAILID + "no argument";
                return false;
            }
            //else  - we not check arguments here - executers will do that
        }

        int len = cmd.length();
        int pos = cmd.indexOf(' ');
        if (pos == -1)
            pos = len;

        // !!! here is some limitation - length of command prefix !!!
        // - in common case we need to use some Regular expression
        name = cmd.mid(CMD_PREFIX_LEN, pos - CMD_PREFIX_LEN).trimmed();
        qDebug() << cmd << ",  parsed to: " << getString();
        return true;
    }
};
