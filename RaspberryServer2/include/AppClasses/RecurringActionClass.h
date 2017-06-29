#ifndef RecurringActionClass_H
#define RecurringActionClass_H

#include "ActionButtonClass.h"
#include <vector>
#include <sstream>

using namespace std;


class RecurringActionClass {
    public:
        RecurringActionClass();
        RecurringActionClass(string, ActionButtonClass, vector<long>, vector<long>);
        virtual ~RecurringActionClass();


        string getRecActName();
        ActionButtonClass getRecActActionButton();
        vector<long> getRecActDates();
        vector<long> getRecActTimes();

        void setRecActName(string);
        void setRecActActionButton(ActionButtonClass);
        void setRecActDates(vector<long>);
        void setRecActTimes(vector<long>);

        bool createFromJson(string);
        string parseToJson();

        //Queues nao foram declaradas privadas pq dava problema no queue.pop();
        vector<long> recActTimes;
        ActionButtonClass recActActionButton;
        vector<long> recActDates;

    protected:


    private:
        string recActName;
};

#endif // RecurringActionClass_H
