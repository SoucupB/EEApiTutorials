#pragma once
#include "Unit.h"
#include "Geometry.h"
#include "Ability.h"

/* 
 * Possible actions that a unit can do.
 * Each action type will have set some of the parameters from below struct 'ActionType'.
 */
enum ActionType {
  ACTION_IDLE, // type
  ACTION_MOVE, // type, targetPoint
  ACTION_REPAIR, // type, target
  ACTION_ATTACK_AREA, // type, targetPoint
  ACTION_ATTACK_TARGET, // type, target
  ACTION_LOAD, // type, loadStartTarget, loadEndTarget
  ACTION_UNLOAD, // type, targetTile
  ACTION_CAST_AREA, // type, targetTile
  ACTION_CAST_TARGET, // type, target
  ACTION_GATHER // type, targetSimpleUnit
};

/* 
 * Struct that is returned for a unit that is currently doing an action.
 */
typedef struct Action_t {
  ActionType type;
  Unit target;
  SimpleUnit targetSimpleUnit;
  TilePoint targetTile;
  Point targetPoint;
  PVOID loadStartTarget;
  PVOID loadEndTarget;
  AbilityTypes ability;
} Action;

/* 
 * Get the action of the unit in param.
 */
Action act_Get(Unit unit);
/* 
 * This method returns true when a citizen waits for gathering resources or when the citizen is far away from the current resource.
 */
uint8_t act_IsWaitingToGather(Unit unit);