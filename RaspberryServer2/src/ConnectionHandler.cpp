#include "ConnectionHandler.h"

ConnectionHandler::ConnectionHandler(int sockParm, int portParm, struct sockaddr_in clientAddrParm){

    _sock = sockParm;
    _port = portParm;
    _clientAddr = clientAddrParm;
}

ConnectionHandler::~ConnectionHandler(){}


void ConnectionHandler::working(){
    int sock = _sock;
    int port = _port;
    struct sockaddr_in clientAddr = _clientAddr;
    int n = 1;


    char buffer[BUFFER_SIZE] = "Inicializando";

    while (n > 0){
        bzero(buffer, BUFFER_SIZE);
        n = read(sock, buffer, BUFFER_SIZE);
        Response response;

        if (n < 0){
            bzero(buffer, BUFFER_SIZE);
            sprintf(buffer, "ERRO ao ler no socket %d na porta %d: %s\n", sock, port, strerror(errno));
            cout << buffer;
            break;
        }
        cout << "Comando recebido do Cliente: " << buffer << "\n";

        Task task;

        if (!task.ParseFromJason(buffer)){
            response.create(STATUS_ERROR, "Erro ao interpretar a mensagem JSON");
        }else if (!task.execute(util::GetLircSocket())){
            response.create(STATUS_ERROR, "Erro ao executar o comando");
        } else {
            response.create(STATUS_OK, "Comando executado com sucesso!");
        }

        bzero(buffer, BUFFER_SIZE);
        sprintf(buffer, response.ParseToJason().c_str());
        //cout << buffer << "\n";

        n = write(sock, buffer, strlen(buffer));
        if (n < 0) {
            bzero(buffer, BUFFER_SIZE);
            sprintf(buffer, "ERRO ao escrever no socket %d na porta %d: %s\n", sock, port, strerror(errno));
            cout << buffer;
            break;
        }
    }

    cout << "Fechando conexão com o host \'"<<inet_ntoa(clientAddr.sin_addr)<<"\' no socket " << sock << " na porta "<<port<<".\n";
    close(sock);

}

