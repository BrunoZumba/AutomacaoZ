#include "Command.h"

/** 'jsonRoot' deve ser uma variavel da classe Command pq precisa ser acessada
pelas classes derivadas.

Como essa variável e' uma referencia (&) ela já deve ser inicializada no construtor
*/
Command::Command(string json):jsonRoot(rootJsonBuffer.parseObject(json)){}

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
string Command::getRequestAction(){
    return this->requestAction;
}
void Command::setRequestAction(string _requestAction){
    this->requestAction = _requestAction;
}
bool Command::getRequestOverwrite(){
    return this->requestOverwrite;
}
void Command::setRequestOverwrite(bool _requestOverwrite){
    this->requestOverwrite = _requestOverwrite;
}

bool Command::createRequestFromJson(){
    //&jsonRoot = this->rootJsonBuffer.parseObject(json);

    //TODO: tirar essa gambs de primeiro usar um char*
    const char* tmp = this->jsonRoot["requestAction"];
    bool tmp2 = this->jsonRoot["requestOverwrite"];

    if ((tmp == NULL) /*|| (tmp2 == NULL)*/) {
        return false;
    }
    this->requestAction = string(tmp);
    this->requestOverwrite = bool(tmp2);

    return true;
}

string Command::ParseResponseToJason(){
    string json;
    DynamicJsonBuffer jsonBuffer;
    DynamicJsonBuffer responseParmBuffer;

    JsonObject& root = jsonBuffer.createObject();
    root["responseStatus"] = this->responseStatus;
    root["responseAction"] = this->responseAction;
    root["responseDesc"] = this->responseDesc;

    //Para getList, o responseParm é um array.
    if(this->requestAction == "getList"){
        JsonArray& responseParmObj = responseParmBuffer.parseArray(this->responseParm);
        root["responseParm"] = responseParmObj;
        root.printTo(json);
    } else { // para demais situações, response parm é uma string
        root["responseParm"] = this->responseParm;
        root.printTo(json);
    }



    return json;
}

void Command::createResponse(int _responseStatus, /*string _responseAction,*/ string _responseDesc, string _responseParm){
	this->responseStatus = _responseStatus;
	this->responseAction = (this->requestAction+"Response");
	this->responseDesc = _responseDesc;
	this->responseParm = _responseParm;
}
