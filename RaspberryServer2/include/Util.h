#ifndef UTIL_H
#define UTIL_H

#include "lirc_client.h"
#include "ArduinoJson.h"
#include<string.h>
#include<iostream>
#include <fstream>
#include <pqxx/pqxx>


#define LIRCD_SOCKET_PATH "/var/run/lirc/lircd"

#define BUFFER_SIZE 2048

#define RECURRING_ACTION_FILE "recurringActionFile"
#define ACTION_BUTTON_FILE "actionButtonFile"

using namespace std;
using namespace pqxx;

namespace util {

//static int lircSock = -1;


int GetLircSocket();

bool saveToFile(fstream&, string, int, string);
bool deleteFromFile(fstream&, int, string);
bool openFile(string, fstream&);


//connection getConnection();

}

#endif // UTIL_H
