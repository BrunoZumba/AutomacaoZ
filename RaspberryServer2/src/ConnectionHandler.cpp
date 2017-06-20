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

    //Indica se a tarefa é um shutdown. Possui um tratamento especial por desligar o Raspberry Pi
    bool shutdownTask = false;

    char buffer[BUFFER_SIZE] = "Inicializando";

    while (true){
        bzero(buffer, BUFFER_SIZE);
        n = read(sock, buffer, BUFFER_SIZE);

        //cout << "N é igual a: " << n << "\n";

        if (n == 0) {
            break;
        }
        if (n < 0){
            bzero(buffer, BUFFER_SIZE);
            sprintf(buffer, "ERRO ao ler no socket %d na porta %d: %s\n", sock, port, strerror(errno));
            cout << buffer;
            break;
        }
        cout << "Comando recebido do Cliente: " << buffer << "\n";

        cout << "Port: " << port <<"\n";

        switch(port){
			case 4391: {
			    CommandTask commandTask;

			    if (!commandTask.ParseRequestFromJason(buffer)){
					commandTask.createResponse(STATUS_ERROR, "responseCommand", "Erro ao interpretar a mensagem JSON", "");
				} else {
                    commandTask.execute();
				}

                bzero(buffer, BUFFER_SIZE);
				sprintf(buffer, commandTask.ParseResponseToJason().c_str());

			break;}
			case 8742:{
				SensorTask sensorTask;
				if (!sensorTask.ParseRequestFromJason(buffer)){
					sensorTask.createResponse(STATUS_ERROR, "responseSensor", "Erro ao interpretar a mensagem JSON", "");
				} else {
                    sensorTask.execute();
				}

                bzero(buffer, BUFFER_SIZE);
				sprintf(buffer, sensorTask.ParseResponseToJason().c_str());
			break;}
			case 8168:{
                SystemActionTask systemActionTask;
				if (!systemActionTask.ParseRequestFromJason(buffer)){
					systemActionTask.createResponse(STATUS_ERROR, "responseSystemAction", "Erro ao interpretar a mensagem JSON", "");
				} else {
				    shutdownTask = systemActionTask.execute();

				}

                bzero(buffer, BUFFER_SIZE);
				sprintf(buffer, systemActionTask.ParseResponseToJason().c_str());
			}
			break;
			case 5223:{
                RecurringActionTask recurringActionTask;


			}
		}

        //cout << buffer << "\n";d
        cout<< "Resposta: " << buffer <<"\n";

        n = write(sock, buffer, strlen(buffer));
        if (n < 0) {
            bzero(buffer, BUFFER_SIZE);
            sprintf(buffer, "ERRO ao escrever no socket %d na porta %d: %s\n", sock, port, strerror(errno));
            cout << buffer;
            break;
        }

        if (shutdownTask){
            //Se o SHUTDOWN fosse feito no SystemTask.execute, o shutdown seria feito antes de enviar a resposta
            //portnato o SystemTask.execute apenas formatava a resposta, aguarda ela ser enviada e depois dá o SHUTDOWN
            system("sudo shutdown -P now");
//            system("netstat -na | grep 8168");
        }
    }

    cout << "Fechando conexão com o host \'"<<inet_ntoa(clientAddr.sin_addr)<<"\' no socket " << sock << " na porta "<<port<<".\n";
    close(sock);

}

