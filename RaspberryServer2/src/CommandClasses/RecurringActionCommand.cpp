#include "RecurringActionCommand.h"

RecurringActionCommand::RecurringActionCommand(string json) : Command(json){}

RecurringActionCommand::~RecurringActionCommand(){}

bool RecurringActionCommand::createRequestFromJson(){
    Command::createRequestFromJson();

    JsonObject& recAction = this->jsonRoot["recurringAction"];
    string recActStr;
    recAction.printTo(recActStr);
    this->recurringAction = RecurringActionClass();
    this->recurringAction.createFromJson(recActStr);

    return true;
}

bool RecurringActionCommand::execute(){
    fstream file;
    if (!util::openFile(RECURRING_ACTION_FILE, file)){
        this->createResponse(STATUS_ERROR, /*"saveResponse",*/ "Erro ao criar o arquivo", "Erro ao acessar o HD");
        return false;
    }

    string line;
    DynamicJsonBuffer jsonBuffer;
    JsonArray& root = jsonBuffer.createArray();

    //percorre todo o arquivo salvando as ações recorrentes em um JsonArray
    while(!file.eof()){
        getline(file, line);
        //cout << "Line: " << line <<"\n";
        //Coloca esse break aqui para evitar que ele leia o caractere de EOF ainda considerando como uma parte do array
        if(file.eof()) break;

        const char* jsonDuplicate = jsonBuffer.strdup(line);
        root.add(jsonBuffer.parseObject(jsonDuplicate));
    }


    if(this->requestAction == "saveRecurringAction"){
        if (recurringActionDAO::insertRecurringAction(this->getRecurringAction()) > 0){
            this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação Recorrente inserida com sucesso","");
            return true;
        } else {
            this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Já existe Ação Recorrente com este nome","requestOverwrite");
            return false;
        }

    } else if(this->requestAction == "getList"){
        vector<RecurringActionClass> recurringActions = recurringActionDAO::getAllRecurringActions();

        if(recurringActions.size() <= 0){
            this->createResponse(STATUS_OK, /*"getListResponse",*/ "Não há Ações Recorrentes cadastradas", "[]");
            return false;
        }

        //Transforma o vector<RecurringActionClass> em um JsonArray
        string arrayStr = "[";
        for (unsigned int i = 0; i<recurringActions.size(); i++){
            arrayStr = arrayStr + recurringActions.at(i).parseToJson() + ",";
        }
        arrayStr[arrayStr.length()-1] = ']';
        arrayStr = arrayStr.substr(0, arrayStr.length());
        cout << "4\n";

        this->createResponse(STATUS_OK, /*"getListResponse",*/ "Sucesso", arrayStr);
        return true;
    } else if (this->requestAction == "deleteRecurringAction"){
        int r = recurringActionDAO::deleteRecurringAction(this->getRecurringAction().getRecActName());
        if (r > 0){
            this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação deletada com sucesso","");
        } else if (r < 0) {
            this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Erro ao deletar a ação","");
        } else if (r == 0) {
            this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Não há ação com este nome","");
        }
    }



//    if(this->requestAction == "getList"){
//        //Se a ação for getList, cria a resposta com uma string do JsonArray
//        string arrayStr;
//        root.printTo(arrayStr);
//        this->createResponse(STATUS_OK, /*"getListResponse",*/ "Sucesso", arrayStr);
//        return true;
//
//    } else { //coloquei esse else separado pois tanto saveRecurringAction quanto deleteRecurringAction preicsa percorrer o arquivo até achar a ocorrencia
//        bool existeAcao = false;
//        //Se a ação for saveRecurringAction, percorre todo o arquivo de ações recorrentes verificando se não existe uma ação com esse nome
//        for(unsigned int i = 0; i < root.size(); i++){
//
//            RecurringActionClass recActTask = RecurringActionClass();
//            recActTask.createFromJson(root[i]);
//
//            //Achou um arquivo com esse nome;
//            if (recActTask.getRecActName() == this->recurringAction.getRecActName()){
//                existeAcao = true;
//
//                if (this->requestAction == "saveRecurringAction"){
//                    if (this->requestOverwrite){
//                        //sobrescreve a ação na posição corrente 'i'
//                        util::saveToFile(file, (string)this->recurringAction.parseToJson(), i, RECURRING_ACTION_FILE);
//                        this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação Recorrente inserida com sucesso","");
//                    } else {
//                        cout<<"requestOverwrite é falso. não fez nada\n";
//                        this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Já existe Ação Recorrente com este nome","requestOverwrite");
//                        return false;
//                    }
//                } else if (this->requestAction == "deleteRecurringAction"){
//                    util::deleteFromFile(file, i, RECURRING_ACTION_FILE);
//                    this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação Recorrente deletada com sucesso","");
//                }
//                break;
//            }
//        }
//
//        //se a acao nao foi encontrada no arquivo, adiciona ela no final do arquivo.
//        if(!existeAcao){
//            if (this->requestAction == "saveRecurringAction"){
//                cout<<"Nao achou a ação. Adicionando ao final\n";
//                util::saveToFile(file, this->recurringAction.parseToJson(), -1, RECURRING_ACTION_FILE);
//                this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação Recorrente inserida com sucesso","");
//            } else if (this->requestAction == "deleteRecurringAction"){
//                this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Não há Ação Recorrente com este nome","");
//            }
//        }
//    }

    if(file.is_open()){
        file.close();
    }
    return true;

}

RecurringActionClass RecurringActionCommand::getRecurringAction(){
    return this->recurringAction;
}
void RecurringActionCommand::setRecurringAction(RecurringActionClass _recurringAction){
    this->recurringAction = _recurringAction;
}
