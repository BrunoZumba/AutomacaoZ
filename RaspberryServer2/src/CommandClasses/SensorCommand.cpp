#include "SensorCommand.h"

SensorCommand::SensorCommand(string _sensorName, string _action){
    this->sensorName = _sensorName;
    this->action = _action;
}
SensorCommand::SensorCommand(){}

SensorCommand::~SensorCommand(){}

string SensorCommand::getSensorName(){
    return this->sensorName;
}
string SensorCommand::getAction(){
    return this->action;
}

void SensorCommand::setSensorName(string _sensorName){
    this->sensorName = _sensorName;
}
void SensorCommand::setAction(string _action){
    this->action = _action;
}

bool SensorCommand::ParseRequestFromJason(string json){
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

bool SensorCommand::execute(){
    if (sensorName == "temperatura") {
        this->createResponse(STATUS_OK, "responseSensor", "27", "");
    } else if (sensorName == "umidade") {
        this->createResponse(STATUS_OK, "responseSensor", "57", "");
    } else {
        this->createResponse(STATUS_ERROR, "responseSensor", "Sensor não reconhecido", "");
        return false;
    }
    return true;
}

