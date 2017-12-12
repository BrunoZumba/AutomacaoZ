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


/* TUDO QUE ESTA COMENTADO COM /// FOI RETIRADO POIS UTILIZAVA ARQUIVO E AGORA ESTA UTILIZANDO O BD */
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
            usleep(1000000); // espera 1 segundos entre um comando e outro.
        }

        if (error){
            this->createResponse(STATUS_ERROR, /*"responseCommand",*/ "Erro ao executar a ação", "");
            return false;
        }

        this->createResponse(STATUS_OK, /*"responseCommand",*/ "Ação executada com sucesso", "");
        return true;

    }

///    fstream file;
///
///    if (!util::openFile(ACTION_BUTTON_FILE, file)){
///        this->createResponse(STATUS_ERROR, /*"saveResponse",*/ "Erro ao criar o arquivo", "Erro ao acessar o HD");
///        return false;
///    }
///
///    string line;
///    DynamicJsonBuffer jsonBuffer;
///    JsonArray& root = jsonBuffer.createArray();
///
///    //percorre todo o arquivo salvando as ações recorrentes em um JsonArray
///    while(!file.eof()){
///        getline(file, line);
///        //Coloca esse break aqui para evitar que ele leia o caractere de EOF ainda considerando como uma parte do array
///        if(file.eof()) break;
///
///        const char* jsonDuplicate = jsonBuffer.strdup(line);
///        root.add(jsonBuffer.parseObject(jsonDuplicate));
///    }

    if(this->requestAction.compare("getAction") == 0){
        ActionButtonClass actionButton = actionButtonDAO::getActionButton(this->actionButton.getActionName());

        cout<<"ActionName que achou no BD: '"<<actionButton.getActionName()<<"'\n";

        if (actionButton.getActionName().length() == 0){
            cout<<"Não achou nada com nome "<<this->actionButton.getActionName()<<"\n";
            this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Não existe ação com o nome '"+this->actionButton.getActionName()+"'","");
            return false;
        } else {
            string returnStr = actionButton.parseToJson();
            this->createResponse(STATUS_OK, /*"getListResponse",*/ "Sucesso", returnStr);
            return true;
        }


        ///DEPRECATED: NAO ESTA SENDO UTILIZADO MAIS VIA ARQUIVO. ESTA VIA BD, CONFORME ACIMA
///        for (unsigned short i = 0; i < root.size(); i++) {
///            ActionButtonClass actBt = ActionButtonClass();
///            actBt.createFromJson(root[i]);
///
///            if(actBt.getActionName().compare(this->actionButton.getActionName()) == 0){
///                string returnStr = actBt.parseToJson();
///                cout<<"Achou no indice: "<<i<<". Resposta: "<<returnStr<<"\n";
///                this->createResponse(STATUS_OK, /*"getListResponse",*/ "Sucesso", returnStr);
///                return true;
///            }
///        }
///        cout<<"Não achou nada com nome "<<this->actionButton.getActionName()<<"\n";
///        this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Não existe ação com o nome '"+this->actionButton.getActionName()+"'","");
///        return false;

    } else if(this->requestAction == "getList"){
        //Busca a lista de ActionButton no banco de dados
        vector<ActionButtonClass> actionButtons;
        actionButtons = actionButtonDAO::getAllActionButtons();

        if (actionButtons.size() <= 0){
            this->createResponse(STATUS_OK, /*"getListResponse",*/ "Sucesso", "[]");
            return true;
        }

        //Transforma o vector<ActionButtonClass> em um JsonArray
        string arrayStr = "[";
        for (unsigned int i = 0; i<actionButtons.size(); i++){
            arrayStr = arrayStr + actionButtons.at(i).parseToJson() + ",";
        }
        arrayStr[arrayStr.length()-1] = ']';
        arrayStr = arrayStr.substr(0, arrayStr.length());

        ///DEPRECATED: nao esta sendo utilizado o root, pois foi alterado para o banco de dados!
///        //Se a ação for getList, cria a resposta com uma string do JsonArray
///        root.printTo(arrayStr);
///        this->createResponse(STATUS_OK, /*"getListResponse",*/ "Sucesso", arrayStr);

        this->createResponse(STATUS_OK, /*"getListResponse",*/ "Sucesso", arrayStr);
        return true;




    } else if (this->requestAction == "saveActionButton"){
        //Salva o novo ActionButton no Banco de Dados
        int res = actionButtonDAO::insertActionButton(this->actionButton);

        if (res == 1) {
            //Inseriu com sucesso
            this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação inserida com sucesso","");
        } else if (res == -2) {
            //Ação já existe. Verifica RequestOverwrite
            if (this->requestOverwrite){
                //Delete depois insere de novo
                actionButtonDAO::deleteActionButton(this->actionButton.getActionName());
                res = actionButtonDAO::insertActionButton(this->actionButton);
                    if (res == 1){
                        this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação inserida com sucesso","");
                    } else {
                        this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Erro ao inserer a Ação","");
                    }
            } else {
                //Solicita ao usuario confirmação de sobrescrita da Ação
                this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Já existe Ação com este nome","requestOverwrite");
            }
        } else {
            this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Erro ao inserer a Ação","");
        }



    } else if (this->requestAction == "deleteActionButton"){
        int r = actionButtonDAO::deleteActionButton(this->actionButton.getActionName());
        if (r > 0){
            this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação deletada com sucesso","");
        } else if (r < 0) {
            this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Erro ao deletar a ação","");
        } else if (r == 0) {
            this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Não há ação com este nome","");
        }



    }
