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


bool openFile(string filename, fstream& file){
    file.open(filename.c_str(), fstream::out | fstream::in);

    if (!file.is_open()) {
        //Testa se o arquivo existe. Se nao existir cria (apenas escrita ) e depois abre para escrita e leitura
        file.open(filename.c_str(), fstream::out);
        file.close();
        file.open(filename.c_str(), fstream::in | fstream::out);
    }
    if (!file.is_open()){
        //Resposta em caso de erro ao abrir o arquivo
        return false;
    }

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

//connection getConnection(){
//    try {
//          connection co("dbname = automacaozdb user = azdbuser password = teste123 hostaddr = 127.0.0.1 port = 5432");
//            cout<< "1"<<endl;
//          if (co.is_open()) {
//             cout << "Opened database successfully: " << co.dbname() << endl;
//          } else {
//             cout << "Can't open database" << endl;
//             return co;
//          }
//            cout<< "2"<<endl;
//          return co;
//       } catch (const std::exception &e) {
//          cerr << e.what() << std::endl;
//          return co;
//       }
//
//}







}
