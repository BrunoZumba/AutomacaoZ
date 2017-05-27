#include<iostream>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<iostream>
#include <string.h> //Usado pelo bzero
#include<arpa/inet.h> //usar inet_ntoa
#include<errno.h>

#include <unistd.h>

#define BUFFER_SIZE 255

using namespace std;

int main(){
    int sock, port, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUFFER_SIZE] = "Inicializando";


    cout<<"Insira a porta: ";
    cin.getline(buffer, 50);
    port = atoi(buffer);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server = gethostbyname("192.168.25.50");
    if (server == NULL){
        cout<<"Não foi possivel encontrar o servidor\n";
        return 1;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
      (char *)&serv_addr.sin_addr.s_addr,
      server->h_length);
    serv_addr.sin_port = htons(port);



    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr))<0){
        cout<<"Erro ao conectar: '" << strerror(errno) << "'\n";
        return 2;
    }

//    cout << "digite a msg: \n";
//    bzero(buffer, BUFFER_SIZE);
//    cin.getline(buffer, BUFFER_SIZE);
//
//    int len = strlen(buffer);
//    buffer[len++]='\n';
//    buffer[len++]='\n';
//    buffer[len]='\0';
//    n = write(sock, buffer, strlen(buffer));
//    cout << "n: " << n << "\n";
//    if (n<0){
//        cout<<"Erro ao escrever\nERRNO: " << strerror(errno) << "\n";
//        exit;
//    }

    cout<<"Aguardando inicio da transmissão com o servidor...\n";

    while (strcmp(buffer, "FECHAR") != 0){
        bzero(buffer, BUFFER_SIZE);



        cout <<"Escreva uma mensagem para mandar para o servidor: '"<< inet_ntoa(serv_addr.sin_addr)<<"' no socket: " << sock << ": ";
        bzero(buffer, BUFFER_SIZE);
        cin.getline(buffer, BUFFER_SIZE);

        n = write(sock, buffer, strlen(buffer));
        if (n<0){
            cout<<"Erro ao escrever\nERRNO: " << strerror(errno) << "\n";
            break;
        }


        bzero(buffer, BUFFER_SIZE);
        cout << "esperando read \n";
        n = read(sock, buffer, BUFFER_SIZE);
        if (n<0) {
            cout<<"Erro na leitura do socket\nERRNO: " << strerror(errno) << "\n";
            break;
        }

        cout << "Mensagem recebida do servidor '" << inet_ntoa(serv_addr.sin_addr) << "': \n'" << buffer <<"'\n";

    }

    cout << "Fechando o socket com o servidor '" << inet_ntoa(serv_addr.sin_addr)  <<"'\n";
    close(sock);





    return 0;
}
