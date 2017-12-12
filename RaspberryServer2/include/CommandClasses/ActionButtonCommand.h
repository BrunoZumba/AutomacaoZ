#ifndef ACTIONBUTTONCOMMAND_H
#define ACTIONBUTTONCOMMAND_H

#include<string>
#include<fstream>
#include<iostream>
#include<unistd.h>

#include "Command.h"
#include "ActionButtonClass.h"
#include "ArduinoJson.h"
#include "TaskCommand.h"
#include "Util.h"
#include "ActionButtonDAO.h"


using namespace std;


class ActionButtonCommand : public Command{
    public:
        ActionButtonCommand(string json);
        virtual ~ActionButtonCommand();

        bool execute();
        bool createRequestFromJson();

        ActionButtonClass getActionButton();
        void setActionButton(ActionButtonClass);


    protected:

    private:
        ActionButtonClass actionButton;
};

#endif // ACTIONBUTTONCOMMAND_H
