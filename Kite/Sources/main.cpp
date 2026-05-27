#include <math.h>
#include "Unit.h"
#include "ActionManager.h"
#include "EETwa.h"
#include "Player.h"

void processEnemyAction(Unit unit, Action action);

void onChangeAction(Unit unit, Action newAction, Action oldAction) {
  processEnemyAction(unit, newAction);
}

Point getOpositePosition(Point center, Point rotator) {
  return (Point) {
    .x = 2 * center.x - rotator.x,
    .y = 2 * center.y - rotator.y,
  };
}

void processSelfUnitAndTarget(Unit selfUnit, Unit enemyUnit) {
  const float currentEnemyRange = unit_Range(enemyUnit);
  const float currentSelfRange = unit_Range(selfUnit);
  if(currentEnemyRange > currentSelfRange && unit_Distance(selfUnit, enemyUnit) < currentSelfRange * 0.85f) {
    return ;
  }
  Point movingPoint = getOpositePosition(unit_Point_Position(selfUnit), unit_Point_Position(enemyUnit));
  unit_Action(selfUnit, movingPoint, UNIT_MOVE);
}

void processEnemyAction(Unit unit, Action action) {
  if(unit_IsSelf(unit) || ply_AreAllies(ply_GetPlayer(unit), ply_Self())) {
    return ;
  }
  if(action.type != ACTION_ATTACK_TARGET) {
    return ;
  }
  Unit target = action.target;
  if(!unit_IsSelf(target)) {
    return ;
  }
  processSelfUnitAndTarget(target, unit);
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