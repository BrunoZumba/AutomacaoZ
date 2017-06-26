#include "SystemActionCommand.h"

SystemActionCommand::SystemActionCommand(string json) : Command(json){}

SystemActionCommand::~SystemActionCommand(){}

string SystemActionCommand::getActionName(){
    return this->actionName;
}

void SystemActionCommand::setActionName(string _actionName){
    this->actionName = _actionName;
}

bool SystemActionCommand::createRequestFromJson() {
    Command::createRequestFromJson();
//    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
//    JsonObject& root = jsonBuffer.parseObject(json);

    //TODO: tirar essa gambs de primeiro usar um char*
    const char* tmp = this->jsonRoot["actionName"];

    if (tmp == NULL) {
        return false;
    }
    this->actionName = string(tmp);

    return true;
}


bool SystemActionCommand::execute(){
    if (actionName == "shutdown") {
        //system("netstat -na | grep 8168");
        this->createResponse(STATUS_OK, /*"responseSystemAction", */"Shutdown executado", "");
        return true;
    } else {
        this->createResponse(STATUS_ERROR, /*"responseSystemAction",*/ "Comando não reconhecido", "");
        return false;
    }


    return false;
}
