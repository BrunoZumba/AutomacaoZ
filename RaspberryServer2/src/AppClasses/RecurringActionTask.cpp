#include "RecurringActionTask.h"

RecurringActionTask::RecurringActionTask(string _name, ActionButtonClass _actionButton, vector<long> _dates, vector<long> _times){
    this->name = _name;
    this->actionButton = _actionButton;
    this->dates = _dates;
    this->times = _times;
}
RecurringActionTask::RecurringActionTask(){}
RecurringActionTask::~RecurringActionTask(){}

bool RecurringActionTask::createFromJson(string json){
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);

    JsonArray& timesArray = root["times"];
    for (unsigned short i = 0; i < timesArray.size(); i++) {
        long time = timesArray[i];
        this->times.insert(this->times.end(),time);
    }


    JsonArray& datesArray = root["dates"];
    for (unsigned short i = 0; i < datesArray.size(); i++){
        long date = datesArray[i];
        this->dates.insert(this->dates.end(), date);
    }

    JsonObject& actionButton = root["actionButton"];
    string actionBtStr;
    actionButton.printTo(actionBtStr);
    this->actionButton = ActionButtonClass();

//    JsonArray& action = actionButton["action"];
//    for (unsigned short i = 0; i < action.size(); i++){
//        JsonObject& cmdTask = action[i]["Task"];
//        const char* devName = cmdTask["deviceName"];
//        const char* buttonName = cmdTask["buttonName"];
//        const char* mode = cmdTask["mode"];
//        if ((devName == NULL) | (buttonName == NULL) || (mode == NULL)) {
//            return false;
//        }
//
////        CommandTask commandTask = CommandTask(devName, buttonName, mode);
//        CommandTask commandTask = CommandTask();
//
//        commandTask.setTask(TaskClass(devName, buttonName, mode));
//        this->commandTasks.push(commandTask);
//    }

    const char* tmp = root["name"];
    if ((tmp == NULL) /*|| (tmp2 == NULL) || (tmp3 == NULL)*/) {
        return false;
    }
    this->name = string(tmp);
    this->actionButton.createFromJson(actionBtStr);


    return true;
}
string RecurringActionTask::parseToJson(){
    DynamicJsonBuffer rootBuffer, actionBuffer, timesBuffer, datesBuffer;
    JsonObject& root = rootBuffer.createObject();
//    JsonObject& actionObj = actionBuffer.createObject();
    JsonArray& timesArray = timesBuffer.createArray();
    JsonArray& datesArray = datesBuffer.createArray();


    string actionJson = this->actionButton.parseToJson();
    JsonObject& actionObj = actionBuffer.parseObject(actionJson);

    for (unsigned short i = 0; i < this->times.size(); i++){
        timesArray.add(this->times.at(i));
    }
    for (unsigned short i = 0; i < this->dates.size(); i++){
        datesArray.add(this->dates.at(i));
    }

    root["name"] = this->name;
    root["dates"] = datesArray;
    root["times"] = timesArray;
    root["actionButton"] = actionObj;


    string json;
    root.printTo(json);
    return json;

}

string RecurringActionTask::getName(){return this->name;}
ActionButtonClass RecurringActionTask::getActionButton(){ return this->actionButton;}
vector<long> RecurringActionTask::getDates(){return this->dates;}
vector<long> RecurringActionTask::getTimes(){return this->times;}
