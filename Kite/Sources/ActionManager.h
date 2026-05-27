#pragma once
#include "Unit.h"
#include "Action.h"
#include <deque>

typedef struct UnitActionQueue_t {
  Unit unit;
  deque<Action> *actionQueue;
} UnitActionQueue;

void act_Register(Unit unit);
void act_Kill(Unit unit);
void act_OnGamePrepare();