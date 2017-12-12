#include "ActionButtonDAO.h"

namespace actionButtonDAO {

int deleteActionButton(string actionName){
    int rowsAffected;
    try {
        pqxx::connection co("dbname = automacaozdb user = azdbuser password = teste123 hostaddr = 127.0.0.1 port = 5432");
        if (co.is_open()) {
         cout << "Opened database successfully: " << co.dbname() << endl;
        } else {
         cout << "Can't open database" << endl;
         return -1;
        }

        /* Create a transactional object. */
        work w(co);
        /* Create  SQL DELETE statement */
        string sql = "DELETE FROM azschema.aztb030_actionButton WHERE no_action = '" + actionName + "';";
        /* Execute SQL query */
        result res = w.exec(sql);
        w.commit();
        rowsAffected = res.affected_rows();

        co.disconnect();
    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
        return -1;
    }
    return rowsAffected;
}

ActionButtonClass getActionButton(string actionName){
    ActionButtonClass actionButton;
    try {
        pqxx::connection co("dbname = automacaozdb user = azdbuser password = teste123 hostaddr = 127.0.0.1 port = 5432");

        if (co.is_open()) {
         cout << "Opened database successfully: " << co.dbname() << endl;
        } else {
         cout << "Can't open database" << endl;
         return actionButton;
        }

        string sql = "SELECT * FROM azschema.aztb030_actionbutton WHERE no_action = '" + actionName + "' ORDER BY seq_action ASC;";

        vector<TaskButtonClass> tbVector;

        nontransaction N(co);
        /* Execute SQL query */
        result R( N.exec( sql ));

        /* List down all the records */
        for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
            TaskButtonClass tbClass;
            tbClass.createFromJson(c[3].as<string>());
            tbVector.insert(tbVector.end(), tbClass);
        }

        ActionClass newAction;
        newAction.setActions(tbVector);

        actionButton.setActionName(R.begin()[1].as<string>());
        actionButton.setAction(newAction);

        co.disconnect ();

    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
        return actionButton;
    }
    return actionButton;


}

vector<ActionButtonClass> getAllActionButtons(){
    vector<ActionButtonClass> actionButtonsList;
    try {
        pqxx::connection co("dbname = automacaozdb user = azdbuser password = teste123 hostaddr = 127.0.0.1 port = 5432");

        if (co.is_open()) {
         cout << "Opened database successfully: " << co.dbname() << endl;
        } else {
         cout << "Can't open database" << endl;
         return vector<ActionButtonClass>();
        }

        char sql[] = "SELECT * FROM azschema.aztb030_actionbutton ORDER BY no_action ASC, seq_action ASC;";

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

                newActionButton.setActionName(c[1].as<string>().c_str());
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
        cout<<"houve erro"<<endl;
        cerr << e.what() << std::endl;
        return vector<ActionButtonClass>();
    }
    return actionButtonsList;

}

int insertActionButton(ActionButtonClass actionButton){
    try {
        pqxx::connection co("dbname = automacaozdb user = azdbuser password = teste123 hostaddr = 127.0.0.1 port = 5432");
        if (co.is_open()) {
         cout << "Opened database successfully: " << co.dbname() << endl;
        } else {
         cout << "Can't open database" << endl;
         return -1;
        }


        for (unsigned short int i = 0; i < actionButton.getAction().getActions().size(); i++){
            pqxx::work txn(co);
            txn.exec(
            "INSERT INTO azschema.aztb030_actionButton (no_action, seq_action, json_action) values (" + txn.quote(actionButton.getActionName()) +
                     ", " + txn.quote(i) + ", " + txn.quote(actionButton.getAction().getActions().at(i).parseToJson()) + ")"
            );
            txn.commit();
        }
        co.disconnect();
    } catch (const std::exception &e) {
      cerr << e.what() << std::endl;
      return -2;
    }
    return 1;
}


}
