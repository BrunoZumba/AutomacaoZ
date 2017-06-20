#include "Task.h"

Task::Task(){}

Task::~Task(){}

int Task::getResponseStatus(){
    return this->responseStatus;
}
string Task::getResponseDesc(){
	return this->responseDesc;
}
string Task::getResponseAction(){
	return this->responseAction;
}
string Task::getResponseParm(){
	return this->responseParm;
}
void Task::setResponseStatus(int _responseStatus){
    this->responseStatus = _responseStatus;
}
void Task::setResponseDesc(string _responseDesc){
    this->responseDesc = _responseDesc;
}
void Task::setResponseAction(string _responseAction){
    this->responseAction = _responseAction;
}
void Task::setResponseParm(string _responseParm){
    this->responseParm = _responseParm;
}

string Task::ParseResponseToJason(){
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

    JsonObject& root = jsonBuffer.createObject();
    root["responseStatus"] = this->responseStatus;
    root["responseAction"] = this->responseAction;
    root["responseDesc"] = this->responseDesc;
    root["responseParm"] = this->responseParm;

    string jason;
    root.printTo(jason);
    return jason;
}

void Task::createResponse(int _responseStatus, string _responseAction, string _responseDesc, string _responseParm){
	this->responseStatus = _responseStatus;
	this->responseAction = _responseAction;
	this->responseDesc = _responseDesc;
	this->responseParm = _responseParm;
}
