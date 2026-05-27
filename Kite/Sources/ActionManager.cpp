#include "ActionManager.h"
#include <map>
#include <windows.h>
#include "SimpleUnit.h"

static map<PVOID, UnitActionQueue> unitActions;

void act_Register(Unit unit) {
  unitActions[unit_Reference(unit)] = (UnitActionQueue) {
    .unit = unit,
    .actionQueue = new deque<Action>()
  };
}

void act_Init() {
}

void act_PushFrontAction(Unit unit, Action action) {
  const PVOID ref = unit_Reference(unit);
  unitActions[ref].actionQueue->push_front(action);
}

void act_OnGamePrepare() {
  unitActions.clear();
}

void act_Kill(Unit unit) {
  const PVOID ref = unit_Reference(unit);
  delete unitActions[ref].actionQueue;
  unitActions.erase(ref);
}

void act_OnFrame() {
}