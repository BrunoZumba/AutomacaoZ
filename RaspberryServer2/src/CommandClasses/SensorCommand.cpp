#include "SensorCommand.h"

SensorCommand::SensorCommand(string json) : Command(json){}
//SensorCommand::SensorCommand(){}

SensorCommand::~SensorCommand(){}

SensorClass SensorCommand::getSensor(){
    return this->sensor;
}

void SensorCommand::setSensor(SensorClass _sensor){
    this->sensor = _sensor;
}

bool SensorCommand::createRequestFromJson(){
    Command::createRequestFromJson();

    JsonObject& sensorObj = this->jsonRoot["sensor"];
    string sensorObjStr;
    sensorObj.printTo(sensorObjStr);
    this->sensor = SensorClass();
    this->sensor.createFromJson(sensorObjStr);

    return true;


}

//bool SensorCommand::createRequestFromJson(string json){
//    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
//    JsonObject& root = jsonBuffer.parseObject(json);
//
//    //TODO: tirar essa gambs de primeiro usar um char*
//    const char* tmp = root["sensorName"];
//    const char* tmp2 = root["action"];
//
//    if ((tmp == NULL) || (tmp2 == NULL)) {
//        return false;
//    }
//    this->sensorName = string(tmp);
//    this->action = string(tmp2);
//
//    return true;
//}

bool SensorCommand::execute(){
    if (this->sensor.getSensorName() == "temperatura") {
        this->createResponse(STATUS_OK, /*"responseSensor",*/ "27", "");
    } else if (this->sensor.getSensorName() == "umidade") {
        this->createResponse(STATUS_OK, /*"responseSensor",*/ "57", "");
    } else {
        this->createResponse(STATUS_ERROR, /*"responseSensor",*/ "Sensor não reconhecido", "");
        return false;
    }
    return true;
}

