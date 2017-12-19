#ifndef RECURRINGACTIONDAO_H
#define RECURRINGACTIONDAO_H


#include "Util.h"
#include "RecurringActionClass.h"
#include "ActionButtonDAO.h"
#include <time.h>

namespace recurringActionDAO {

int insertRecurringAction(RecurringActionClass);
vector<RecurringActionClass> getAllRecurringActions();
RecurringActionClass getRecurringAction(string);
int deleteRecurringAction(string);


}
#endif // RECURRINGACTIONDAO_H
