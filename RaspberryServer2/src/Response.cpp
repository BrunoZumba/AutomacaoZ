#include "Response.h"

Response::Response(int _status, string _desc){
    this->status = _status;
    this->desc = _desc;
}
Response::Response(string json){
    this->ParseFromJason(json);
}
Response::Response(){};

Response::~Response(){}


void Response::create(int _status, string _desc){
    this->status = _status;
    this->desc = _desc;
}

int Response::getStatus(){
    return this->status;
}
string Response::getDesc(){
    return this->desc;
}

void Response::setStatus(int status){
    this->status = status;
}
void Response::setDesc(string desc){
    this->desc = desc;
}

string Response::ParseToJason(){
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

    JsonObject& root = jsonBuffer.createObject();
    root["status"] = this->status;
    root["desc"] = this->desc;

    string jason;
    root.printTo(jason);
    return jason;
}


void Response::ParseFromJason(string json){
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);

    //TODO: tirar essa gambs de primeiro usar um char*
    this->status = root["status"];
    const char* tmp = root["desc"];
    this->desc = string(tmp);
}
