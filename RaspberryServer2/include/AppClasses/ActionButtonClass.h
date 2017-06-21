#ifndef ACTIONBUTTONCLASS_H
#define ACTIONBUTTONCLASS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ActionClass.h"

class ActionButtonClass
{
    public:
        ActionButtonClass();
        virtual ~ActionButtonClass();

        string getActionName();
        void setActionName(string);
        ActionClass getAction();
        void setActionName(ActionClass);

        string parseToJson();
        bool createFromJson(string);

    protected:

    private:
        string actionName;
        ActionClass action;
};

#endif // ACTIONBUTTONCLASS_H
