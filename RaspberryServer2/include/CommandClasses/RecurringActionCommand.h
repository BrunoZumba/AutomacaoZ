#ifndef RECURRINGACTIONCOMMAND_H
#define RECURRINGACTIONCOMMAND_H

#include <iostream>
#include <fstream>
#include "Command.h"
#include "RecurringActionClass.h"

#define RECURRING_ACTION_FILE "recurringActionFile"

class RecurringActionCommand : public Command {
    public:
        RecurringActionCommand(string json);
        virtual ~RecurringActionCommand();

        bool execute();
        bool createRequestFromJson();

        RecurringActionClass getRecurringAction();
        void setRecurringAction(RecurringActionClass);



    protected:

    private:

        RecurringActionClass recurringAction;
};

#endif // RECURRINGACTIONCOMMAND_H
