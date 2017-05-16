#ifndef TASK_H
#define TASK_H

#include<string>

using namespace std;

class Task
{
    public:
        Task(string, string, string);
        virtual ~Task();

        string getDeviceName();
        string getButtonName();
        string getMode();

        void setDeviceName(string);
        void setButtonName(string);
        void setMode(string);

    protected:


    private:
        string deviceName;
        string buttonName;
        string mode;
};

#endif // TASK_H
