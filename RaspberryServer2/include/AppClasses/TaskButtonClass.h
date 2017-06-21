#ifndef TASKBUTTONCLASS_H
#define TASKBUTTONCLASS_H

#include "TaskClass.h"

class TaskButtonClass
{
    public:
        TaskButtonClass();
        virtual ~TaskButtonClass();

        string getButtonId();
        void setButtonId(string);
        TaskClass getTask();
        void setTask(TaskClass);

        string parseToJson();
        bool createFromJson(string);

    protected:

    private:
        string buttonId;
        TaskClass task;
};

#endif // TASKBUTTONCLASS_H
