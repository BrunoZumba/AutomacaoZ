#include "TaskClass.h"

TaskClass::TaskClass(string _deviceName, string _buttonName, string _mode){
    this->deviceName = _deviceName;
    this->buttonName = _buttonName;
    this->mode = _mode;
}

TaskClass::TaskClass(){}
TaskClass::~TaskClass(){}

bool TaskClass::createFromJson(string json){
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);

    //TODO: tirar essa gambs de primeiro usar um char*
    const char* tmp = root["deviceName"];
    const char* tmp2 = root["buttonName"];
    const char* tmp3 = root["mode"];

    if ((tmp == NULL) || (tmp2 == NULL) || (tmp3 == NULL)) {
        return false;
    }
    this->deviceName = string(tmp);
    this->buttonName = string(tmp2);
    this->mode = string(tmp3);

    return true;
}

string TaskClass::parseToJson(){
    DynamicJsonBuffer jsonBuffer;

    JsonObject& root = jsonBuffer.createObject();
    root["deviceName"] = this->deviceName;
    root["buttonName"] = this->buttonName;
    root["mode"] = this->mode;

    string json;
    root.printTo(json);
    return json;
}

string TaskClass::getDeviceName(){
    return this->deviceName;
}
string TaskClass::getButtonName(){
    return this->buttonName;
}
string TaskClass::getMode(){
    return this->mode;
}

void TaskClass::setDeviceName(string _deviceName){
    this->deviceName = _deviceName;
}
void TaskClass::setButtonName(string _buttonName){
    this->buttonName = _buttonName;
}
void TaskClass::setMode(string _mode){
    this->mode = _mode;
}
