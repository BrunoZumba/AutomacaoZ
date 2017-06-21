#ifndef ACTIONCLASS_H
#define ACTIONCLASS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "TaskButtonClass.h"

using namespace std;

class ActionClass
{
    public:
        ActionClass();
        virtual ~ActionClass();

        vector<TaskButtonClass> getActions();
        void setActions(vector<TaskButtonClass>);

        string parseToJson();
        bool createFromJson(string);

    protected:

    private:
        vector<TaskButtonClass> actions;
};

#endif // ACTIONCLASS_H
