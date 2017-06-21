#ifndef TASKCLASS_H
#define TASKCLASS_H

#include<string>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>

#include "ArduinoJson.h"

using namespace std;


class TaskClass
{
    public:
        TaskClass(string, string, string);
        TaskClass();
        virtual ~TaskClass();

        string getDeviceName();
        string getButtonName();
        string getMode();
        void setDeviceName(string);
        void setButtonName(string);
        void setMode(string);

        string parseToJson();
        bool createFromJson(string);

    protected:

    private:
        string deviceName;
        string buttonName;
        string mode;
};

#endif // TASKCLASS_H
