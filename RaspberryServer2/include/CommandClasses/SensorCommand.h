
#ifndef SENSORTASK_H
#define SENSORTASK_H


#include "Command.h"
#include "SensorClass.h"

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
