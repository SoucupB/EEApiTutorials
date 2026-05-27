#pragma once
#include "Unit.h"
#include "Action.h"
#include <deque>

typedef struct ActionWithFlag_t {
  Action action;
  uint8_t waitForIdle;
} ActionWithFlag;

typedef struct UnitActionQueue_t {
  Unit unit;
  deque<ActionWithFlag> *actionQueue;
} UnitActionQueue;

void act_Register(Unit unit);
void act_Kill(Unit unit);
void act_OnGamePrepare();
void act_OnFrame();
void act_PushFrontAction(Unit unit, Action action, uint8_t waitForIdle);
uint8_t act_IsIdle(Unit unit);