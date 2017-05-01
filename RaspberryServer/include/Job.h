#ifndef JOB_H
#define JOB_H

#include<iostream>

using namespace std;


class Job
{
    public:
        Job(int, int);
        virtual ~Job();
        void working();

    protected:

    private:
};

#endif // JOB_H
