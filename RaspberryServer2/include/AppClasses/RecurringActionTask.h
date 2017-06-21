#ifndef RECURRINGACTIONTASK_H
#define RECURRINGACTIONTASK_H

#include "ActionButtonClass.h"
#include <vector>

using namespace std;


class RecurringActionTask {
    public:
        RecurringActionTask();
        RecurringActionTask(string, ActionButtonClass, vector<long>, vector<long>);
        virtual ~RecurringActionTask();


        string getName();
        ActionButtonClass getActionButton();
        vector<long> getDates();
        vector<long> getTimes();

        void setName(string);
        void setActionButton(ActionButtonClass);
        void setDates(vector<long>);
        void setTimes(vector<long>);

        bool createFromJson(string);
        string parseToJson();

        //Queues nao foram declaradas privadas pq dava problema no queue.pop();
        vector<long> times;
        ActionButtonClass actionButton;
        vector<long> dates;

    protected:


    private:
        string name;
};

#endif // RECURRINGACTIONTASK_H
