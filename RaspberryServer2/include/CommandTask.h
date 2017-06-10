#ifndef COMMANDTASK_H
#define COMMANDTASK_H


#include<string>
#include<iostream>
#include "lirc_client.h"
#include "ArduinoJson.h"
#include "Util.h"

using namespace std;

#define STATUS_OK 1
#define STATUS_ERROR 0

class CommandTask
{
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
        void setResponseStatus(int);
		void setResponseDesc(string);

        bool execute(int);
		void createResponse(int, string);

        string ParseResponseToJason();
        bool ParseRequestFromJason(string);

    protected:


    private:
        string deviceName;
        string buttonName;
        string mode;
		int responseStatus;
		string responseDesc;
};

#endif // COMMANDTASK_H
