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
        if (this->requestOverwrite){
            //Faz update (delete/insert) da ação recorrente
            int resDel = recurringActionDAO::deleteRecurringAction(this->getRecurringAction().getRecActName());
            int resIns = recurringActionDAO::insertRecurringAction(this->getRecurringAction());

            if ((resDel > 0 ) && (resIns == 1)){
                this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação alterada com sucesso","");
                return true;
            } else {
                this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Erro ao alterar a Ação","");
                return false;
            }
        } else {
            int res = recurringActionDAO::insertRecurringAction(this->getRecurringAction());
            if (res == 1){
                //Inseriu com sucesso
                this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação Recorrente inserida com sucesso","");
                return true;
            } else if (res == -2){
                //Ação Recorrente já existe. Solicita RequestOverwrite
                this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Já existe Ação Recorrente com este nome","requestOverwrite");
                return true;
            } else {
                this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Erro ao inserir a Ação Recorrente","");
                return false;
            }
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
