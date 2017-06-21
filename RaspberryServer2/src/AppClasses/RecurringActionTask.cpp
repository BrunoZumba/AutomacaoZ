#include "RecurringActionTask.h"

RecurringActionTask::RecurringActionTask(string _name, queue<CommandTask> _commandTasks, queue<long> _dates, queue<long> _times){
    this->name = _name;
    this->commandTasks = _commandTasks;
    this->dates = _dates;
    this->times = _times;
}
RecurringActionTask::RecurringActionTask(){}
RecurringActionTask::~RecurringActionTask(){}

bool RecurringActionTask::ParseRequestFromJason(string json){
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);

    JsonArray& timesArray = root["times"];
    for (unsigned short i = 0; i < timesArray.size(); i++) {
        long time = timesArray[i];
        this->times.push(time);
    }

    JsonArray& datesArray = root["dates"];
    for (unsigned short i = 0; i < datesArray.size(); i++){
        long date = datesArray[i];
        this->dates.push(date);
    }

    JsonObject& actionButton = root["actionButton"];
    JsonArray& action = actionButton["action"];

    for (unsigned short i = 0; i < action.size(); i++){
        JsonObject& cmdTask = action[i]["Task"];
        const char* devName = cmdTask["deviceName"];
        const char* buttonName = cmdTask["buttonName"];
        const char* mode = cmdTask["mode"];
        if ((devName == NULL) | (buttonName == NULL) || (mode == NULL)) {
            return false;
        }

//        CommandTask commandTask = CommandTask(devName, buttonName, mode);
        CommandTask commandTask = CommandTask();

        commandTask.setTask(TaskClass(devName, buttonName, mode));
        this->commandTasks.push(commandTask);
    }

    const char* tmp = root["name"];
    if ((tmp == NULL) /*|| (tmp2 == NULL) || (tmp3 == NULL)*/) {
        return false;
    }
    this->name = string(tmp);


    return true;
}

bool RecurringActionTask::execute(){
    return true;
}

string RecurringActionTask::getName(){return this->name;}
queue<CommandTask> RecurringActionTask::getCommandTasks(){ return this->commandTasks;}
queue<long> RecurringActionTask::getDates(){return this->dates;}
queue<long> RecurringActionTask::getTimes(){return this->times;}
