#include <math.h>
#include "Unit.h"
#include "ActionManager.h"
#include "EETwa.h"
#include "Player.h"

// void processEnemyAction(Unit unit, Action action);

// void onChangeAction(Unit unit, Action newAction, Action oldAction) {
//   processEnemyAction(unit, newAction);
// }

uint8_t isSelfUnitChecker(Unit unit);
void processGameActions();

Point getOpositePosition(Point center, Point rotator) {
  return (Point) {
    .x = 2 * center.x - rotator.x,
    .y = 2 * center.y - rotator.y,
  };
}

// void processSelfUnitAndTarget(Unit selfUnit, Unit enemyUnit) {
//   const float currentEnemyRange = unit_Range(enemyUnit);
//   const float currentSelfRange = unit_Range(selfUnit);
//   if(currentEnemyRange > currentSelfRange && unit_Distance(selfUnit, enemyUnit) < currentSelfRange * 0.85f) {
//     return ;
//   }
//   Point movingPoint = getOpositePosition(unit_Point_Position(selfUnit), unit_Point_Position(enemyUnit));
//   unit_Action(selfUnit, movingPoint, UNIT_MOVE);
// }

// void processEnemyAction(Unit unit, Action action) {
//   if(unit_IsSelf(unit) || ply_AreAllies(ply_GetPlayer(unit), ply_Self())) {
//     return ;
//   }
//   if(action.type != ACTION_ATTACK_TARGET) {
//     return ;
//   }
//   Unit target = action.target;
//   if(!unit_IsSelf(target)) {
//     return ;
//   }
//   processSelfUnitAndTarget(target, unit);
// }

uint8_t isUnit(Unit unit) {
  return !eeTypes_IsBuilding(unit_Type(unit));
}

uint8_t isEnemyUnit(Unit unit) {
  return isUnit(unit) && !unit_IsSelf(unit);
}

uint8_t isSelfUnitChecker(Unit unit) {
  return isUnit(unit) && unit_IsSelf(unit);
}

void findAndAttackEnemy(Unit selfUnit, vector<Unit> &enemyUnits) {
  if(!enemyUnits.size()) {
    return ;
  }
  Action currentAction;
  currentAction.type = ACTION_ATTACK_AREA;
  currentAction.targetPoint = unit_Point_Position(enemyUnits[0]);
  act_PushFrontAction(selfUnit, currentAction);
}

void processAttackUnits() {
  vector<Unit> selfUnits = unit_Filter(isSelfUnitChecker);
  vector<Unit> enemyUnits = unit_Filter(isEnemyUnit);
  for(size_t i = 0; i < selfUnits.size(); i++) {
    if(act_IsIdle(selfUnits[i])) {
      findAndAttackEnemy(selfUnits[i], enemyUnits);
    }
  }
}

void processCurrentUnit(Unit selfUnit, vector<Unit> &enemyUnits) {
  for(size_t i = 0; i < enemyUnits.size(); i++) {
    const float currentEnemyRange = unit_Range(enemyUnits[i]);
    const float currentSelfRange = unit_Range(selfUnit);
    if(currentEnemyRange > currentSelfRange && unit_Distance(selfUnit, enemyUnits[i]) < currentSelfRange * 0.85f) {
      continue;
    }
    Point movingPoint = getOpositePosition(unit_Point_Position(selfUnit), unit_Point_Position(enemyUnits[i]));
    Action currentAction;
    currentAction.type = ACTION_MOVE;
    currentAction.targetPoint = movingPoint;
    act_PushFrontAction(selfUnit, currentAction);
  }
}

void processAttackActions(PVOID _) {
  vector<Unit> enemyUnits = unit_Filter(isEnemyUnit);
  vector<Unit> selfUnits = unit_Filter(isSelfUnitChecker);
  for(size_t i = 0; i < selfUnits.size(); i++) {
    processCurrentUnit(selfUnits[i], enemyUnits);
  }
  processAttackUnits();
}

void processGameActions() {
  TimeAtom atom;
  atom.method = (PVOID)processAttackActions;
  atom.arguments = NULL;
  atom.time = 225;
  eeTa_AddFrameMethod(atom);
}

void bt_OnFrame() {
  act_OnFrame();
}

void bt_OnInit() {
  processGameActions();
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