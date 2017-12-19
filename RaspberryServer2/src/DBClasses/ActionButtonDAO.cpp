#include "ActionButtonDAO.h"

namespace actionButtonDAO {

int deleteActionButton(string actionName){
    int rowsAffected, rowsAffected2;
    try {
        pqxx::connection co("dbname = automacaozdb user = azdbuser password = teste123 hostaddr = 127.0.0.1 port = 5432");
        if (! co.is_open()) {
         cout << util::currentDateTime() << "Can't open database" << endl;
         return -1;
        }

        /* Create a transactional object. */
        work w(co);
        /* Create  SQL DELETE statement */
        string sql  = "DELETE FROM azschema.aztb031_jsontask where id_actionbutton = (select id_actionButton from azschema.aztb032_actionbutton where no_action = '" + actionName + "');";
        result res = w.exec(sql);
        rowsAffected = res.affected_rows();

        string sql2 = "DELETE FROM azschema.aztb032_actionButton WHERE no_action = '" + actionName + "';";
        res = w.exec(sql2);
        w.commit();
        rowsAffected2 = res.affected_rows();

        co.disconnect();
    } catch (const std::exception &e) {
        cerr << util::currentDateTime() << e.what() << std::endl;
        return -1;
    }

    if (rowsAffected == 0 || rowsAffected2 == 0)
        return 0;

    return rowsAffected + rowsAffected2;
}

ActionButtonClass getActionButton(string actionName){
    ActionButtonClass actionButton;
    try {
        pqxx::connection co("dbname = automacaozdb user = azdbuser password = teste123 hostaddr = 127.0.0.1 port = 5432");

        if (!co.is_open()) {
         cout << util::currentDateTime() << "Can't open database" << endl;
         return actionButton;
        }

//        string sql = "SELECT id_actionButton, no_action, seq_action, json_action FROM azschema.aztb030_actionbutton WHERE no_action = '" + actionName + "' ORDER BY seq_action ASC;";

        string sql = "SELECT id_jsonTask, nu_seq, json_task FROM azschema.aztb031_jsonTask WHERE id_actionButton = (SELECT id_actionbutton from azschema.aztb032_actionbutton where no_action = '" + actionName + "') ORDER BY nu_seq ASC;";

        vector<TaskButtonClass> tbVector;

        nontransaction N(co);
        /* Execute SQL query */
        result R( N.exec( sql ));

        /* List down all the records */
        for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
            TaskButtonClass tbClass;
            tbClass.createFromJson(c[2].as<string>());
            tbVector.insert(tbVector.end(), tbClass);
        }

        ActionClass newAction;
        newAction.setActions(tbVector);

        actionButton.setActionName(actionName);
        actionButton.setAction(newAction);

        co.disconnect ();

    } catch (const std::exception &e) {
        cerr << util::currentDateTime() << e.what() << std::endl;
        return actionButton;
    }
    return actionButton;


}

vector<ActionButtonClass> getAllActionButtons(){
    vector<ActionButtonClass> actionButtonsList;
    try {
        pqxx::connection co("dbname = automacaozdb user = azdbuser password = teste123 hostaddr = 127.0.0.1 port = 5432");

        if (!co.is_open()) {
         cout << util::currentDateTime() << "Can't open database" << endl;
         return vector<ActionButtonClass>();
        }

//        char sql[] = "SELECT id_actionButton, no_action, seq_action, json_action FROM azschema.aztb030_actionbutton ORDER BY no_action ASC, seq_action ASC;";

        char sql[] = "SELECT a.id_jsonTask, b.no_action, a.nu_seq, a.json_task FROM azschema.aztb031_jsonTask as a, azschema.aztb032_actionbutton as b WHERE a.id_actionButton = b.id_actionButton ORDER BY b.no_action ASC, a.nu_seq ASC;";

        nontransaction N(co);
        result R( N.exec( sql ));


        if(R.size() == 0){
            return vector<ActionButtonClass>();
        }

        //Pega cada entrada no banco de dados e transforma em um vetor de ActionButton
        ActionButtonClass newActionButton;
        vector<TaskButtonClass> taskButtonVector;

        //Pega o primeiro elemento do retorno do BD e coloca no newActionButton
        result::const_iterator c = R[0];
        newActionButton.setActionName(c[1].as<string>());

        for (unsigned int i = 1; i <= R.size(); i++){
            if(strcmp(newActionButton.getActionName().c_str(), c[1].as<string>().c_str()) != 0){
                //Se o ActionName do retorno do banco eh diferente do anterior significa que trocou de acao
                ActionClass newAction;
                newAction.setActions(taskButtonVector);
                newActionButton.setAction(newAction);

                actionButtonsList.insert(actionButtonsList.end(), newActionButton);

                newActionButton.setActionName(c[1].as<string>());
                taskButtonVector.erase(taskButtonVector.begin(), taskButtonVector.end());

            }
            TaskButtonClass tb;
            tb.createFromJson(c[3].as<string>());
            taskButtonVector.insert(taskButtonVector.end(), tb);
            c = R[i];

        }
        //Pega o ultimo elemento do retorno do bd, que ficou de fora do loop acima
        ActionClass newAction;
        newAction.setActions(taskButtonVector);
        newActionButton.setAction(newAction);
        actionButtonsList.insert(actionButtonsList.end(), newActionButton);

//        for (int i = 0; i < actionButtonsList.size(); i++){
//            cout<<"ActionName: " << actionButtonsList.at(i).getActionName()<<endl;
//            cout<<"Actions: " << actionButtonsList.at(i).getAction().parseToJson()<<endl;
//            cout<<"\n\n";
//        }

        co.disconnect ();

    } catch (const std::exception &e) {
        cerr << util::currentDateTime() << e.what() << std::endl;
        return vector<ActionButtonClass>();
    }
    return actionButtonsList;

}

