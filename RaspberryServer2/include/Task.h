#ifndef TASK_H
#define TASK_H

#include<string>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>

#include "ArduinoJson.h"
#include "Util.h"

using namespace std;

class Task{
    public:
        Task();
        virtual ~Task();


		int getResponseStatus();
		string getResponseDesc();
		void setResponseStatus(int);
		void setResponseDesc(string);

		void createResponse(int, string);

        string ParseResponseToJason();

//        virtual bool execute() = 0;//Subclass must implement
        virtual bool execute(int) = 0;//Subclass must implement
        virtual bool ParseRequestFromJason(string) = 0; //Subclass must implement

    protected:
		int responseStatus;
		string responseDesc;

    private:
};

#endif // TASK_H
