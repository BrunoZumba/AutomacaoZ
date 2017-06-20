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

using namespace std;

#define QTD_SERVICOS 4



int main(){
    int portas[QTD_SERVICOS] = {4391, 8742, 8168, 5223};
    pthread_t threads[QTD_SERVICOS];
    int i, rc;
    char buffer[BUFFER_SIZE];

    /**
     * O thread pool são apenas threads que ficam aguardando novas tarefas.
     * Quando uma tarefa chega, uma thread a pega, trata, delete e volta a esperar novas.
     */
//    ListenThreadPool::createThreadPool();

    RecurringActionTask recurringActionTask;
    string buffer1 = "{\"name\":\"f\",\"dates\":[\"123\",\"456\",\"789\"],\"times\":[\"1497808731\", \"1111111\",\"2222\"],\"actionButton\":{\"actionName\":\"aço\",\"action\":[{\"ID\":\"2131558568\",\"Task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_EXIT\",\"mode\":\"SEND_ONCE\"}},{\"ID\":\"2131558569\",\"Task\":{\"deviceName\":\"ControleNet\",\"buttonName\":\"KEY_POWER\",\"mode\":\"SEND_ONCE\"}}]}}";

    recurringActionTask.ParseRequestFromJason(buffer1);

    cout << "Name: " << recurringActionTask.getName() << "\n";
    while(!recurringActionTask.times.empty()){
        cout << "Time: " << recurringActionTask.times.front() << "\n";
        recurringActionTask.times.pop();
    }
    cout << "size>: " << recurringActionTask.times.size();
    while(!recurringActionTask.dates.empty()){
        cout << "Dates: " << recurringActionTask.dates.front() << "\n";
        recurringActionTask.dates.pop();
    }
    cout << "size>: " << recurringActionTask.dates.size();

    while(!recurringActionTask.commandTasks.empty()){
        cout <<"Dev Name: " << recurringActionTask.commandTasks.front().getDeviceName() << "\n";
        cout <<"BT Name: " << recurringActionTask.commandTasks.front().getButtonName() << "\n";
        cout <<"mode: " << recurringActionTask.commandTasks.front().getMode() << "\n";
        recurringActionTask.commandTasks.pop();
    }

//    queue<long> dates = recurringActionTask.getDates();
//    cout << "\nDates: " << dates.size();
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
