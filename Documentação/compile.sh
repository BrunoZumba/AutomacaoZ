#!/bin/bash
MAIN_PATH="/home/bruno/Documentos/AutomacaoZ/trunk/RaspberryServer2"
#MAIN_PATH="/home/pi/Desktop/AutomacaoZ.git/trunk/RaspberryServer2"


g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/main.cpp -o $MAIN_PATH/obj/Debug/main.o
g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/ConnectionHandler.cpp -o $MAIN_PATH/obj/Debug/src/ConnectionHandler.o
g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/Job.cpp -o $MAIN_PATH/obj/Debug/src/Job.o
g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/ListenThreadPool.cpp -o $MAIN_PATH/obj/Debug/src/ListenThreadPool.o
g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/Util.cpp -o $MAIN_PATH/obj/Debug/src/Util.o

g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/AppClasses/ActionButtonClass.cpp -o $MAIN_PATH/obj/Debug/src/AppClasses/ActionButtonClass.o
g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/AppClasses/ActionClass.cpp -o $MAIN_PATH/obj/Debug/src/AppClasses/ActionClass.o
g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/AppClasses/RecurringActionTask.cpp -o $MAIN_PATH/obj/Debug/src/AppClasses/RecurringActionTask.o
g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/AppClasses/TaskButtonClass.cpp -o $MAIN_PATH/obj/Debug/src/AppClasses/TaskButtonClass.o
g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/AppClasses/TaskClass.cpp -o $MAIN_PATH/obj/Debug/src/AppClasses/TaskClass.o

g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/CommandClasses/Command.cpp -o $MAIN_PATH/obj/Debug/src/CommandClasses/Command.o
g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/CommandClasses/RecurringActionCommand.cpp -o $MAIN_PATH/obj/Debug/src/CommandClasses/RecurringActionCommand.o
g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/CommandClasses/SensorCommand.cpp -o $MAIN_PATH/obj/Debug/src/CommandClasses/SensorCommand.o
g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/CommandClasses/SystemActionCommand.cpp -o $MAIN_PATH/obj/Debug/src/CommandClasses/SystemActionCommand.o
g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/CommandClasses/TaskCommand.cpp -o $MAIN_PATH/obj/Debug/src/CommandClasses/TaskCommand.o
g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/CommandClasses/SensorCommand.cpp -o $MAIN_PATH/obj/Debug/src/CommandClasses/SensorCommand.o
g++ -Wall -g -I/usr/local/lib -I$MAIN_PATH/include -I$MAIN_PATH/include/AppClasses -I$MAIN_PATH/src/AppClasses -I$MAIN_PATH/include/CommandClasses -c $MAIN_PATH/src/CommandClasses/SensorCommand.cpp -o $MAIN_PATH/obj/Debug/src/CommandClasses/SensorCommand.o

g++ -o $MAIN_PATH/bin/Debug/RaspberryServer2 $MAIN_PATH/obj/Debug/main.o $MAIN_PATH/obj/Debug/src/AppClasses/ActionButtonClass.o $MAIN_PATH/obj/Debug/src/AppClasses/ActionClass.o $MAIN_PATH/obj/Debug/src/AppClasses/RecurringActionTask.o $MAIN_PATH/obj/Debug/src/AppClasses/TaskButtonClass.o $MAIN_PATH/obj/Debug/src/AppClasses/TaskClass.o $MAIN_PATH/obj/Debug/src/CommandClasses/Command.o $MAIN_PATH/obj/Debug/src/CommandClasses/RecurringActionCommand.o $MAIN_PATH/obj/Debug/src/CommandClasses/SensorCommand.o $MAIN_PATH/obj/Debug/src/CommandClasses/SystemActionCommand.o $MAIN_PATH/obj/Debug/src/CommandClasses/TaskCommand.o $MAIN_PATH/obj/Debug/src/ConnectionHandler.o $MAIN_PATH/obj/Debug/src/Job.o $MAIN_PATH/obj/Debug/src/ListenThreadPool.o $MAIN_PATH/obj/Debug/src/Util.o -lpthread /usr/local/lib/liblirc_client.so

