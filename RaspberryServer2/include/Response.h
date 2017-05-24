#ifndef RESPONSE_H
#define RESPONSE_H

#include<string>
#include<iostream>
#include<string.h>
#include "ArduinoJson.h"
#include "Util.h"

#define STATUS_OK 1
#define STATUS_ERROR 0

using namespace std;

class Response
{
    public:
        Response(int, string);
        Response(string);
        Response();
        virtual ~Response();

        void create(int, string);

        int getStatus();
        string getDesc();

        void setStatus(int);
        void setDesc(string);

        string ParseToJason();
        void ParseFromJason(string);

    protected:

    private:
        int status; //0 se erro. 1 se sucesso
        string desc;
};

#endif // RESPONSE_H
