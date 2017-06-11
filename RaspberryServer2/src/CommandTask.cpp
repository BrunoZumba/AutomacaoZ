
#include "CommandTask.h"

CommandTask::CommandTask(string _deviceName, string _buttonName, string _mode){
    this->deviceName = _deviceName;
    this->buttonName = _buttonName;
    this->mode = _mode;
}
CommandTask::CommandTask(){}

CommandTask::~CommandTask(){}

string CommandTask::getDeviceName(){
    return this->deviceName;
}
string CommandTask::getButtonName(){
    return this->buttonName;
}
string CommandTask::getMode(){
    return this->mode;
}

void CommandTask::setDeviceName(string _deviceName){
    this->deviceName = _deviceName;
}
void CommandTask::setButtonName(string _buttonName){
    this->buttonName = _buttonName;
}
void CommandTask::setMode(string _mode){
    this->mode = _mode;
}

bool CommandTask::ParseRequestFromJason(string json){
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);

    //TODO: tirar essa gambs de primeiro usar um char*
    const char* tmp = root["deviceName"];
    const char* tmp2 = root["buttonName"];
    const char* tmp3 = root["mode"];

    if ((tmp == NULL) || (tmp2 == NULL) || (tmp3 == NULL)) {
        return false;
    }
    this->deviceName = string(tmp);
    this->buttonName = string(tmp2);
    this->mode = string(tmp3);

    return true;
}


bool CommandTask::execute(int lircSock){

    if (this->mode.compare("SEND_ONCE") == 0){
        if (lirc_send_one(lircSock, this->deviceName.c_str(), this->buttonName.c_str()) != 0) {
            char buffer[BUFFER_SIZE];
            sprintf(buffer, "ERRO! Nao foi possivel enviar o comando %s pelo device %s através do socket LIRC %d\n", this->buttonName.c_str(), this->deviceName.c_str(), lircSock);
            cout << buffer;
            responseStatus = STATUS_ERROR;
            responseDesc = "Erro ao enviar o comando";
            return false;
        }
    } else {
        char buffer[BUFFER_SIZE];
        sprintf(buffer, "ERRO! Nao foi possivel identificar o modo %s\n", this->mode.c_str());
        cout << buffer;
        responseStatus = STATUS_ERROR;
        responseDesc = buffer;
        return false;
    }

    responseStatus = STATUS_OK;
    responseDesc = "Comando enviado com sucesso";

    return true;
}


