#ifndef RECURRINGACTIONTASK_H
#define RECURRINGACTIONTASK_H

#include "Task.h"
#include "CommandTask.h"
#include <queue>

#define STATUS_OK 1
#define STATUS_ERROR 0

using namespace std;


class RecurringActionTask : public Task {
    public:
        RecurringActionTask();
        RecurringActionTask(string, queue<CommandTask>, queue<long>, queue<long>);
        virtual ~RecurringActionTask();


        string getName();
        queue<CommandTask> getCommandTasks();
        queue<long> getDates();
        queue<long> getTimes();

        void setName(string);
        void setCommandTasks(queue<CommandTask>);
        void setDates(queue<long>);
        void setTimes(queue<long>);

        bool ParseRequestFromJason(string);
        bool execute();

        //Queues nao foram declaradas privadas pq dava problema no queue.pop();
        queue<long> times;
        queue<CommandTask> commandTasks;
        queue<long> dates;

    protected:


    private:
        string name;
};

#endif // RECURRINGACTIONTASK_H
