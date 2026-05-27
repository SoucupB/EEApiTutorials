#include "ActionManager.h"
#include <map>
#include <windows.h>
#include "SimpleUnit.h"

static map<PVOID, UnitActionStruct> unitActions;
static void (*onActionChange)(Unit, Action, Action);

void act_Register(Unit unit) {
  unitActions[unit_Reference(unit)] = (UnitActionStruct) {
    .unit = unit,
    .currentAction = act_Get(unit)
  };
}

void act_Init() {
  // unitActions.clear();
  // onActionChange = NULL;
}

void act_Kill(Unit unit) {
  unitActions.erase(unit_Reference(unit));
}

void act_SetAction(void (*method)(Unit, Action, Action)) {
  onActionChange = method;
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

void act_OnFrame() {
  for(auto &it : unitActions) {
    Action currentAction = act_Get(it.second.unit);
    if(!onActionChange || !act_AreActionDifferent(currentAction, it.second.currentAction)) {
      continue;
    }
    onActionChange(it.second.unit, currentAction, it.second.currentAction);
    unitActions[it.first] = (UnitActionStruct) {
      .unit = it.second.unit,
      .currentAction = currentAction
    };
  }
}