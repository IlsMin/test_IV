#pragma once

#include <QtWidgets/QWidget>
#include <QtPlugin>

#include "common_parts.h"

// namespace ?

#define T_IV_PLUGIN "Test IV Plugin ver 0.0.1"


class TestIV_ExecutorInterface : public QObject
{
        Q_OBJECT
    protected:

        QWidget * pluginWidget  = nullptr;
        static inline bool state= false;
        //QVariant lastValidArg;


        virtual void doSetter(const CmdParts & cmnd) = 0;
        virtual void doGetter(CmdParts & cmnd) = 0;
        virtual bool checkCommandArgs(const CmdParts &/* cmnd*/)
        {
            return pluginWidget != nullptr;
        }

        virtual void init() {}  // for own needs
        virtual ~TestIV_ExecutorInterface() {}

    public:
        static bool getEquipState() {return state; }
        static bool ifTurnOnCmd(const CmdParts & cmdPart)
        { return T_STATE == cmdPart.name && cmdPart.arg.toBool(); }

        virtual void setWidget(QWidget * value)  final
        {
            pluginWidget = value;
            init();
        }

        // virtual QString getPluginName() = 0;
        virtual void execute(CmdParts & cmd) final
        {
            if (checkCommandArgs(cmd))
            {

                if (cmd.isSetter)
                {
                    doSetter(cmd);
                    if( T_STATE == cmd.name &&
                        cmd.rezult.startsWith(T_OK)
                       ) state = cmd.arg.toBool();
                }
                else doGetter(cmd);
            }
        };
        // public slots:
        //   virtual void beforeDestroing() {}

};
Q_DECLARE_INTERFACE(TestIV_ExecutorInterface, T_IV_PLUGIN)
