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


        virtual ~TestIV_ExecutorInterface() {}

    public:
        virtual bool checkCommandArgs( CmdParts & cmnd)
        {
            bool rez = pluginWidget != nullptr;
            if(!rez)
                cmnd.rezult = T_FAILID + "The equipment is not initialized";
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
