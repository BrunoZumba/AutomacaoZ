#include "ListenThreadPool.h"

ListenThreadPool::ListenThreadPool(){}

ListenThreadPool::~ListenThreadPool(){}

queue<Job*> jobQueue;
pthread_mutex_t queueLock;
pthread_cond_t queueCond;


int prepareSocket(int );
void createThreadPool();
void *executeThread(void *);

void *ListenThreadPool::Manage(void * parm){
    int port = *((int *) parm);
    int listenSocket, newSocket;
    struct sockaddr_in clientAddr;
    socklen_t clientSize = sizeof(clientAddr);
    char msg[255];

    //cout << "Porta: " << port << "\n";


    createThreadPool();


    listenSocket = prepareSocket(port);
    if (listenSocket < 0){
        printf(msg, "ERRO ao preparar o socket.\n%s\n", strerror(errno));
        cout<<msg;
        exit (-1);
    }

    newSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&clientSize);
    if(newSocket < 0){
        printf(msg, "ERRO ao aceitar a conexao no socket %d (porta %d)\n%s\n", listenSocket, port, strerror(errno));
        cout << msg;
        exit (-3);
    }

    Job *job = new Job(newSocket, port);

    pthread_mutex_lock(&queueLock);
    jobQueue.push(job);
    pthread_mutex_unlock(&queueLock);
    pthread_cond_signal(&queueCond);


}

void createThreadPool(){
    int i;
    pthread_t threads[THREAD_POOL_SIZE];

    for (i = 0; i < THREAD_POOL_SIZE; i++){
        pthread_create(&threads[i], NULL, executeThread, NULL);
    }
}

void *executeThread(void *){
    Job *oneJob = NULL;

    pthread_mutex_lock(&queueLock);
    while (jobQueue.empty())
        pthread_cond_wait(&queueCond, &queueLock);

    oneJob = jobQueue.front();
    jobQueue.pop();
    pthread_mutex_unlock(&queueLock);

    if (oneJob)
        oneJob->working();
    delete oneJob;
    oneJob = NULL;
}



//Abre o socket para escutar na porta especifica
int prepareSocket(int porta){
    int sock, rc;
    struct sockaddr_in serverAddr;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock < 0) return sock;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(porta);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    rc = bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    if (rc < 0) {
        if (sock) close(sock);
        return rc;
    }

    rc = listen(sock, LISTEN_QUEUE);

    if (rc < 0){
        if (sock) close(sock);
        return rc;
    }

    return sock;
}
