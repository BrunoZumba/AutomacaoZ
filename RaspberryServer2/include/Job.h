#ifndef JOB_H
#define JOB_H

#include<iostream>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

using namespace std;

#define BUFFER_SIZE 255


class Job
{
    public:
        Job(int, int, struct sockaddr_in);
        virtual ~Job();
        void working();

    protected:

    private:
        int _sock;
        int _port;
        struct sockaddr_in _clientAddr;
};

#endif // JOB_H
