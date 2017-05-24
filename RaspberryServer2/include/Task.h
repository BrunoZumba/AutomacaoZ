#ifndef TASK_H
#define TASK_H


#include<string>
#include<iostream>
#include "lirc_client.h"
#include "ArduinoJson.h"
#include "Util.h"

using namespace std;

class Task
{
    public:
        Task(string, string, string);
        Task();
        virtual ~Task();

        string getDeviceName();
        string getButtonName();
        string getMode();

        void setDeviceName(string);
        void setButtonName(string);
        void setMode(string);

        bool execute(int);

        string ParseToJason();
        bool ParseFromJason(string);

    protected:


    private:
        string deviceName;
        string buttonName;
        string mode;
};

#endif // TASK_H
