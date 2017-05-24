#include "Task.h"

Task::Task(string _deviceName, string _buttonName, string _mode){
    this->deviceName = _deviceName;
    this->buttonName = _buttonName;
    this->mode = _mode;
}
Task::Task(){}

Task::~Task(){}

string Task::getDeviceName(){
    return this->deviceName;
}

string Task::getButtonName(){
    return this->buttonName;
}
string Task::getMode(){
    return this->mode;
}

void Task::setDeviceName(string _deviceName){
    this->deviceName = _deviceName;
}
void Task::setButtonName(string _buttonName){
    this->buttonName = _buttonName;
}
void Task::setMode(string _mode){
    this->mode = _mode;
}

string Task::ParseToJason(){
    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

    JsonObject& root = jsonBuffer.createObject();
    root["deviceName"] = this->deviceName;
    root["buttonName"] = this->buttonName;
    root["mode"] = this->mode;

    string jason;
    root.printTo(jason);
    return jason;
}

bool Task::ParseFromJason(string json){
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

bool Task::execute(int lircSock){

    if (this->mode.compare("SEND_ONCE") == 0){
        if (lirc_send_one(lircSock, this->deviceName.c_str(), this->buttonName.c_str()) == -1) {
            char buffer[BUFFER_SIZE];
            sprintf(buffer, "ERRO! Nao foi possivel enviar o comando %s pelo device %s através do socket LIRC %d\n%s\n", this->buttonName.c_str(), this->deviceName.c_str(), lircSock, strerror(errno));
            cout << buffer;
            return false;
        }
    } else {
        char buffer[BUFFER_SIZE];
        sprintf(buffer, "ERRO! Nao foi possivel identificar o modo %s\n", this->mode.c_str());
        cout << buffer;
        return false;
    }

    return true;
}


