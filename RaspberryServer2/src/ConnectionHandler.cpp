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
    int i = 0, n = 1;


    char buffer[BUFFER_SIZE] = "Inicializando";

    while (n > 0){
        bzero(buffer, BUFFER_SIZE);
        sprintf(buffer, "Iteracao %d do socket %d na porta %d", i, sock, port);
        //cout << buffer << "\n";

        n = write(sock, buffer, strlen(buffer));
        if (n < 0) {
            bzero(buffer, BUFFER_SIZE);
            sprintf(buffer, "ERRO ao escrever no socket %d na porta %d: %s\n", sock, port, strerror(errno));
            cout << buffer;
            break;
        }

        bzero(buffer, BUFFER_SIZE);
        n = read(sock, buffer, BUFFER_SIZE);

        if (n < 0){
            bzero(buffer, BUFFER_SIZE);
            sprintf(buffer, "ERRO ao ler no socket %d na porta %d: %s\n", sock, port, strerror(errno));
            cout << buffer;
            break;
        }

        //sprintf(buffer, "Msg rcv do host \'%s\' no socket %d na porta  %d\n", , sock, port);
        cout << "Msg rcv do host \'" << inet_ntoa(clientAddr.sin_addr) << "\' no socket "<< sock <<" na porta "<<port<<": "<<buffer <<"\n";
        i++;
    }

    cout << "Fechando conexão com o host \'"<<inet_ntoa(clientAddr.sin_addr)<<"\' no socket " << sock << " na porta "<<port<<".\n";
    close(sock);

}

