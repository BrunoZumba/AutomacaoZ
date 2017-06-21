
#include "CommandTask.h"

CommandTask::CommandTask(){}
CommandTask::~CommandTask(){}


bool CommandTask::execute(){
    int lircSock = util::GetLircSocket();

    if (this->task.getMode().compare("SEND_ONCE") == 0){
        if (lirc_send_one(lircSock, this->task.getDeviceName().c_str(), this->task.getButtonName().c_str()) != 0) {
            char buffer[BUFFER_SIZE];
            sprintf(buffer, "ERRO! Nao foi possivel enviar o comando %s pelo device %s através do socket LIRC %d\n", this->task.getButtonName().c_str(), this->task.getDeviceName().c_str(), lircSock);
            cout << buffer;
            this->createResponse(STATUS_ERROR, "responseCommand", "Erro ao enviar o comando", "");
            return false;
        }
    } else {
        char buffer[BUFFER_SIZE];
        sprintf(buffer, "ERRO! Nao foi possivel identificar o modo %s\n", this->task.getMode().c_str());
        cout << buffer;
        this->createResponse(STATUS_ERROR, "responseCommand", buffer, "");
        responseStatus = STATUS_ERROR;
        responseDesc = buffer;
        return false;
    }

    this->createResponse(STATUS_OK, "responseCommand", "Comando enviado com sucesso", "");

    return true;
}

TaskClass CommandTask::getTask(){
    return this->task;
}

void CommandTask::setTask(TaskClass _task){
    this->task = _task;
}




