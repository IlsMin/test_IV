#pragma once

#include <QtWidgets/QWidget>
#include <QtPlugin>

#include "common_parts.h"

// namespace ?

#define T_IV_PLUGIN "Test IV Plugin ver 0.0.1"

class State;
class TestIV_ExecutorInterface : public QObject
{
        Q_OBJECT
    protected:

        QWidget * pluginWidget  = nullptr;
        inline static bool equipmentState = false;


        virtual ~TestIV_ExecutorInterface() {}
        bool isStateOn(const CmdParts & cmdPart)
        {
            return isStateOn(cmdPart.arg.toString());
        }
        bool isStateOn(const QString & cmdArg)
        {
            return cmdArg.length() == 2 && cmdArg.at(1) == 'n';   // on : off
        }


    public:
        virtual bool checkCommandArgs( CmdParts & cmnd)
        {
            bool rez = pluginWidget != nullptr;
            if (!rez)
                cmnd.rezult = T_FAILID + "The equipment is not initialized";
            else
            {
                if (!equipmentState)    // need to turn on before!
                {
                    qDebug() << "state" << equipmentState << &equipmentState;

                    if ( cmnd.name != T_STATE &&
                         !isStateOn(cmnd)
                       )
                    {
                        cmnd.rezult = T_FAILID + "equipment state is turned off";
                        rez = false;
                    }
                }
            }
            return rez;
        }
        virtual void init() /*{}*/   = 0; // for executer needs
        virtual void doSetter(const CmdParts & cmnd) = 0;
        virtual void doGetter(CmdParts & cmnd) = 0;

        virtual void setWidget(QWidget * value)  final
        {
            pluginWidget = value;
            //init();
        }

        // virtual QString getPluginName() = 0;

};
Q_DECLARE_INTERFACE(TestIV_ExecutorInterface, T_IV_PLUGIN)
