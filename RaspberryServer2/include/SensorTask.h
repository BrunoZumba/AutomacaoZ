
#ifndef SENSORTASK_H
#define SENSORTASK_H


#include "Task.h"

using namespace std;

#define STATUS_OK 1
#define STATUS_ERROR 0

class SensorTask : public Task {
    public:
        SensorTask(string, string);
        SensorTask();
        virtual ~SensorTask();

        string getSensorName();
        string getAction();

        void setSensorName(string);
        void setAction(string);

        bool execute(int);

        bool ParseRequestFromJason(string);

    protected:


    private:
        string sensorName;
        string action;

};

#endif // SENSORTASK_H
