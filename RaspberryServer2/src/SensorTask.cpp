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
int SensorTask::getResponseStatus(){
    return this->responseStatus;
}
string SensorTask::getResponseDesc(){
	return this->responseDesc;
}

void SensorTask::setSensorName(string _sensorName){
    this->sensorName = _sensorName;
}
void SensorTask::setAction(string _action){
    this->action = _action;
}
void SensorTask::setResponseStatus(int _responseStatus){
    this->responseStatus = _responseStatus;
}
void SensorTask::setResponseDesc(string _responseDesc){
    this->responseDesc = _responseDesc;
}

string SensorTask::ParseResponseToJason(){
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

    JsonObject& root = jsonBuffer.createObject();
    root["responseStatus"] = this->responseStatus;
    root["responseDesc"] = this->responseDesc;

    string jason;
    root.printTo(jason);
    return jason;
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

bool SensorTask::execute(){
    if (sensorName == "temperatura") {
        responseStatus = STATUS_OK;
        responseDesc = "27";
    } else if (sensorName == "umidade") {
        responseStatus = STATUS_OK;
        responseDesc = "57";
    }


    return true;
}

void SensorTask::createResponse(int _responseStatus, string _responseDesc){
	this->responseStatus = _responseStatus;
	this->responseDesc = _responseDesc;
}
