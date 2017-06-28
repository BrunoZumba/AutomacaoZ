 /*
 *  dht11.c:
 *	Simple test program to test the wiringPi functions
 *	DHT11 test
 */
 
#include <wiringPi.h>
#include <unistd.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include "/home/pi/Desktop/AutomacaoZ.git/trunk/RaspberryServer2/include/rc-switch/RCSwitch.h"
#define PIN		28


using namespace std;

int main( void ) {
	int code = 8080;
	int pulseLength = 0;

	if (wiringPiSetup () == -1) return 1;
	RCSwitch mySwitch = RCSwitch();
	mySwitch.setProtocol(0); //default protocol
//	mySwitch.setPulseLength(pulseLength); //default length
	if (pulseLength != 0) mySwitch.setPulseLength(pulseLength);
	mySwitch.enableTransmit(PIN);

	mySwitch.send(code, 24);

}
