#include "ActionManager.h"
#include <map>
#include <windows.h>
#include "SimpleUnit.h"

static map<PVOID, UnitActionQueue> unitActions;

void act_Register(Unit unit) {
  unitActions[unit_Reference(unit)] = (UnitActionQueue) {
    .unit = unit,
    .actionQueue = new deque<ActionWithFlag>()
  };
}

void act_Init() {
}

void act_PushFrontAction(Unit unit, Action action, uint8_t waitForIdle) {
  const PVOID ref = unit_Reference(unit);
  if(unitActions.find(ref) == unitActions.end()) {
    return ;
  }
  if(unitActions[ref].actionQueue->size() >= 7) {
    unitActions[ref].actionQueue->pop_back();
  }
  unitActions[ref].actionQueue->push_front((ActionWithFlag) {
    .action = action,
    .waitForIdle = waitForIdle
  });
}

void act_OnGamePrepare() {
  unitActions.clear();
}

void act_Kill(Unit unit) {
  const PVOID ref = unit_Reference(unit);
  if(unitActions.find(ref) == unitActions.end()) {
    return ;
  }
  delete unitActions[ref].actionQueue;
  unitActions.erase(ref);
}

uint8_t isSelfUnit(Unit unit) {
  return !eeTypes_IsBuilding(unit_Type(unit)) && unit_IsSelf(unit);
}

uint8_t act_ArePointsDifferent(Point a, Point b) {
  return a.x != b.x || a.y != b.y;
}

uint8_t act_Tile_ArePointsDifferent(TilePoint a, TilePoint b) {
  return a.x != b.x || a.y != b.y;
}

uint8_t act_IsTargetDiff(Unit a, Unit b) {
  return unit_Reference(a) != unit_Reference(b);
}

uint8_t act_AreActionDifferent(Action a, Action b) {
  if(a.type != b.type) {
    return 1;
  }
  switch (a.type)
  {
    case ACTION_MOVE: {
      return act_ArePointsDifferent(a.targetPoint, b.targetPoint);
    }
    case ACTION_ATTACK_AREA: {
      return act_ArePointsDifferent(a.targetPoint, b.targetPoint);
    }
    case ACTION_CAST_AREA: {
      return act_Tile_ArePointsDifferent(a.targetTile, b.targetTile);
    }
    case ACTION_REPAIR: {
      return act_IsTargetDiff(a.target, b.target);
    }
    case ACTION_CAST_TARGET: {
      return act_IsTargetDiff(a.target, b.target);
    }
    case ACTION_ATTACK_TARGET: {
      return act_IsTargetDiff(a.target, b.target);
    }
    case ACTION_GATHER: {
      return su_Reference(a.targetSimpleUnit) != su_Reference(b.targetSimpleUnit);
    }
    default:
      break;
  }

  return 0;
}

uint8_t act_IsIdle(Unit unit) {
  const PVOID ref = unit_Reference(unit);
  const uint8_t isIdle = unit_IsIdle(unit);
  if(unitActions.find(ref) == unitActions.end() || isIdle) {
    return 1;
  }
  UnitActionQueue actionQueue = unitActions[unit_Reference(unit)];
  if(actionQueue.actionQueue->size()) {
    return 0;
  }
  return isIdle;
}

uint8_t canProceed(Action currentAction, ActionWithFlag queueAction) {
  if(currentAction.type == ACTION_IDLE && queueAction.waitForIdle) {
    return 1;
  }
  if(queueAction.waitForIdle) {
    return 0;
  }
  return act_AreActionDifferent(currentAction, queueAction.action);
}

void processAction(UnitActionQueue unitAction, Action currentAction) {
  if(!unitAction.actionQueue->size()) {
    return ;
  }
  ActionWithFlag queueAction = unitAction.actionQueue->front();
  if(!canProceed(currentAction, queueAction)) {
    return ;
  }
  unitAction.actionQueue->pop_front();
  switch (queueAction.action.type)
  {
    case ACTION_MOVE: {
      unit_Action(unitAction.unit, queueAction.action.targetPoint, UNIT_MOVE);
      break;
    }
    case ACTION_ATTACK_AREA: {
      unit_Action(unitAction.unit, queueAction.action.targetPoint, UNIT_ATTACK);
      break;
    }
    case ACTION_ATTACK_TARGET: {
      if(!unit_IsValid(queueAction.action.target)) {
        return ;
      }
      unit_AttackTarget(unitAction.unit, queueAction.action.target);
      break;
    }
    
    default:
      break;
  }

}

void act_OnFrame() {
  vector<Unit> selfUnits = unit_Filter(isSelfUnit);
  for(size_t i = 0; i < selfUnits.size(); i++) {
    const PVOID ref = unit_Reference(selfUnits[i]);
    Action currentAction = act_Get(selfUnits[i]);
    if(unitActions.find(ref) == unitActions.end()) {
      continue;
    }
    processAction(unitActions[ref], currentAction);
  }
}