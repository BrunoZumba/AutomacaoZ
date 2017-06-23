#ifndef ACTIONBUTTONCOMMAND_H
#define ACTIONBUTTONCOMMAND_H

#include<string>
#include<fstream>
#include<iostream>

#include "Command.h"
#include "ActionButtonClass.h"
#include "ArduinoJson.h"

using namespace std;

#define ACTION_BUTTON_FILE "actionButtonFile"

class ActionButtonCommand : public Command{
    public:
        ActionButtonCommand();
        virtual ~ActionButtonCommand();

        bool execute();
        bool createFromJson(string);

        ActionButtonClass getActionButton();
        void setActionButton(ActionButtonClass);

        string getActionType();
        void setActionType(string);
        bool getOverwrite();
        void setOverwrite(bool);

    protected:

    private:
        string actionType;
        bool overwrite;
        ActionButtonClass actionButton;
};

#endif // ACTIONBUTTONCOMMAND_H
