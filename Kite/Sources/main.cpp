#include <math.h>
#include "Unit.h"
#include "ActionManager.h"
#include "EETwa.h"

void onChangeAction(Unit unit, Action newAction, Action oldAction) {
  if(!unit_IsSelf(unit)) {
    return ;
  }
  eeTa_FilePrintf("Unit changed action from %p to %p\n", oldAction.type, newAction.type);
}

void bt_OnFrame() {
  act_OnFrame();
}

void bt_OnInit() {
  act_Init();
  act_SetAction(onChangeAction);
}

void bt_OnUnitDestroy(Unit unit) {
  act_Kill(unit);
}

void bt_OnGamePrepare() {
  act_OnGamePrepare();
}

void bt_OnUnitCreate(Unit unit) {
  act_Register(unit);
}