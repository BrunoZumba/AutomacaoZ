#include "TaskButtonClass.h"

TaskButtonClass::TaskButtonClass(){
    //ctor
}

TaskButtonClass::~TaskButtonClass(){
    //dtor
}



string TaskButtonClass::parseToJson(){
    DynamicJsonBuffer jsonBuffer;
    DynamicJsonBuffer taskBuffer;

    JsonObject& root = jsonBuffer.createObject();
    root["buttonId"] = this->buttonId;

    string taskStr = this->task.parseToJson();
    JsonObject& taskObj = taskBuffer.parseObject(taskStr);

    root["task"] = taskObj;


    string json;
    root.printTo(json);
    return json;
}

bool TaskButtonClass::createFromJson(string json){
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);

    //TODO: tirar essa gambs de primeiro usar um char*
    const char* tmp = root["buttonId"];

    JsonObject& taskJson = root["task"];
    string taskStr;
    taskJson.printTo(taskStr);
    this->task = TaskClass();

    if (tmp == NULL) {
        return false;
    }

    this->buttonId = string(tmp);
    this->task.createFromJson(taskStr);

    return true;
}



string TaskButtonClass::getButtonId(){
    return this->buttonId;
}
void TaskButtonClass::setButtonId(string _buttonId){
    this->buttonId = _buttonId;
}
TaskClass TaskButtonClass::getTask(){
    return this->task;
}
void TaskButtonClass::setTask(TaskClass _task){
    this->task = _task;
}
