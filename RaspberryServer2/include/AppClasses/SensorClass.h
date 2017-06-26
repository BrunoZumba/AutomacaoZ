#ifndef SENSORCLASS_H
#define SENSORCLASS_H

#include<string>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>

#include "ArduinoJson.h"

using namespace std;

class SensorClass
{
    public:
        SensorClass(string, string);
        SensorClass();
        virtual ~SensorClass();

        void setSensorName(string);
        void setSensorAction(string);
        string getSensorName();
        string getSensorAction();

        string parseToJson();
        bool createFromJson(string);

    protected:

    private:
        string sensorName;
        string sensorAction;
};

#endif // SENSORCLASS_H
