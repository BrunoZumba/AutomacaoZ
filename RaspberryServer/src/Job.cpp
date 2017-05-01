#include "Job.h"


struct jobStruct {
    int sock;
    int port;

};

struct jobStruct st;

Job::Job(int _sock, int _port){
    st.sock = _sock;
    st.port = _port;
}

Job::~Job(){}


void Job::working(){

    cout << "Socket: " << st.sock << ". Porta: " << st.port << "\n";
}
