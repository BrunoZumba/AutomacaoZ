#include "SensorTask.h"

SensorTask::SensorTask(string _sensorName, string _action){
    this->sensorName = _sensorName;
    this->action = _action;
}
SensorTask::SensorTask(){}

SensorTask::~SensorTask(){}

string SensorTask::getSensorName(){
    return this->sensorName;
}
string SensorTask::getAction(){
    return this->action;
}

void SensorTask::setSensorName(string _sensorName){
    this->sensorName = _sensorName;
}
void SensorTask::setAction(string _action){
    this->action = _action;
}

bool SensorTask::ParseRequestFromJason(string json){
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);

    //TODO: tirar essa gambs de primeiro usar um char*
    const char* tmp = root["sensorName"];
    const char* tmp2 = root["action"];

    if ((tmp == NULL) || (tmp2 == NULL)) {
        return false;
    }
    this->sensorName = string(tmp);
    this->action = string(tmp2);

    return true;
}

bool SensorTask::execute(int a){
    if (sensorName == "temperatura") {
        responseStatus = STATUS_OK;
        responseDesc = "27";
    } else if (sensorName == "umidade") {
        responseStatus = STATUS_OK;
        responseDesc = "57";
    }



    return true;
}

