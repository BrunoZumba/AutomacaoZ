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
#include "RecurringActionClass.h"
#include "TaskButtonClass.h"
#include "ActionClass.h"
#include "ActionButtonClass.h"
#include "RecurringActionCommand.h"
#include "ActionButtonCommand.h"

#include <fstream>

using namespace std;

#define QTD_SERVICOS 5



int main(){
    int portas[QTD_SERVICOS] = {4391, 8742, 8168, 5223,6292};
    pthread_t threads[QTD_SERVICOS];
    int i, rc;
    char buffer[BUFFER_SIZE];

//    string actBtCmdBuffer = "{\"requestAction\":\"saveList\",\"requestOverwrite\":true,\"actionButton\":{\"actionName\":\"aço\",\"action\":[{\"buttonId\":\"111\",\"task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_EXIT\",\"mode\":\"SEND_ONCE\"}},{\"buttonId\":\"2131558569\",\"task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_POWER\",\"mode\":\"SEND_ONCE\"}}]}}";
//    string actBtCmdBuffer1 = "{\"requestAction\":\"getList\",\"requestOverwrite\":false,\"actionButton\":{}}";
//    ActionButtonCommand actionButtonCommand = ActionButtonCommand(actBtCmdBuffer1);
//    if(actionButtonCommand.createRequestFromJson()){
//        cout<<"\nCriou ActBt\n";
//        if(actionButtonCommand.execute()){
//            cout<<"Sucesso\n";
////            cout<<actionButtonCommand.getResponseAction()<<"\n";
//            cout<<"GetResponseParm: '"<<actionButtonCommand.getResponseParm()<<"'\n";
//            cout<<"ParseResponseJson: '"<<actionButtonCommand.ParseResponseToJason().c_str()<<"'\n";
//        }
//    }


//    string recurringActionBuffer = "{\"requestAction\":\"saveList\",\"requestOverwrite\":false,\"recurringAction\":{\"name\":\"888\",\"dates\":[000,000,000],\"times\":[1497808731, 1111111,2222],\"actionButton\":{\"actionName\":\"aço\",\"action\":[{\"buttonId\":\"2131558568\",\"task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_EXIT\",\"mode\":\"SEND_ONCE\"}},{\"buttonId\":\"2131558569\",\"task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_POWER\",\"mode\":\"SEND_ONCE\"}}]}}}";
//    string recurringActionBuffer2 = "{\"requestAction\":\"getList\",\"requestOverwrite\":,\"recurringAction\":{}}";
//    RecurringActionCommand recurringActionCommand = RecurringActionCommand(recurringActionBuffer2);
//    if(recurringActionCommand.createRequestFromJson()){
//        cout<<"\nCriou Recurring Action\n";
//        if (recurringActionCommand.execute()){
//            cout<<"Executou com sucesso\n";
//            cout<<recurringActionCommand.getResponseAction()<<"\n";
//            cout<<recurringActionCommand.getResponseParm();
//        }
//    }

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





//    vector<int> lala;
//    lala.insert(lala.end(), 1);
//    lala.insert(lala.end(), 2);
//    for (int i = 0; i < lala.size(); i++){
//        cout << i << ": " <<lala.at(i) << "\n";
//    }
//

//////    string buffer1 = "{\"name\":\"f\",\"dates\":[\"123\",\"456\",\"789\"],\"times\":[\"1497808731\", \"1111111\",\"2222\"],\"actionButton\":{\"actionName\":\"aço\",\"action\":[{\"buttonId\":\"2131558568\",\"task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_EXIT\",\"mode\":\"SEND_ONCE\"}},{\"buttonId\":\"2131558569\",\"task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_POWER\",\"mode\":\"SEND_ONCE\"}}]}}";
//////    RecurringActionClass recurringActionCommand = RecurringActionClass(buffer1);
//////    if(recurringActionCommand.createRequestFromJson()){
//////        cout<< "Name: " << recurringActionCommand.getRecurringAction().getName();
//////        for(int i = 0; i < recurringActionCommand.getRecurringAction().getDates().size(); i++){
//////            cout<<"\nData["<<i<<"]:" << recurringActionCommand.getRecurringAction().getDates().at(i);
//////        }
//////        for(int i = 0; i < recurringActionCommand.getTimes().size(); i++){
//////            cout<<"\nTime["<<i<<"]:" << recurringActionCommand.getTimes().at(i);
//////        }
//////        cout<<"\nAction: " << recurringActionCommand.getActionButton().parseToJson();
//////    }



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
//        cout<<"\nButtonId: "<< taskButton.getButtonId();
//        cout<<"\ntask: "<< taskButton.getTask().parseToJson();
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

   /**
     * O thread pool são apenas threads que ficam aguardando novas tarefas.
     * Quando uma tarefa chega, uma thread a pega, trata, delete e volta a esperar novas.
     */
    ListenThreadPool::createThreadPool();

//return 0;
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
