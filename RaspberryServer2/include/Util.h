#ifndef UTIL_H
#define UTIL_H

#include "lirc_client.h"
#include "ArduinoJson.h"
#include<string.h>
#include<iostream>
#include <fstream>


#define LIRCD_SOCKET_PATH "/var/run/lirc/lircd"

#define BUFFER_SIZE 2048


using namespace std;

namespace util {

//static int lircSock = -1;


int GetLircSocket();

bool saveToFile(fstream&, string, int, string);
bool deleteFromFile(fstream&, int, string);

}

#endif // UTIL_H
