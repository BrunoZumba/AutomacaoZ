#include<stdio.h>
#include<pthread.h>
#include<iostream>
#include<errno.h>   //usado pela errno
#include<string.h>  //usado pela strerror

#include"include/ListenThreadPool.h"

using namespace std;

#define QTD_SERVICOS 3



int main(){
    int portas[QTD_SERVICOS] = {8180,8280,8380};
    pthread_t threads[QTD_SERVICOS];
    int i, rc;
    char buffer[255];


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
