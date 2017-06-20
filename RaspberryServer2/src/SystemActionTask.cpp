#include "SystemActionTask.h"

SystemActionTask::SystemActionTask(string _actionName){
    this->actionName = _actionName;
}

SystemActionTask::SystemActionTask(){}
SystemActionTask::~SystemActionTask(){}

string SystemActionTask::getActionName(){
    return this->actionName;
}

void SystemActionTask::setActionName(string _actionName){
    this->actionName = _actionName;
}

bool SystemActionTask::ParseRequestFromJason(string json) {
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);

    //TODO: tirar essa gambs de primeiro usar um char*
    const char* tmp = root["actionName"];

    if (tmp == NULL) {
        return false;
    }
    this->actionName = string(tmp);

    return true;
}


bool SystemActionTask::execute(){
    if (actionName == "shutdown") {
        //system("netstat -na | grep 8168");
        this->createResponse(STATUS_OK, "responseSystemAction", "Shutdown executado", "");
        return true;
    } else {
        this->createResponse(STATUS_ERROR, "responseSystemAction", "Comando não reconhecido", "");
        return false;
    }


    return false;
}
