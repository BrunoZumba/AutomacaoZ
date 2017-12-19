#include "RecurringActionDAO.h"

namespace recurringActionDAO {

int deleteRecurringAction(string recurringActionName){
    int rowsAffected;
    try {
        pqxx::connection co("dbname = automacaozdb user = azdbuser password = teste123 hostaddr = 127.0.0.1 port = 5432");
        if (! co.is_open()) {
            cout << util::currentDateTime() << "Can't open database" << endl;
            return -1;
        }

        /* Create a transactional object. */
        work w(co);
        /* Create  SQL DELETE statement */
        string sql = "DELETE FROM azschema.aztb040_recurringAction WHERE no_recurringAction = '" + recurringActionName + "';";
        result res = w.exec(sql);
        w.commit();
        rowsAffected = res.affected_rows();

        co.disconnect();
    } catch (const std::exception &e) {
        cerr << util::currentDateTime() << e.what() << std::endl;
        return -1;
    }

    if (rowsAffected == 0)
        return 0;

    return rowsAffected;
}

vector<RecurringActionClass> getAllRecurringActions(){
    vector<RecurringActionClass> recurringActionsList;
    try {
        pqxx::connection co("dbname = automacaozdb user = azdbuser password = teste123 hostaddr = 127.0.0.1 port = 5432");

        if (!co.is_open()) {
         cout << util::currentDateTime() << "Can't open database" << endl;
         return vector<RecurringActionClass>();
        }

//        string sql = "SELECT a.no_recurringaction, a.ts_execution, a.bl_executed, a.nu_dayinsecs, a.nu_timeinsecs, b.no_action FROM azschema.aztb040_recurringAction as a, azschema.aztb032_actionbutton as b WHERE a.id_actionButton = b.id_actionButton ORDER BY a.no_recurringaction;";

        const char *sql = "SELECT tb040.no_recurringaction, tb032.no_action, tb041.ts_execution, tb041.nu_dayinsecs, tb041.nu_timeinsecs "
                        "FROM azschema.aztb032_actionbutton as tb032, azschema.aztb040_recurringaction as tb040, azschema.aztb041_recurringactionexecution as tb041 "
                        "WHERE "
                        "tb040.id_recurringaction = tb041.id_recurringAction AND "
                        "tb040.id_actionbutton = tb032.id_actionbutton "
                        "ORDER BY "
                        "tb040.no_recurringaction ASC;";

        nontransaction N(co);
        result R( N.exec( sql ));

        if(R.size() == 0){
            return vector<RecurringActionClass>();
        }

        //Pega cada entrada no banco de dados e transforma em um vetor de RecurringAction
        RecurringActionClass newRecurringAction;
        vector<long long> dates, times;
        //Pega o primeiro elemento do retorno do BD e coloca no newActionButton
        result::const_iterator c = R[0];

        newRecurringAction.setRecActName(c[0].as<string>());
        string recurringActionName = c[0].as<string>();
        string actionButtonName = c[1].as<string>();
        cout<<"1\n";

        for (unsigned int i = 1; i <= R.size(); i++){
            cout<<"NoRecActBD: "<<c[0].as<string>()<<endl;
            cout<<"NoRecActVar: "<<newRecurringAction.getRecActName()<<endl;
            if(strcmp(recurringActionName.c_str(), c[0].as<string>().c_str()) != 0){
                cout<<"dentro loop\n";
                RecurringActionClass newRecurringAction;
                //Se o RecActName do retorno do banco eh diferente do anterior significa que trocou de RecurringAction
                newRecurringAction.setRecActName(recurringActionName);
                newRecurringAction.setRecActDates(dates);
                newRecurringAction.setRecActTimes(times);
                newRecurringAction.setRecActActionButton(actionButtonDAO::getActionButton(actionButtonName));
                recurringActionsList.insert(recurringActionsList.end(), newRecurringAction);

                //newRecurringAction.setRecActName(c[0].as<string>());
                recurringActionName = c[0].as<string>();
                actionButtonName = c[1].as<string>();
                dates.erase(dates.begin(), dates.end());
                times.erase(times.begin(), times.end());
            }
            cout<<"fora loop\n";

            dates.insert(dates.end(), c[3].as<long long>() * 1000);
            times.insert(times.end(), c[4].as<long long>() * 1000);

            c = R[i];

        }
        cout<<"fim loop\n";

        //Pega o ultimo elemento do retorno do bd, que ficou de fora do loop acima
        newRecurringAction.setRecActName(recurringActionName);
        newRecurringAction.setRecActDates(dates);
        newRecurringAction.setRecActTimes(times);
        newRecurringAction.setRecActActionButton(actionButtonDAO::getActionButton(actionButtonName));
        recurringActionsList.insert(recurringActionsList.end(), newRecurringAction);

        co.disconnect ();

    } catch (const std::exception &e) {
        cerr << util::currentDateTime() << e.what() << std::endl;
        return vector<RecurringActionClass>();
    }
    return recurringActionsList;

}

int insertRecurringAction(RecurringActionClass recurringAction){
    try {
        pqxx::connection co("dbname = automacaozdb user = azdbuser password = teste123 hostaddr = 127.0.0.1 port = 5432");
        if (!co.is_open()) {
         cout << "Can't open database" << endl;
         return -1;
        }

        pqxx::work txn(co);
        string sql = "INSERT INTO azschema.aztb040_recurringAction (no_recurringAction, id_actionButton) VALUES (" + txn.quote(recurringAction.getRecActName())
        +", (select id_actionbutton from azschema.aztb032_actionbutton where no_action = " + txn.quote(recurringAction.getRecActActionButton().getActionName())
        +")) returning id_recurringAction;";

        pqxx::result r = txn.exec(sql);
//        txn.commit();
        int res = r[0][0].as<int>();

        for (unsigned int i = 0; i < recurringAction.getRecActDates().size(); i++){
            for (unsigned int j = 0; j < recurringAction.getRecActTimes().size(); j++){
                long long ts = recurringAction.getRecActDates().at(i) + recurringAction.getRecActTimes().at(j);
                ts = ts/1000;

                string sql2 = "INSERT INTO azschema.aztb041_recurringActionExecution (bl_executed, ts_execution, nu_dayinsecs, nu_timeinsecs, id_recurringAction) values (false, to_timestamp("
                +txn.quote(ts)+ "), "+ txn.quote(recurringAction.getRecActDates().at(i)/1000) + ", "
                +txn.quote(recurringAction.getRecActTimes().at(j)/1000) +", "+txn.quote(res)+");";

                pqxx::result r = txn.exec(sql2);

            }
        }
        txn.commit();
        co.disconnect();
    } catch (const std::exception &e) {
        cout << util::currentDateTime() << "INFO: " << e.what() << std::endl;
        return -2;
    }
    return 1;
}
}
