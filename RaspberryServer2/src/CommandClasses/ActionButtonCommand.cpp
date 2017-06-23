#include "ActionButtonCommand.h"

ActionButtonCommand::ActionButtonCommand(){}

ActionButtonCommand::~ActionButtonCommand(){}

bool ActionButtonCommand::execute(){
    fstream file (ACTION_BUTTON_FILE, fstream::out | fstream::in);

    if (!file.is_open()) {
        //Testa se o arquivo existe. Se nao existir cria (apenas escrita ) e depois abre para escrita e leitura
        file.open(ACTION_BUTTON_FILE, fstream::out);
        file.close();
        file.open(ACTION_BUTTON_FILE, fstream::in | fstream::out);
    }
    if (!file.is_open()){
        //Resposta em caso de erro ao abrir o arquivo
        this->createResponse(STATUS_ERROR, "saveResponse", "Erro ao criar o arquivo", "Erro ao acessar o HD");
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


    if(this->actionType == "getList"){
        //Se a ação for getList, cria a resposta com uma string do JsonArray
        string arrayStr;
        root.printTo(arrayStr);
        this->createResponse(STATUS_OK, "getListResponse", "Sucesso", arrayStr);
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

                if (this->actionType == "saveList"){
                    if (this->overwrite){
                        //sobrescreve a ação na posição corrente 'i'
                        util::saveToFile(file, (string)this->actionButton.parseToJson(), i, ACTION_BUTTON_FILE);

                    } else {
                        cout<<"overwrite é falso. não fez nada\n";
                        this->createResponse(STATUS_ERROR, "saveListResponse", "Já existe ação com este nome","Overwrite?");
                        return false;
                    }

                } else if (this->actionType == "deleteList"){
                    util::deleteFromFile(file, i, ACTION_BUTTON_FILE);
                }

                break;
            }
        }

        //se a acao nao foi encontrada no arquivo, adiciona ela no final do arquivo.
        if((!existeAcao) && (this->actionType == "saveList")){
            cout<<"Nao achou a ação. Adicionando ao final\n";
            util::saveToFile(file, this->actionButton.parseToJson(), -1, ACTION_BUTTON_FILE);
        }
    }

    if(file.is_open()){
        file.close();
    }
    return true;


}

bool ActionButtonCommand::createFromJson(string json){
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);

    //TODO: tirar essa gambs de primeiro usar um char*
    const char* tmp = root["actionType"];
    bool tmp2 = root["overwrite"];

    if ((tmp == NULL) /*|| (tmp2 == NULL)*/) {
        return false;
    }
    this->actionType = string(tmp);
    this->overwrite = bool(tmp2);

    JsonObject& actBt = root["actionButton"];
    string actBtStr;
    actBt.printTo(actBtStr);
    this->actionButton = ActionButtonClass();
    this->actionButton.createFromJson(actBtStr);


    return true;
}






ActionButtonClass ActionButtonCommand::getActionButton(){
    return this->actionButton;
}
void ActionButtonCommand::setActionButton(ActionButtonClass _actionButton){
    this->actionButton = _actionButton;
}
string ActionButtonCommand::getActionType(){
    return this->actionType;
}
void ActionButtonCommand::setActionType(string _actionType){
    this->actionType = _actionType;
}
bool ActionButtonCommand::getOverwrite(){
    return this->overwrite;
}
void ActionButtonCommand::setOverwrite(bool _overwrite){
    this->overwrite = _overwrite;
}