int insertActionButton(ActionButtonClass actionButton){
    try {
        pqxx::connection co("dbname = automacaozdb user = azdbuser password = teste123 hostaddr = 127.0.0.1 port = 5432");
        if (!co.is_open()) {
         cout << util::currentDateTime() << "Can't open database" << endl;
         return -1;
        }

        pqxx::work txn(co);
        pqxx::result r = txn.exec(
            "INSERT INTO azschema.aztb032_actionButton (no_action) values (" + txn.quote(actionButton.getActionName()) +") returning id_actionbutton"
            );
        txn.commit();
        int res = r[0][0].as<int>();

        for (unsigned short int i = 0; i < actionButton.getAction().getActions().size(); i++){
            pqxx::work txn2(co);
            txn2.exec(
            "INSERT INTO azschema.aztb031_jsonTask (id_actionButton, nu_seq, json_task) values (" + txn2.quote(res) +
                     ", " + txn2.quote(i) + ", " + txn2.quote(actionButton.getAction().getActions().at(i).parseToJson()) + ")"
            );
            txn2.commit();
        }
        co.disconnect();
    } catch (const std::exception &e) {
      cout << util::currentDateTime() << "INFO: " << e.what() << std::endl;
      return -2;
    }
    return 1;
}

int updateActionButton(ActionButtonClass actionButton){
    //Esse metodo delete as entradas deste ActionButton na tabela 31 JsonTask e insere de novo. Nao altera nada na tabela 32 ActionButton
    try {
        pqxx::connection co("dbname = automacaozdb user = azdbuser password = teste123 hostaddr = 127.0.0.1 port = 5432");
        if (!co.is_open()) {
         cout << util::currentDateTime() << "Can't open database" << endl;
         return -1;
        }

        //Delete as entradas da tabela 31 JsonTask
        work w(co);
        string sql  = "DELETE FROM azschema.aztb031_jsontask where id_actionbutton = (select id_actionButton from azschema.aztb032_actionbutton where no_action = '" + actionButton.getActionName()+ "');";
        result res = w.exec(sql);
        w.commit();

        //Select no id_actionButton na tabela 32 ActionButton
        string sql2 = "SELECT id_actionbutton FROM azschema.aztb032_actionbutton WHERE no_action = '" +actionButton.getActionName()+"';";

        nontransaction N(co);
        result R( N.exec( sql2 ));
        N.commit();
        result::const_iterator c = R[0];

        int idActionButton = c[0].as<int>();

        //Insere novamente na tabela 31 JsonTask utilizando o id_actionButton
        for (unsigned short int i = 0; i < actionButton.getAction().getActions().size(); i++){
            pqxx::work txn(co);
            txn.exec(
            "INSERT INTO azschema.aztb031_jsonTask (id_actionButton, nu_seq, json_task) values (" + txn.quote(idActionButton) +
                     ", " + txn.quote(i) + ", " + txn.quote(actionButton.getAction().getActions().at(i).parseToJson()) + ")"
            );
            txn.commit();
        }
        co.disconnect();

    } catch (const std::exception &e) {
      cout << util::currentDateTime() << "INFO: " << e.what() << std::endl;
      return -2;
    }
    return 1;
}


}
