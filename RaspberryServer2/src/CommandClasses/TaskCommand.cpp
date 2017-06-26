
#include "TaskCommand.h"

TaskCommand::TaskCommand(string json) : Command(json){}
TaskCommand::~TaskCommand(){}

bool TaskCommand::createRequestFromJson(){
    Command::createRequestFromJson();
//    DynamicJsonBuffer jsonBuffer;
//    JsonObject& root = jsonBuffer.parseObject(json);
//
//    //TODO: tirar essa gambs de primeiro usar um char*
//    const char* tmp = root["requestAction"];
//    bool tmp2 = root["requestOverwrite"];
//
//    if ((tmp == NULL) /*|| (tmp2 == NULL)*/) {
//        return false;
//    }
//    this->requestAction = string(tmp);
//    this->requestOverwrite = bool(tmp2);

    JsonObject& taskObj = this->jsonRoot["task"];
    string taskObjStr;
    taskObj.printTo(taskObjStr);
    this->task = TaskClass();
    this->task.createFromJson(taskObjStr);

    cout<<"TKCMD requestAction: "<<this->requestAction<<"\n";
    cout<<"TKCMD requestOverwrite: "<<this->requestOverwrite<<"\n";
    cout<<"TKCMD task: "<<taskObjStr<<"\n";


    return true;

}

bool TaskCommand::execute(){
    int lircSock = util::GetLircSocket();

    if (this->task.getMode().compare("SEND_ONCE") == 0){
        if (lirc_send_one(lircSock, this->task.getDeviceName().c_str(), this->task.getButtonName().c_str()) != 0) {
            char buffer[BUFFER_SIZE];
            sprintf(buffer, "ERRO! Nao foi possivel enviar o comando %s pelo device %s através do socket LIRC %d\n", this->task.getButtonName().c_str(), this->task.getDeviceName().c_str(), lircSock);
            cout << buffer;
            this->createResponse(STATUS_ERROR, /*"responseCommand",*/ "Erro ao enviar o comando", "");
            return false;
        }
    } else {
        char buffer[BUFFER_SIZE];
        sprintf(buffer, "ERRO! Nao foi possivel identificar o modo '%s'\n", this->task.getMode().c_str());
        cout << buffer;
        this->createResponse(STATUS_ERROR, /*"responseCommand",*/ buffer, "");
        responseStatus = STATUS_ERROR;
        responseDesc = buffer;
        return false;
    }

    this->createResponse(STATUS_OK, /*"responseCommand",*/ "Comando enviado com sucesso", "");

    return true;
}

TaskClass TaskCommand::getTask(){
    return this->task;
}

void TaskCommand::setTask(TaskClass _task){
    this->task = _task;
}




