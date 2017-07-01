
#include "TaskCommand.h"

TaskCommand::TaskCommand(string json) : Command(json){}
TaskCommand::~TaskCommand(){}

bool TaskCommand::createRequestFromJson(){
    Command::createRequestFromJson();

    JsonObject& taskObj = this->jsonRoot["task"];
    string taskObjStr;
    taskObj.printTo(taskObjStr);
    this->task = TaskClass();
    this->task.createFromJson(taskObjStr);

//    cout<<"TKCMD requestAction: "<<this->requestAction<<"\n";
//    cout<<"TKCMD requestOverwrite: "<<this->requestOverwrite<<"\n";
//    cout<<"TKCMD task: "<<taskObjStr<<"\n";


    return true;

}

bool TaskCommand::execute(){
    int lircSock = util::GetLircSocket();
    char buffer[BUFFER_SIZE];

    if (this->task.getDeviceName().compare("ControleLuz") == 0){
        #ifndef __arm__ //Verifica se o codigo esta rodando no RaspberryPi. Caso contrario, retorna false
            this->createResponse(STATUS_ERROR, /*"responseSensor",*/ "Necessário server estar no RPi para este serviço", "");
            return false;
        #endif // __arm__
        //Prepara os itens para enviar o sinal RF
        int pulseLength = 0;
        if (wiringPiSetup () == -1) {
            sprintf(buffer, "ERRO! Nao foi possivel inicializar WiringPi\n");
            cout << buffer;
            this->createResponse(STATUS_ERROR, /*"responseCommand",*/ "Erro ao preparar a lib WiringPi", "");
            responseStatus = STATUS_ERROR;
            responseDesc = buffer;
            return false;
        }

        RCSwitch mySwitch = RCSwitch();
        mySwitch.setProtocol(0); //default protocol
        if (pulseLength != 0) mySwitch.setPulseLength(pulseLength);
        mySwitch.enableTransmit(PIN);

        if (this->task.getButtonName().compare("KEY_ON") == 0){
            int code = 8080;
            mySwitch.send(code, 24);

        } else if (this->task.getButtonName().compare("KEY_OFF") == 0){
            int code = 1;
            mySwitch.send(code, 24);

        } else {
            sprintf(buffer, "ERRO! Nao foi possivel identificar o botão pressionado\n");
            cout << buffer;
            this->createResponse(STATUS_ERROR, /*"responseCommand",*/ "Erro ao identificar o comando RF", "");
            responseStatus = STATUS_ERROR;
            responseDesc = buffer;
            return false;
        }
    } else {
        if (this->task.getMode().compare("SEND_ONCE") == 0){
            if (lirc_send_one(lircSock, this->task.getDeviceName().c_str(), this->task.getButtonName().c_str()) != 0) {
                sprintf(buffer, "ERRO! Nao foi possivel enviar o comando %s pelo device %s através do socket LIRC %d\n", this->task.getButtonName().c_str(), this->task.getDeviceName().c_str(), lircSock);
                cout << buffer;
                this->createResponse(STATUS_ERROR, /*"responseCommand",*/ "Erro ao enviar o comando", "");
                return false;
            }
        } else {
            sprintf(buffer, "ERRO! Nao foi possivel identificar o modo '%s'\n", this->task.getMode().c_str());
            cout << buffer;
            this->createResponse(STATUS_ERROR, /*"responseCommand",*/ buffer, "");
            responseStatus = STATUS_ERROR;
            responseDesc = buffer;
            return false;
        }
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




