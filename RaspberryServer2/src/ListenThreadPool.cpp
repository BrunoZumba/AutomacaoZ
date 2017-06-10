#include "ListenThreadPool.h"

ListenThreadPool::ListenThreadPool(){}

ListenThreadPool::~ListenThreadPool(){}

queue<ConnectionHandler*> connectionHandlerQueue;
pthread_mutex_t queueLock;
pthread_cond_t queueCond;


int prepareSocket(int );
void createThreadPool();
void *executeThread(void *);

/**
 * Thread "infinita" que fica no loop escutando na porta especificada.
 *
 * Quando uma conexão chega nesta porta (independente da porta) esta thread coloca esta
 * conexão na fila para as threads do ThreadPool tratarem
 */
void *ListenThreadPool::Manage(void * parm){
    int port = *((int *) parm);
    int listenSocket, newSocket;
    struct sockaddr_in clientAddr;
    socklen_t clientSize = sizeof(clientAddr);
    char msg[255];

    listenSocket = prepareSocket(port);
    if (listenSocket < 0){
        sprintf(msg, "ERRO ao preparar o socket na porta %d.\n%s\n", port, strerror(errno));
        cout<<msg;
        pthread_exit(NULL);
    }
    cout<<"Criada Thread na porta " << port << "\n";


    while (true){
        newSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&clientSize);
        if(newSocket < 0){
            sprintf(msg, "ERRO ao aceitar a conexao no socket %d (porta %d)\n%s\n", listenSocket, port, strerror(errno));
            cout << msg;
            exit (-3);
        }

        sprintf(msg, "O client %s se conectou na porta %d pelo socket %d\n", inet_ntoa(clientAddr.sin_addr), port, newSocket);
        cout << msg;

        //Quando ha nova conexao, cria-se um novo job, o coloca na fila
        ConnectionHandler *connectionHandler = new ConnectionHandler(newSocket, port, clientAddr);

        pthread_mutex_lock(&queueLock);
        connectionHandlerQueue.push(connectionHandler);
        pthread_mutex_unlock(&queueLock);
        pthread_cond_signal(&queueCond);
    }

    pthread_exit(NULL);

}

/**
 * Esta thread é o coração do thread pool. Cada thread do pool executa esta função
 *
 * Ela fica parada esperando uma nova tarefa estar disponível na fila.
 */
void *executeThread(void *){
    cout << "Passando por execute Thread\n";
    ConnectionHandler *oneConnectionHandler = NULL;

    while (true){
        pthread_mutex_lock(&queueLock);
        while (connectionHandlerQueue.empty())
            pthread_cond_wait(&queueCond, &queueLock);

        oneConnectionHandler = connectionHandlerQueue.front();
        connectionHandlerQueue.pop();
        pthread_mutex_unlock(&queueLock);

        if (oneConnectionHandler)
            oneConnectionHandler->working();
        delete oneConnectionHandler;
        oneConnectionHandler = NULL;
    }
    pthread_exit(NULL);
}


//Cria a quantidade de threads especificada para trabalhar
void ListenThreadPool::createThreadPool(){
    cout << "Creating Thread Pool\n";
    int i;
    pthread_t threads[THREAD_POOL_SIZE];

    for (i = 0; i < THREAD_POOL_SIZE; i++){
        pthread_create(&threads[i], NULL, executeThread, NULL);
    }
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
