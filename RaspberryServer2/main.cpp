#include<stdio.h>
#include<pthread.h>
#include<iostream>
#include<errno.h>   //usado pela errno
#include<string.h>  //usado pela strerror

#include"include/ListenThreadPool.h"
#include"include/Util.h"
#include "ArduinoJson.h"
#include "Util.h"

using namespace std;

#define QTD_SERVICOS 2



int main(){
    int portas[QTD_SERVICOS] = {4391, 8742};
    pthread_t threads[QTD_SERVICOS];
    int i, rc;
    char buffer[BUFFER_SIZE];

    /**
     * O thread pool são apenas threads que ficam aguardando novas tarefas.
     * Quando uma tarefa chega, uma thread a pega, trata, delete e volta a esperar novas.
     */
    ListenThreadPool::createThreadPool();

    for (i = 0; i < QTD_SERVICOS; i++){
        rc = pthread_create(&threads[i], NULL, ListenThreadPool::Manage, (void*) &portas[i]);
        if (rc < 0){
            printf(buffer, "ERROR: Erro ao criar a thread na porta %d\n%s\n", portas[i], strerror(errno));
            cout << buffer;

            return -1;
        }
        //getchar();
    }

    pthread_exit(NULL);

    return 0;
}
