#include "ActionButtonCommand.h"

ActionButtonCommand::ActionButtonCommand(string json) : Command(json){}

ActionButtonCommand::~ActionButtonCommand(){}

bool ActionButtonCommand::createRequestFromJson(){
    Command::createRequestFromJson();

    JsonObject& actBt = this->jsonRoot["actionButton"];
    string actBtStr;
    actBt.printTo(actBtStr);
    this->actionButton = ActionButtonClass();
    this->actionButton.createFromJson(actBtStr);

    return true;
}

bool ActionButtonCommand::execute(){
    if(this->requestAction == "executeActionButton"){
        bool error = false;

        //Percorre o array de ações e criando TaskCommands e executando elas
        for (unsigned short int i = 0; i < this->actionButton.getAction().getActions().size(); i++){
            DynamicJsonBuffer jsonBuffer;
            JsonObject& jsonObj = jsonBuffer.createObject();

            DynamicJsonBuffer jsonBuffer2;
            JsonObject& jsonObj2 = jsonBuffer2.parseObject(this->actionButton.getAction().getActions().at(i).getTask().parseToJson());
            jsonObj["requestAction"] = "taskCommand";
            jsonObj["requestOverwrite"] = false;
            jsonObj["task"] = jsonObj2;

            string taskCmdStr;
            jsonObj.printTo(taskCmdStr);
            TaskCommand taskCmd = TaskCommand(taskCmdStr);
            if (!taskCmd.createRequestFromJson() || (!taskCmd.execute())){
                //Se der erro em um comando, não deixa de enviar os outros. Ou deveria deixar?
                error = true;
            }
            usleep(200000); // espera 0.2 segundos entre um comando e outro.
        }

        if (error){
            this->createResponse(STATUS_ERROR, /*"responseCommand",*/ "Erro ao executar a ação", "");
            return false;
        }

        this->createResponse(STATUS_ERROR, /*"responseCommand",*/ "Ação executada com sucesso", "");
        return true;

    }

    fstream file (ACTION_BUTTON_FILE, fstream::out | fstream::in);

    if (!file.is_open()) {
        //Testa se o arquivo existe. Se nao existir cria (apenas escrita ) e depois abre para escrita e leitura
        file.open(ACTION_BUTTON_FILE, fstream::out);
        file.close();
        file.open(ACTION_BUTTON_FILE, fstream::in | fstream::out);
    }
    if (!file.is_open()){
        //Resposta em caso de erro ao abrir o arquivo
        this->createResponse(STATUS_ERROR, /*"saveResponse",*/ "Erro ao criar o arquivo", "Erro ao acessar o HD");
        return false;
    }

    string line;
    DynamicJsonBuffer jsonBuffer;
    JsonArray& root = jsonBuffer.createArray();

    //percorre todo o arquivo salvando as ações recorrentes em um JsonArray
    while(!file.eof()){
        getline(file, line);
        //Coloca esse break aqui para evitar que ele leia o caractere de EOF ainda considerando como uma parte do array
        if(file.eof()) break;

        const char* jsonDuplicate = jsonBuffer.strdup(line);
        root.add(jsonBuffer.parseObject(jsonDuplicate));
    }

    if(this->requestAction.compare("getAction") == 0){

        for (unsigned short i = 0; i < root.size(); i++) {
            ActionButtonClass actBt = ActionButtonClass();
            actBt.createFromJson(root[i]);

            if(actBt.getActionName().compare(this->actionButton.getActionName()) == 0){
                string returnStr = actBt.parseToJson();
                cout<<"Achou no indice: "<<i<<". Resposta: "<<returnStr<<"\n";
                this->createResponse(STATUS_OK, /*"getListResponse",*/ "Sucesso", returnStr);
                return true;
            }
        }
        cout<<"Não achou nada com nome "<<this->actionButton.getActionName()<<"\n";
        this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Não existe ação com o nome '"+this->actionButton.getActionName()+"'","");
        return false;
    } else if(this->requestAction == "getList"){
        //Se a ação for getList, cria a resposta com uma string do JsonArray
        string arrayStr;
        root.printTo(arrayStr);
        this->createResponse(STATUS_OK, /*"getListResponse",*/ "Sucesso", arrayStr);
        return true;

    } else { //coloquei esse else separado pois tanto saveList quanto deleteList preicsa percorrer o arquivo até achar a ocorrencia
        bool existeAcao = false;
        //Se a ação for saveList, percorre todo o arquivo de ações recorrentes verificando se não existe uma ação com esse nome
        for(unsigned int i = 0; i < root.size(); i++){

            ActionButtonClass actBtClass = ActionButtonClass();
            actBtClass.createFromJson(root[i]);

            //Achou um arquivo com esse nome;
            if (actBtClass.getActionName() == this->actionButton.getActionName()){
                existeAcao = true;

                if (this->requestAction == "saveActionButton"){
                    if (this->requestOverwrite){
                        //sobrescreve a ação na posição corrente 'i'
                        util::saveToFile(file, (string)this->actionButton.parseToJson(), i, ACTION_BUTTON_FILE);
                        this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação inserida com sucesso","");
                    } else {
                        cout<<"requestOverwrite é falso. não fez nada\n";
                        this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Já existe Ação com este nome","requestOverwrite");
                        return false;
                    }
                } else if (this->requestAction == "deleteActionButton"){
                    util::deleteFromFile(file, i, ACTION_BUTTON_FILE);
                    this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação deletada com sucesso","");
                }
                break;
            }
        }

        //se a acao nao foi encontrada no arquivo, adiciona ela no final do arquivo.
        if(!existeAcao) {
            if (this->requestAction == "saveActionButton"){
                cout<<"Nao achou a ação. Adicionando ao final\n";
                util::saveToFile(file, this->actionButton.parseToJson(), -1, ACTION_BUTTON_FILE);
                this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação inserida com sucesso","");
            } else if (this->requestAction == "deleteActionButton"){
                this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Não há Ação com este nome","");
            }
        }
    }

    if(file.is_open()){
        file.close();
    }
    return true;
}


ActionButtonClass ActionButtonCommand::getActionButton(){
    return this->actionButton;
}
void ActionButtonCommand::setActionButton(ActionButtonClass _actionButton){
    this->actionButton = _actionButton;
}
