#ifndef UTIL_H
#define UTIL_H

//#include<"lirc_client.h>
#include "ArduinoJson.h"
#include "Task.h"
#include<string.h>


#define LIRCD_SOCKET_PATH "/var/run/lirc/lircd"
static int lircSock;

using namespace std;

namespace util {

static bool CreateLircSocket();



string ParseToJason(Task);
}

#endif // UTIL_H
