
#include "Util.h"


namespace util {

int GetLircSocket(){
    if (lircSock < 0){
        lircSock = lirc_get_local_socket(LIRCD_SOCKET_PATH, 0);

        if (lircSock < 0){
            char buffer[255];
            sprintf(buffer, "ERRO ao criar o socket LIRC %d\n%s\n", lircSock, strerror(errno));
            cout << buffer;
            return -1;
        }
    }

    return lircSock;
}






}
