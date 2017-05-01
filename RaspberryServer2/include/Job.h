#ifndef JOB_H
#define JOB_H

#include<iostream>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

using namespace std;

#define BUFFER_SIZE 255


class Job
{
    public:
        Job(int, int);
        virtual ~Job();
        void working();

    protected:

    private:
        int sock2;
        int port2;
};

#endif // JOB_H
