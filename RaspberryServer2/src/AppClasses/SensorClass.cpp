#include "SensorClass.h"

SensorClass::SensorClass(string _sensorName, string _sensorAction){
    this->sensorName = _sensorName;
    this->sensorAction = _sensorAction;
}

SensorClass::SensorClass(){}

SensorClass::~SensorClass(){}


string SensorClass::parseToJson(){
    DynamicJsonBuffer jsonBuffer;

    JsonObject& root = jsonBuffer.createObject();
    root["sensorName"] = this->sensorName;
    root["sensorAction"] = this->sensorAction;

    string json;
    root.printTo(json);
    return json;
}

bool SensorClass::createFromJson(string json){
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);

    //TODO: tirar essa gambs de primeiro usar um char*
    const char* tmp = root["sensorName"];
    const char* tmp2 = root["sensorAction"];

    if ((tmp == NULL) || (tmp2 == NULL)) {
        return false;
    }
    this->sensorName = string(tmp);
    this->sensorAction = string(tmp2);

    return true;
}




void SensorClass::setSensorName(string _sensorName){
    this->sensorName = _sensorName;
}
void SensorClass::setSensorAction(string _sensorAction){
    this->sensorAction = _sensorAction;
}
string SensorClass::getSensorName(){
    return this->sensorName;
}
string SensorClass::getSensorAction(){
    return this->sensorAction;
}



