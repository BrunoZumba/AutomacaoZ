#include "SensorCommand.h"

SensorCommand::SensorCommand(string json) : Command(json){}
//SensorCommand::SensorCommand(){}

SensorCommand::~SensorCommand(){}

SensorClass SensorCommand::getSensor(){
    return this->sensor;
}

void SensorCommand::setSensor(SensorClass _sensor){
    this->sensor = _sensor;
}

bool SensorCommand::createRequestFromJson(){
    Command::createRequestFromJson();

    JsonObject& sensorObj = this->jsonRoot["sensor"];
    string sensorObjStr;
    sensorObj.printTo(sensorObjStr);
    this->sensor = SensorClass();
    this->sensor.createFromJson(sensorObjStr);

    return true;


}

bool SensorCommand::execute(){
    #ifndef __arm__ //Verifica se o codigo esta rodando no RaspberryPi. Caso contrario, retorna false
        this->createResponse(STATUS_ERROR, /*"responseSensor",*/ "Necessário server estar no RPi para este serviço", "");
        return false;
    #endif // __arm__

    //Qtd de vezes que o servidor vai tentar pegar o valor do sensor
    int retry = 10;

    int dht11_dat[5] = { 0, 0, 0, 0, 0 };
    uint8_t laststate;
    uint8_t counter;
    uint8_t j, i;
    //float   f; /* fahrenheit */

    if ( wiringPiSetup() == -1 ){
        this->createResponse(STATUS_ERROR, /*"responseSensor",*/ "Erro ao iniciar a lib WiringPi", "");
        return false;
    }

    for (int a = 0; a < retry; a++){
        laststate       = HIGH;
        counter         = 0;
        j               = 0;
        dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

        /* pull pin down for 18 milliseconds */
        pinMode( DHTPIN, OUTPUT );
        digitalWrite( DHTPIN, LOW );
        delay( 18 );
        /* then pull it up for 40 microseconds */
        digitalWrite( DHTPIN, HIGH );
        delayMicroseconds( 40 );
        /* prepare to read the pin */
        pinMode( DHTPIN, INPUT );

        /* detect change and read data */
        for ( i = 0; i < MAXTIMINGS; i++ ){
            counter = 0;
            while ( digitalRead( DHTPIN ) == laststate ){
                    counter++;
                    delayMicroseconds( 1 );
                    if ( counter == 255 ){
                            break;
                    }
            }
            laststate = digitalRead( DHTPIN );

            if ( counter == 255 )
                    break;

            /* ignore first 3 transitions */
            if ( (i >= 4) && (i % 2 == 0) ){
                /* shove each bit into the storage bytes */
                dht11_dat[j / 8] <<= 1;
                if ( counter > 16 )
                        dht11_dat[j / 8] |= 1;
                j++;
            }
        }

        /*
         * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
         * print it out if data is good
         */
        if ( (j >= 40) && (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF))){
	    //f = dht11_dat[2] * 9. / 5. + 32;
            //printf( "Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n",dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
            stringstream ss;
            if (this->sensor.getSensorName().compare("temperatura") == 0) {
                ss << dht11_dat[2];
                this->createResponse(STATUS_OK, /*"responseSensor",*/ ss.str(), "");
            } else if (this->sensor.getSensorName().compare("umidade") == 0) {
                ss << dht11_dat[0];
                this->createResponse(STATUS_OK, /*"responseSensor",*/ ss.str(), "");
            } else {
                this->createResponse(STATUS_ERROR, /*"responseSensor",*/ "Sensor não reconhecido", "");
                return false;
            }
            return true;
        }
        usleep(200000);
    }

    this->createResponse(STATUS_ERROR, /*"responseSensor",*/ "Erro com os sensores", "");
    return false;
}

