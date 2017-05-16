
#include "Util.h"

namespace util {

static bool CreateLircSocket(){
//    lircSock = lirc_get_local_socket(LIRCD_SOCKET_PATH, 0);
//
//    if (lircSock < 0){
//        char[255] buffer;
//        sprintf(buffer, "ERRO ao criar o socket LIRC %d\n%s\n", lircSock, strerror(errno));
//        cout << buffer;
//        return false;
//    }

    return true;
}



string ParseToJason(Task task){
    StaticJsonBuffer<200> jsonBuffer;

    JsonObject& root = jsonBuffer.createObject();
    root["deviceName"] = task.getDeviceName();
    root["buttonName"] = task.getButtonName();
    root["mode"] = task.getMode();

    string lala;
    root.printTo(lala);
    return lala;
    // This prints:
    // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
}
}
