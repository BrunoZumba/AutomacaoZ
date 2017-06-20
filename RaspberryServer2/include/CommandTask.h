#ifndef COMMANDTASK_H
#define COMMANDTASK_H


#include "Task.h"

using namespace std;

#define STATUS_OK 1
#define STATUS_ERROR 0

class CommandTask : public Task {
    public:
        CommandTask(string, string, string);
        CommandTask();
        virtual ~CommandTask();

        string getDeviceName();
        string getButtonName();
        string getMode();
        void setDeviceName(string);
        void setButtonName(string);
        void setMode(string);

        bool ParseRequestFromJason(string);
        bool execute();


    protected:


    private:
        string deviceName;
        string buttonName;
        string mode;

};

#endif // COMMANDTASK_H
