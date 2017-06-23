#include "Util.h"

namespace util {

int GetLircSocket(){
    int lircSock;
//    if (lircSock < 0){
        lircSock = lirc_get_local_socket(LIRCD_SOCKET_PATH, 0);

        if (lircSock < 0){
            char buffer[255];
            sprintf(buffer, "ERRO ao criar o socket LIRC %d\n%s\n", lircSock, strerror(errno));
            cout << buffer;
            return -1;
        }
//    }

    return lircSock;
}

bool deleteFromFile(fstream& file, int pos, string fileName){
    fstream tempFile("tempFile", fstream::out);
    file.clear();
    file.seekg(0,ios::beg);

    string line;
    int a = 0;

    while(!file.eof()){
        getline(file, line);
        if(file.eof()) break;
        if (a++!=pos){
            tempFile<<line<<std::endl;
        }
    }
    tempFile.close();
    file.close();
    remove(fileName.c_str());
    rename("tempFile", fileName.c_str());

    return true;

}

bool saveToFile(fstream& file, string saveString, int pos, string fileName){

    file.clear();
    file.seekg(0,ios::beg);
    string line;

    if (pos == -1 ){ //salvar no final do arquivo
        file.seekg(0,ios::end);
        file.clear();
        file << saveString << std::endl;
    } else {
        fstream tempFile("tempFile", fstream::out);

        int a = 0;
        while(!file.eof()){
            getline(file, line);
            if(file.eof()) break;
            if (a++==pos){
                tempFile<<saveString << std::endl;
            } else {
                tempFile<<line<<std::endl;
            }
        }
        tempFile.close();
        file.close();
        remove(fileName.c_str());
        rename("tempFile", fileName.c_str());
    }

    return true;
}






}
