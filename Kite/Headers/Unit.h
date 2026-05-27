#pragma once
#include "EETwa.h"
#include <stdint.h>
#include "Resource.h"

typedef struct Unit_t Unit;

int8_t unit_IsDead(Unit unit);
vector<Unit> unit_GetBuildings(int8_t player);
vector<Unit> unit_GetUnits(int8_t player);
int32_t unit_CurrentlyBuilding(Unit building);
int8_t unit_IsBuilding(Unit unit);
TilePoint unit_Tile_Position(Unit unit);
uint16_t unit_GetPlaneID(Unit unit);
void unit_Point_CastAbility(Unit unit, Point target, AbilityTypes ability);
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