
#ifndef SENSORTASK_H
#define SENSORTASK_H


#include "Command.h"

using namespace std;

#define STATUS_OK 1
#define STATUS_ERROR 0

class SensorCommand : public Command {
    public:
        SensorCommand(string, string);
        SensorCommand();
        virtual ~SensorCommand();

        string getSensorName();
        string getAction();

        void setSensorName(string);
        void setAction(string);

        bool execute();

        bool ParseRequestFromJason(string);

    protected:


    private:
        string sensorName;
        string action;

};

#endif // SENSORTASK_H