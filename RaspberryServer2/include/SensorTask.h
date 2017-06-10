
#ifndef SENSORTASK_H
#define SENSORTASK_H


#include<string>
#include<iostream>
#include "lirc_client.h"
#include "ArduinoJson.h"
#include "Util.h"

using namespace std;

#define STATUS_OK 1
#define STATUS_ERROR 0

class SensorTask
{
    public:
        SensorTask(string, string);
        SensorTask();
        virtual ~SensorTask();

        string getSensorName();
        string getAction();
		int getResponseStatus();
		string getResponseDesc();

        void setSensorName(string);
        void setAction(string);
		void setResponseStatus(int);
		void setResponseDesc(string);

        bool execute();
		void createResponse(int, string);

        string ParseResponseToJason();
        bool ParseRequestFromJason(string);

    protected:


    private:
        string sensorName;
        string action;
		int responseStatus;
		string responseDesc;

};

#endif // SENSORTASK_H
