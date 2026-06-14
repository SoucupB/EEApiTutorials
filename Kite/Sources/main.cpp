#include <math.h>
#include "Unit.h"
#include "ActionManager.h"
#include "EETwa.h"
#include "Player.h"

uint8_t isSelfUnitChecker(Unit unit);
void processGameActions();

Point getOpositePosition(Point center, Point rotator) {
  return (Point) {
    .x = 2 * center.x - rotator.x,
    .y = 2 * center.y - rotator.y,
  };
}

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
  act_PushFrontAction(selfUnit, currentAction, 1);
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
  Unit enemyToRunFrom = unit_Null();
  float maxDistance = 100000.0f;
  for(size_t i = 0; i < enemyUnits.size(); i++) {
    const float currentEnemyRange = unit_Range(enemyUnits[i]);
    const float currentSelfRange = unit_Range(selfUnit);
    const float currentDistance = unit_Distance(selfUnit, enemyUnits[i]);
    if(currentEnemyRange > currentSelfRange || currentDistance >= currentSelfRange * 0.4f) {
      continue;
    }
    if(maxDistance > currentDistance) {
      maxDistance = currentDistance;
      enemyToRunFrom = enemyUnits[i];
    }
  }
  if(!unit_Reference(enemyToRunFrom)) {
    return ;
  }
  Point movingPoint = getOpositePosition(unit_Point_Position(selfUnit), unit_Point_Position(enemyToRunFrom));
  Action currentAction;
  currentAction.type = ACTION_MOVE;
  currentAction.targetPoint = movingPoint;
  act_PushFrontAction(selfUnit, currentAction, 0);
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