#ifndef COMMANDTASK_H
#define COMMANDTASK_H


#include "Task.h"
#include "AppClasses/TaskClass.h"

using namespace std;

#define STATUS_OK 1
#define STATUS_ERROR 0

class CommandTask : public Task {
    public:
        CommandTask();
        virtual ~CommandTask();

        bool execute();

        TaskClass getTask();
        void setTask(TaskClass);


    protected:


    private:
        TaskClass task;


};

#endif // COMMANDTASK_H
