#include<stdio.h>
#include<pthread.h>
#include<iostream>
#include<errno.h>   //usado pela errno
#include<string.h>  //usado pela strerror
#include<unistd.h>

#include"include/ListenThreadPool.h"
#include"include/Util.h"
#include "ArduinoJson.h"
#include "Util.h"
#include "RecurringActionTask.h"
#include "TaskButtonClass.h"
#include "ActionClass.h"
#include "ActionButtonClass.h"
#include "RecurringActionCommand.h"

#include <fstream>

using namespace std;

#define QTD_SERVICOS 4



int main(){
    int portas[QTD_SERVICOS] = {4391, 8742, 8168, 5223};
    pthread_t threads[QTD_SERVICOS];
    int i, rc;
    char buffer[BUFFER_SIZE];


    string recurringActionBuffer = "{\"actionType\":\"deleteList\",\"overwrite\":true,\"recurringAction\":{\"name\":\"777\",\"dates\":[000,000,000],\"times\":[1497808731, 1111111,2222],\"actionButton\":{\"actionName\":\"aço\",\"action\":[{\"buttonId\":\"2131558568\",\"task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_EXIT\",\"mode\":\"SEND_ONCE\"}},{\"buttonId\":\"2131558569\",\"task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_POWER\",\"mode\":\"SEND_ONCE\"}}]}}}";
    string recurringActionBuffer2 = "{\"actionType\":\"getList\",\"overwrite\":\"\",\"recurringAction\":{}}";
    RecurringActionCommand recurringActionCommand;
    if(recurringActionCommand.createFromJson(recurringActionBuffer)){
        cout<<"\nCriou Recurring Action\n";
        if (recurringActionCommand.execute()){
            cout<<"Executou com sucesso\n";
//            cout<<recurringActionCommand.getResponseAction()<<"\n";
//            cout<<recurringActionCommand.getResponseParm();
        }
    }

//    string lala = "{\"nome1\":\"Vai sifude\", \"nome2\":\"tomanucu\"}";
//    string lala2 = "{\"nome1\":\"Vai sifude2\", \"nome2\":\"tomanucu2\"}";
//    DynamicJsonBuffer jsonBuffer;
//    DynamicJsonBuffer jsonBuffer2;
//    JsonArray& root = jsonBuffer.createArray();
//
//    JsonObject& jsonObj = jsonBuffer2.parseObject(lala);
//    root.add(jsonObj);
//    jsonObj = jsonBuffer2.parseObject(lala2);
//    root.add(jsonObj);
//
//    string aux;
//    root.printTo(aux);
//    cout<<"Aux: "<<aux;



    /**
     * O thread pool são apenas threads que ficam aguardando novas tarefas.
     * Quando uma tarefa chega, uma thread a pega, trata, delete e volta a esperar novas.
     */
//    ListenThreadPool::createThreadPool();

//    vector<int> lala;
//    lala.insert(lala.end(), 1);
//    lala.insert(lala.end(), 2);
//    for (int i = 0; i < lala.size(); i++){
//        cout << i << ": " <<lala.at(i) << "\n";
//    }
//
//    RecurringActionTask recurringActionTask = RecurringActionTask();
//    string buffer1 = "{\"name\":\"f\",\"dates\":[\"123\",\"456\",\"789\"],\"times\":[\"1497808731\", \"1111111\",\"2222\"],\"actionButton\":{\"actionName\":\"aço\",\"action\":[{\"buttonId\":\"2131558568\",\"task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_EXIT\",\"mode\":\"SEND_ONCE\"}},{\"buttonId\":\"2131558569\",\"task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_POWER\",\"mode\":\"SEND_ONCE\"}}]}}";
//    if(recurringActionTask.createFromJson(buffer1)){
//        cout<< "Name: " << recurringActionTask.getName();
//        for(int i = 0; i < recurringActionTask.getDates().size(); i++){
//            cout<<"\nData["<<i<<"]:" << recurringActionTask.getDates().at(i);
//        }
//        for(int i = 0; i < recurringActionTask.getTimes().size(); i++){
//            cout<<"\nTime["<<i<<"]:" << recurringActionTask.getTimes().at(i);
//        }
//        cout<<"\nAction: " << recurringActionTask.getActionButton().parseToJson();
//    }



//    string taskBuffer = "{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_EXIT\",\"mode\":\"SEND_ONCE\"}";
//    TaskClass task = TaskClass();
//    if (task.createFromJson(taskBuffer)){
//        cout<<"Device: " <<task.getDeviceName();
//        cout<<"\nButton: " <<task.getButtonName();
//        cout<<"\nmode: " <<task.getMode();
//        cout<<"\nTask: "<<task.parseToJson();
//    }
//
//    string taskButtonBUffer = "{\"buttonId\":\"2131558568\",\"task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_EXIT\",\"mode\":\"SEND_ONCE\"}}";
//    TaskButtonClass taskButton = TaskButtonClass();
//    if (taskButton.createFromJson(taskButtonBUffer)){
////        cout<<"\nButtonId: "<< taskButton.getButtonId();
////        cout<<"\ntask: "<< taskButton.getTask().parseToJson();
//    }
//
//    string actionBuffer = "[{\"buttonId\":\"2131558568\",\"task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_EXIT\",\"mode\":\"SEND_ONCE\"}},{\"buttonId\":\"2131558569\",\"task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_POWER\",\"mode\":\"SEND_ONCE\"}}]";
//    ActionClass action = ActionClass();
//    if (action.createFromJson(actionBuffer)){
//        for (int i = 0; i < action.getActions().size(); i++){
//            cout << "Action[" << i <<"]: " << action.getActions().at(i).parseToJson()<<"\n";
//        }
//    }
//
//    string actionButtonBuffer = "{\"actionName\":\"aço\",\"action\":[{\"buttonId\":\"2131558568\",\"task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_EXIT\",\"mode\":\"SEND_ONCE\"}},{\"buttonId\":\"2131558569\",\"task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_POWER\",\"mode\":\"SEND_ONCE\"}}]}";
//    ActionButtonClass actionButton = ActionButtonClass();
//    if (actionButton.createFromJson(actionButtonBuffer)){
//        cout << "ActionName: " << actionButton.getActionName();
//        cout << "\nAction: " << actionButton.getAction().getActions().at(0).getTask().parseToJson();
//    }


return 0;
    for (i = 0; i < QTD_SERVICOS; i++){
        rc = pthread_create(&threads[i], NULL, ListenThreadPool::Manage, (void*) &portas[i]);
        if (rc < 0){
            printf(buffer, "ERROR: Erro ao criar a thread na porta %d\n%s\n", portas[i], strerror(errno));
            cout << buffer;
            return -1;
        }
	    usleep(500);
        //getchar();d
    }

    pthread_exit(NULL);

    return 0;
}
