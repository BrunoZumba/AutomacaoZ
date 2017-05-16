#include "Task.h"

Task::Task(string _deviceName, string _buttonName, string _mode){
    deviceName = _deviceName;
    buttonName = _buttonName;
    mode = _mode;
}
Task::~Task(){}

string Task::getDeviceName(){
    return this->deviceName;
}

string Task::getButtonName(){
    return buttonName;
}
string Task::getMode(){
    return mode;
}

void Task::setDeviceName(string _deviceName){
    deviceName = _deviceName;
}
void Task::setButtonName(string _buttonName){
    buttonName = _buttonName;
}
void Task::setMode(string _mode){
    mode = _mode;
}



