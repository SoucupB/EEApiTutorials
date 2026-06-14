#pragma once
#include "EETwa.h"
#include <stdint.h>
#include "Resource.h"

typedef struct Unit_t Unit;

/*
 * Returns 1 if unit is dead or 0 if not.
 */
int8_t unit_IsDead(Unit unit);
/*
 * Gets an array of buildings from a specific player index.
 */
vector<Unit> unit_GetBuildings(int8_t player);
/*
 * Gets an array of units from a specific player index.
 */
vector<Unit> unit_GetUnits(int8_t player);
/*
 * Returns a type of unit if the current building is researching something. If its idle it will return the IDLE from EETypesStruct.
 */
int32_t unit_CurrentlyBuilding(Unit building);
/*
 * Checks if the unit is a building. 1 for yes and 0 for no.
 */
int8_t unit_IsBuilding(Unit unit);
/*
 * Gets unit tile position.
 */
TilePoint unit_Tile_Position(Unit unit);
/*
 * Gets the plane ID of the tile the unit is standing on.
 */
uint16_t unit_GetPlaneID(Unit unit);
/*
 * Cast an ability to a point, such as earthquake, volcano or hurricane.
 */
void unit_Point_CastAbility(Unit unit, Point target, AbilityTypes ability);
/*
 * Gets the unit type of the unit param.
 */
UnitType unit_Type(Unit unit);
int32_t unit_CurrentHp(Unit unit);
int32_t unit_TotalHP(Unit unit);
PVOID unit_Reference(Unit unit);
uint8_t unit_GetPlayerIndex(Unit unit);
int8_t unit_IsIdle(Unit unit);
Point unit_Point_Position(Unit unit);
int8_t unit_IsBuildingComplete(Unit unit);
int8_t unit_Building_IsIdle(Unit building);
Unit unit_Null();
uint8_t unit_CurrentEnergy(Unit unit);
float unit_Range(Unit unit);
float unit_Distance(Unit first, Unit dst);
vector<Unit> unit_IdleBuildings(int8_t player);
vector<Unit> unit_Filter(uint8_t (*method)(Unit));
vector<Unit> unit_FilterFromArray(vector<Unit> &units, uint8_t (*method)(Unit));
uint8_t unit_IsTransport(Unit unit);
size_t unit_Transport_Population(Unit transport);
vector<Unit> unit_FilterWithBuffer(uint8_t (*method)(Unit, PVOID), PVOID buffer);
vector<UnitType> unit_AllBuildableTypes(Unit unit);
int32_t unit_Population(Unit unit);
uint8_t unit_IsNeutral(Unit unit);
char *unit_Name(Unit unit);
vector<AbilityTypes> unit_Abilities(Unit unit);
Unit unit_SimpleUnitToUnit(SimpleUnit su);
uint8_t unit_IsSelf(Unit unit);
vector<Unit> unit_Player_GetBuildings(Player ply);
vector<Unit> unit_Player_GetUnits(Player ply);
uint8_t unit_IsEnemy(Unit unit);
int32_t unit_GetMaxFuel(Unit unit);
int32_t unit_GetCurrentFuel(Unit unit);

void unit_Object_CastAbility(Unit unit, Unit target, AbilityTypes ability);
uint8_t unit_CanBuild(Unit building, UnitType type);
uint8_t unit_Building_CanBuildAt(Unit citizen, UnitType buildingType, TilePoint tile);
/* This doesn't work correctly with Airports */
TilePoint unit_Building_FindBuildablePosition(Unit citizen, UnitType buildingType, TilePoint tile);
void unit_AttackTarget(Unit attacker, Unit target);
void unit_Build(Unit building, UnitType type);
void unit_Building_Build(Unit citizen, TilePoint tile, UnitType unitType);
void unit_Transport_Unload(Unit transport, TilePoint tile);
void unit_Transport_Load(Unit transport, vector<Unit> &units);
void unit_Convert(Unit src, Unit dst);
uint8_t unit_CanCast(Unit unit, AbilityTypes ability);
void unit_Action(Unit unit, Point point, UnitAction action);
void unit_Repair(Unit unit, Unit target);
void unit_Farm(Unit unit, Resource resource);
Unit unit_FromRerence(PVOID reference);
uint8_t unit_IsValid(Unit unit);
void unit_Costs(Unit unit, ResourceCost *costs, uint8_t *resCount);
void unit_Stop(Unit unit);