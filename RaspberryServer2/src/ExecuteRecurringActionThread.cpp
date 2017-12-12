#include "ExecuteRecurringActionThread.h"

ExecuteRecurringActionThread::ExecuteRecurringActionThread(){}

ExecuteRecurringActionThread::~ExecuteRecurringActionThread(){}

void ExecuteRecurringActionThread::funcao(int sig){
    cout<<"passou na fç do signal\n";
    usleep(3000000);
}

RecurringActionClass ExecuteRecurringActionThread::getNextAction(){
    fstream file;
    if (!util::openFile(RECURRING_ACTION_FILE, file)){
        //Nao foi possível verificar proxima ação
    }

    string line;
    RecurringActionClass nextRecAct;
    RecurringActionClass currentRecAct;

    while(!file.eof()){
        RecurringActionClass currentRecAct;
        getline(file, line);
        //Coloca esse break aqui para evitar que ele leia o caractere de EOF ainda considerando como uma parte do array
        if(file.eof()) break;

        currentRecAct.createFromJson(line);

        if (nextRecAct.getRecActName() == ""){ //se for a primeira iteração, nextRecAct = currentRecAct
            nextRecAct = currentRecAct;
        } else {
            //o array de Dates esta em ordem crescente, entao so precisa verificar o primeiro elemento
            if(nextRecAct.getRecActDates().at(0) > currentRecAct.getRecActDates().at(0)){
                nextRecAct = currentRecAct;
            } else if (nextRecAct.getRecActDates().at(0) == currentRecAct.getRecActDates().at(0)){
            //se as datas do primeiro elemento do array forem iguais, analisa os horarios de execução, que também estao esm ordem crescente
                if(nextRecAct.getRecActTimes().at(0) > currentRecAct.getRecActTimes().at(0)){
                    nextRecAct = currentRecAct;
                }
            }
        }
    }

    return nextRecAct;

}

void *ExecuteRecurringActionThread::Execute(void * parm){
    signal(SIGUSR1, funcao);

    RecurringActionClass nextRecAct = getNextAction();
    cout<<"Proxima Ação a ser executada: "<<nextRecAct.getRecActName();

}

