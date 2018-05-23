#include "RecurringActionDAO.h"

namespace recurringActionDAO {

int deleteRecurringAction(string recurringActionName){
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
        string sql = "DELETE FROM azschema.aztb041_recurringActionExecution WHERE id_recurringAction = ( "
                     "select id_recurringAction FROM azschema.aztb040_recurringAction WHERE no_recurringAction = '" + recurringActionName + "');";
        result res = w.exec(sql);
        rowsAffected = res.affected_rows();

        string sql2 = "DELETE FROM azschema.aztb040_recurringaction WHERE no_recurringAction = '" + recurringActionName + "';";
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

vector<RecurringActionClass> getAllRecurringActions(){
    vector<RecurringActionClass> recurringActionsList;
    try {
        pqxx::connection co("dbname = automacaozdb user = azdbuser password = teste123 hostaddr = 127.0.0.1 port = 5432");

        if (!co.is_open()) {
            cout << util::currentDateTime() << "Can't open database" << endl;
            return vector<RecurringActionClass>();
        }
        nontransaction N(co);

        const char *sql = "SELECT tb040.no_recurringaction, tb032.no_action, tb040.id_recurringaction "
                        "FROM azschema.aztb032_actionbutton as tb032, azschema.aztb040_recurringaction as tb040 "
                        "WHERE "
                        "tb040.id_actionbutton = tb032.id_actionbutton "
                        "ORDER BY "
                        "tb040.no_recurringaction ASC;";

        result ResultQuery( N.exec( sql ));

        if(ResultQuery.size() == 0){
            return vector<RecurringActionClass>();
        }

        //Pega cada entrada no banco de dados e transforma em um vetor de RecurringAction
        vector<long long> dates, times;

        for (unsigned int i = 0; i < ResultQuery.size(); i++){
            result::const_iterator c = ResultQuery[i];
            string recurringActionName = c[0].as<string>();
            string actionButtonName = c[1].as<string>();
            int idRecAct = c[2].as<int>();

            RecurringActionClass newRecurringAction;
            newRecurringAction.setRecActName(recurringActionName);
            newRecurringAction.setRecActActionButton(actionButtonDAO::getActionButton(actionButtonName));

            char sqlTimes[200];
            strcpy(sqlTimes, "SELECT distinct nu_timeinsecs FROM azschema.aztb041_recurringactionexecution WHERE id_recurringAction = ");
            strcat(sqlTimes, to_string(idRecAct).c_str());
            strcat(sqlTimes, ";");
            result ResultTimes( N.exec( sqlTimes ));

            times.erase(times.begin(), times.end());
            for (unsigned int j = 0; j < ResultTimes.size(); j++){
                times.insert(times.end(), ResultTimes[j][0].as<long long>() * 1000);
            }
            newRecurringAction.setRecActTimes(times);

            char sqlDates[200];
            strcpy(sqlDates, "SELECT distinct nu_dayinsecs FROM azschema.aztb041_recurringactionexecution WHERE id_recurringAction = ");
            strcat(sqlDates, to_string(idRecAct).c_str());
            strcat(sqlDates, ";");
            result ResultDates( N.exec( sqlDates ));

            dates.erase(dates.begin(), dates.end());
            for (unsigned int j = 0; j < ResultDates.size(); j++){
                dates.insert(dates.end(), ResultDates[j][0].as<long long>() * 1000);
            }
            newRecurringAction.setRecActDates(dates);
            recurringActionsList.insert(recurringActionsList.end(), newRecurringAction);

        }


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

int updateRecurringAction(RecurringActionClass recurringAction){
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
