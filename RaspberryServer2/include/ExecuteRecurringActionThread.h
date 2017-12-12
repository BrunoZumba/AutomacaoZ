#ifndef EXECUTERECURRINGACTIONTHREAD_H
#define EXECUTERECURRINGACTIONTHREAD_H


#include<string.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<iostream>

#include<signal.h>

#include "Util.h"
#include "RecurringActionClass.h"


using namespace std;


class ExecuteRecurringActionThread
{
    public:
        ExecuteRecurringActionThread();
        virtual ~ExecuteRecurringActionThread();

        static void *Execute(void *);
        static void funcao(int);
        static RecurringActionClass getNextAction();

    protected:

    private:
};

#endif // EXECUTERECURRINGACTIONTHREAD_H
