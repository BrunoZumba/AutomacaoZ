#ifndef SYSTEMACTIONTASK_H
#define SYSTEMACTIONTASK_H


#include "Command.h"

using namespace std;

#define STATUS_OK 1
#define STATUS_ERROR 0

class SystemActionCommand : public Command {
    public:
        SystemActionCommand(string);
        SystemActionCommand();
        virtual ~SystemActionCommand();

        string getActionName();
        void setActionName(string);

        bool ParseRequestFromJason(string);
        bool execute();

    protected:

    private:
        string actionName;
};

#endif // SYSTEMACTIONTASK_H

