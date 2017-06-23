#include "Command.h"

Command::Command(){}

Command::~Command(){}

int Command::getResponseStatus(){
    return this->responseStatus;
}
string Command::getResponseDesc(){
	return this->responseDesc;
}
string Command::getResponseAction(){
	return this->responseAction;
}
string Command::getResponseParm(){
	return this->responseParm;
}
void Command::setResponseStatus(int _responseStatus){
    this->responseStatus = _responseStatus;
}
void Command::setResponseDesc(string _responseDesc){
    this->responseDesc = _responseDesc;
}
void Command::setResponseAction(string _responseAction){
    this->responseAction = _responseAction;
}
void Command::setResponseParm(string _responseParm){
    this->responseParm = _responseParm;
}

string Command::ParseResponseToJason(){
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

    JsonObject& root = jsonBuffer.createObject();
    root["responseStatus"] = this->responseStatus;
    root["responseAction"] = this->responseAction;
    root["responseDesc"] = this->responseDesc;
    root["responseParm"] = this->responseParm;

    string json;
    root.printTo(json);
    return json;
}

void Command::createResponse(int _responseStatus, string _responseAction, string _responseDesc, string _responseParm){
	this->responseStatus = _responseStatus;
	this->responseAction = _responseAction;
	this->responseDesc = _responseDesc;
	this->responseParm = _responseParm;
}
