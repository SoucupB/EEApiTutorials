#include <math.h>
#include "Unit.h"
#include "ActionManager.h"

void bt_OnFrame() {
  
}

void bt_OnInit() {
  act_Init();
}

void bt_OnUnitDestroy(Unit unit) {
  act_Kill(unit);
}

void bt_OnUnitCreate(Unit unit) {
  act_Register(unit);
}