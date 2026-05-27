#pragma once
#include "Unit.h"
#include "Action.h"

typedef struct UnitActionStruct_t {
  Unit unit;
  Action currentAction;
} UnitActionStruct;

void act_Register(Unit unit);
void act_Kill(Unit unit);
void act_OnFrame();
void act_Init();
void act_SetAction(void (*method)(Unit, Action, Action));