///    else { //coloquei esse else separado pois tanto saveList quanto deleteList preicsa percorrer o arquivo até achar a ocorrencia
///        bool existeAcao = false;
///        //Se a ação for saveList, percorre todo o arquivo de ações recorrentes verificando se não existe uma ação com esse nome
///        for(unsigned int i = 0; i < root.size(); i++){
///
///            ActionButtonClass actBtClass = ActionButtonClass();
///            actBtClass.createFromJson(root[i]);
///
///            //Achou um arquivo com esse nome;
///            if (actBtClass.getActionName() == this->actionButton.getActionName()){
///                existeAcao = true;
///
///                if (this->requestAction == "saveActionButton"){
///                    if (this->requestOverwrite){
///                        //sobrescreve a ação na posição corrente 'i'
///                        util::saveToFile(file, (string)this->actionButton.parseToJson(), i, ACTION_BUTTON_FILE);
///                        this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação inserida com sucesso","");
///                    } else {
///                        cout<<"requestOverwrite é falso. não fez nada\n";
///                        this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Já existe Ação com este nome","requestOverwrite");
///                        return false;
///                    }
///                } else if (this->requestAction == "deleteActionButton"){
///                    util::deleteFromFile(file, i, ACTION_BUTTON_FILE);
///                    this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação deletada com sucesso","");
///                }
///                break;
///            }
///        }
///
///        //se a acao nao foi encontrada no arquivo, adiciona ela no final do arquivo.
///        if(!existeAcao) {
///            if (this->requestAction == "saveActionButton"){
///                cout<<"Nao achou a ação. Adicionando ao final\n";
///                util::saveToFile(file, this->actionButton.parseToJson(), -1, ACTION_BUTTON_FILE);
///                this->createResponse(STATUS_OK, /*"saveListResponse",*/ "Ação inserida com sucesso","");
///            } else if (this->requestAction == "deleteActionButton"){
///                this->createResponse(STATUS_ERROR, /*"saveListResponse",*/ "Não há Ação com este nome","");
///            }
///        }
///    }
///
///    if(file.is_open()){
///        file.close();
///    }
    return true;
}


ActionButtonClass ActionButtonCommand::getActionButton(){
    return this->actionButton;
}
void ActionButtonCommand::setActionButton(ActionButtonClass _actionButton){
    this->actionButton = _actionButton;
}
