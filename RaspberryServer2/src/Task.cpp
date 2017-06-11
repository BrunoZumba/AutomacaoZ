#include "Task.h"

Task::Task(){}

Task::~Task(){}

int Task::getResponseStatus(){
    return this->responseStatus;
}
string Task::getResponseDesc(){
	return this->responseDesc;
}
void Task::setResponseStatus(int _responseStatus){
    this->responseStatus = _responseStatus;
}
void Task::setResponseDesc(string _responseDesc){
    this->responseDesc = _responseDesc;
}

string Task::ParseResponseToJason(){
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

    JsonObject& root = jsonBuffer.createObject();
    root["responseStatus"] = this->responseStatus;
    root["responseDesc"] = this->responseDesc;

    string jason;
    root.printTo(jason);
    return jason;
}

void Task::createResponse(int _responseStatus, string _responseDesc){
	this->responseStatus = _responseStatus;
	this->responseDesc = _responseDesc;
}
