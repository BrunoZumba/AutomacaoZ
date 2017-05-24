#!/bin/bash
MAIN_PATH="/home/bruno/Documentos/AutomacaoZ/trunk/RaspberryServer2"

g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -c $MAIN_PATH/src/ConnectionHandler.cpp -o $MAIN_PATH/obj/Debug/src/ConnectionHandler.o

g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -c $MAIN_PATH/src/Task.cpp -o $MAIN_PATH/obj/Debug/src/Task.o

g++ -o $MAIN_PATH/bin/Debug/RaspberryServer2 $MAIN_PATH/obj/Debug/main.o $MAIN_PATH/obj/Debug/src/ConnectionHandler.o $MAIN_PATH/obj/Debug/src/Job.o $MAIN_PATH/obj/Debug/src/ListenThreadPool.o $MAIN_PATH/obj/Debug/src/Response.o $MAIN_PATH/obj/Debug/src/Task.o $MAIN_PATH/obj/Debug/src/Util.o   -lpthread /usr/local/lib/liblirc_client.so -lpthread


