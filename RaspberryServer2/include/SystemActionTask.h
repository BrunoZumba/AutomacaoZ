#ifndef SYSTEMACTIONTASK_H
#define SYSTEMACTIONTASK_H


#include "Task.h"

using namespace std;

#define STATUS_OK 1
#define STATUS_ERROR 0

class SystemActionTask : public Task {
    public:
        SystemActionTask(string);
        SystemActionTask();
        virtual ~SystemActionTask();

        string getActionName();
        void setActionName(string);

        bool ParseRequestFromJason(string);
        //Esse int não é usado. Mas e necessário por causa da super classe Task
        bool execute();

    protected:

    private:
        string actionName;
};

#endif // SYSTEMACTIONTASK_H

