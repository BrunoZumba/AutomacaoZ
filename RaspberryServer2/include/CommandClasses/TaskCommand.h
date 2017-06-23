#ifndef COMMANDTASK_H
#define COMMANDTASK_H


#include "Command.h"
#include "TaskClass.h"

using namespace std;


class TaskCommand : public Command {
    public:
        TaskCommand();
        virtual ~TaskCommand();

        bool execute();

        TaskClass getTask();
        void setTask(TaskClass);


    protected:


    private:
        TaskClass task;


};

#endif // COMMANDTASK_H
