#include "ActionButtonClass.h"

ActionButtonClass::ActionButtonClass(){
    //ctor
}

ActionButtonClass::~ActionButtonClass(){
    //dtor
}




string ActionButtonClass::parseToJson(){
    DynamicJsonBuffer jsonBuffer;
    DynamicJsonBuffer actionBuffer;

    JsonObject& root = jsonBuffer.createObject();
    root["actionName"] = this->actionName;

    string actionStr = this->action.parseToJson();
    JsonArray& actionObj = actionBuffer.parseArray(actionStr);

    root["action"] = actionObj;


    string json;
    root.printTo(json);
    return json;

}

bool ActionButtonClass::createFromJson(string json){
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);

    //TODO: tirar essa gambs de primeiro usar um char*
    const char* tmp = root["actionName"];

    JsonArray& actionJson = root["action"];
    string actionStr;
    actionJson.printTo(actionStr);
    this->action = ActionClass();

    if (tmp == NULL) {
        return false;
    }

    this->actionName = string(tmp);
    this->action.createFromJson(actionStr);

    return true;
}

string ActionButtonClass::getActionName(){
    return this->actionName;
}
void ActionButtonClass::setActionName(string _actionName){
    this->actionName = _actionName;
}
ActionClass ActionButtonClass::getAction(){
    return this->action;
}
void ActionButtonClass::setActionName(ActionClass _action){
    this->action = _action;
}
