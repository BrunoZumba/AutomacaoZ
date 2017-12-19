#ifndef ACTIONBUTTONDAO_H
#define ACTIONBUTTONDAO_H

#include "Util.h"
#include "ActionButtonClass.h"

namespace actionButtonDAO {

int insertActionButton(ActionButtonClass);
vector<ActionButtonClass> getAllActionButtons();
ActionButtonClass getActionButton(string);
int deleteActionButton(string);
int updateActionButton(ActionButtonClass);


}

#endif // ACTIONBUTTONDAO_H
