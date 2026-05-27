#include "ActionManager.h"
#include <unordered_map>
#include <windows.h>

static unordered_map<PVOID, UnitActionStruct> unitActions;
static PVOID onActionChange;

void act_Register(Unit unit) {
  unitActions[unit_Reference(unit)] = (UnitActionStruct) {
    .unit = unit,
    .currentAction = (Action) {
      .type = ACTION_IDLE
    }
  };
}

void act_Init() {
  unitActions.clear();
}

void act_Kill(Unit unit) {
  unitActions.erase(unit_Reference(unit));
}

uint8_t act_AreActionDifferent(Action a, Action b) {
  if(a.type != b.type) {
    return 1;
  }
  switch (a.type)
  {
    case ACTION_ATTACK_AREA: {
      
      break;
    }
    
    default:
      break;
  }
}

void act_OnFrame() {
  for(auto &it : unitActions) {
    Action currentAction = act_Get((Unit) {
      ._payload = it.first
    });
  }
}