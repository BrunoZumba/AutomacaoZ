#include "ActionClass.h"

ActionClass::ActionClass(){
    //ctor
}

ActionClass::~ActionClass(){
    //dtor
}



string ActionClass::parseToJson(){
    DynamicJsonBuffer jsonBuffer;

    JsonArray& root = jsonBuffer.createArray();
    for (unsigned short i = 0; i < this->actions.size(); i++){
//        cout << "\nParse: " << this->actions.at(i).parseToJson();
        root.add(this->actions.at(i).parseToJson());
    }

    string json;
    root.printTo(json);
    return json;

}

bool ActionClass::createFromJson(string json){
    DynamicJsonBuffer jsonBuffer;
    JsonArray& root = jsonBuffer.parseArray(json);
    for (unsigned short i = 0; i < root.size(); i++) {
        TaskButtonClass taskButton = TaskButtonClass();
        taskButton.createFromJson(root[i]);
        this->actions.insert(this->actions.end(), taskButton);
    }

    return true;

}

vector<TaskButtonClass> ActionClass::getActions(){
    return this->actions;
}
void ActionClass::setActions(vector<TaskButtonClass> _actions){
    this->actions = _actions;
}
