#ifndef TASK_H
#define TASK_H

#include<string>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>

#include "ArduinoJson.h"
#include "Util.h"

using namespace std;


#define STATUS_OK 1
#define STATUS_ERROR 0

class Command{
    public:
        Command(string);
        virtual ~Command();


		int getResponseStatus();
		string getResponseDesc();
		string getResponseAction();
		string getResponseParm();
		void setResponseStatus(int);
		void setResponseDesc(string);
		void setResponseAction(string);
		void setResponseParm(string);

		string getRequestAction();
        void setRequestAction(string);
        bool getRequestOverwrite();
        void setRequestOverwrite(bool);

		void createResponse(int, /*string,*/ string, string);

        string ParseResponseToJason();

//        virtual bool execute() = 0;//Subclass must implement
        virtual bool execute() = 0;//Subclass must implement
        virtual bool createRequestFromJson();

    protected:
        string requestAction;
        bool requestOverwrite;

		int responseStatus;
        string responseAction;
		string responseDesc;
		string responseParm;

		//Usado para fazer o parse
		DynamicJsonBuffer rootJsonBuffer;
        JsonObject& jsonRoot ;

    private:
};

#endif // TASK_H
