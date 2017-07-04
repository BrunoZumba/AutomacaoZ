#ifndef RecurringActionClass_H
#define RecurringActionClass_H

#include "ActionButtonClass.h"
#include <vector>
#include <sstream>
#include <string>

using namespace std;


class RecurringActionClass {
    public:
        RecurringActionClass();
        RecurringActionClass(string, ActionButtonClass, vector<long long>, vector<long long>);
        virtual ~RecurringActionClass();


        string getRecActName();
        ActionButtonClass getRecActActionButton();
        vector<long long> getRecActDates();
        vector<long long> getRecActTimes();

        void setRecActName(string);
        void setRecActActionButton(ActionButtonClass);
        void setRecActDates(vector<long long>);
        void setRecActTimes(vector<long long>);

        bool createFromJson(string);
        string parseToJson();

        //Queues nao foram declaradas privadas pq dava problema no queue.pop();
        vector<long long> recActTimes;
        ActionButtonClass recActActionButton;
        vector<long long> recActDates;

    protected:


    private:
        string recActName;
};

#endif // RecurringActionClass_H
