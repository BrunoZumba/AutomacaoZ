#include "Job.h"


struct jobStruct {
    int sock;
    int port;

};

struct jobStruct st;

Job::Job(int _sock, int _port){
    st.sock = _sock;
    st.port = _port;

    sock2 = _sock;
    port2 = _port;
}

Job::~Job(){}


void Job::working(){
    int sock = sock2;
    int port = port2;

    int i = 0, n = 1;
    cout << "Socket: " << sock << ". Porta: " << port << "\n";


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

        cout << "Mensagem recebida no socket "<< sock <<" na porta "<<port<<": "<<buffer <<"\n";
        i++;
    }

    cout << "Fechando conexão do socket " << sock << " na porta "<<port<<".\n";
    close(sock);

}
