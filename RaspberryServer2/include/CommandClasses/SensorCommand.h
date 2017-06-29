#ifndef SENSORTASK_H
#define SENSORTASK_H

#include "Command.h"
#include "SensorClass.h"

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sstream>
#define MAXTIMINGS      85
#define DHTPIN          7

using namespace std;

#define STATUS_OK 1
#define STATUS_ERROR 0

class SensorCommand : public Command {
    public:
        SensorCommand(string json);
        virtual ~SensorCommand();

        SensorClass getSensor();
        void setSensor(SensorClass);

        bool execute();
        bool createRequestFromJson();

    protected:


    private:
        SensorClass sensor;

};

#endif // SENSORTASK_H
