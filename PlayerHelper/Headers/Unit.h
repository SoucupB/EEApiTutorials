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
/*
 * Gets the current HP of the unit.
 */
int32_t unit_CurrentHp(Unit unit);
/*
 * Gets the total HP of the unit.
 */
int32_t unit_TotalHP(Unit unit);
/*
 * Gets the reference of the. This is a unique number (like an ID). No other units or reference will have this ID in the same time,
 * however, its not guaranteed that alive units cannot inherit this ID from units that died sometime ago.
 */
PVOID unit_Reference(Unit unit);
/*
 * Gets the player index of the current unit. Generally its 1 for the self player.
 */
uint8_t unit_GetPlayerIndex(Unit unit);
/*
 * Checks if a unit is idle.
 */
int8_t unit_IsIdle(Unit unit);
/*
 * Gets the exact coordinate position of the unit on the map.
 */
Point unit_Point_Position(Unit unit);
/*
 * Returns 1 if the building is complete. (for example, if the barracks can produce units).
 */
int8_t unit_IsBuildingComplete(Unit unit);
/*
 * Returns 1 if the building is not training any unit or research.
 */
int8_t unit_Building_IsIdle(Unit building);
/*
 * A unit instance with reference 0.
 */
Unit unit_Null();
/*
 * Gets the current energy of the unit.
 */
uint8_t unit_CurrentEnergy(Unit unit);
/*
 * Gets the the range of the unit. This also works on Melee units since all of them have a very short range, generally less than 1.
 */
float unit_Range(Unit unit);
/*
 * Returns the euclidian distance between 2 units.
 */
float unit_Distance(Unit first, Unit dst);
/*
 * Returns the euclidian distance between 2 units.
 */
vector<Unit> unit_IdleBuildings(int8_t player);
/*
 * A method which returns an array of units based on a filter method.
 * The method should return 1 if it wants the unit in the array or 0 otherwise.
 */
vector<Unit> unit_Filter(uint8_t (*method)(Unit));
/*
 * Functionally works like "unit_Filter" but it filter units present in "units" param
 */
vector<Unit> unit_FilterFromArray(vector<Unit> &units, uint8_t (*method)(Unit));
/*
 * Checks if unit transport other units, such as water transports, air transports and space transports. (This does not include ground transport if there are any).
 */
uint8_t unit_IsTransport(Unit unit);
/*
 * Gets the total population in a transport.
 */
size_t unit_Transport_Population(Unit transport);
/*
 * Works like filter "unit_Filter", but it also has an extra buffer which will be mirrored in "method" params.
 */
vector<Unit> unit_FilterWithBuffer(uint8_t (*method)(Unit, PVOID), PVOID buffer);
/*
 * Returns all buildable types of a unit. This includes citizens and buildings.
 * This does work only for currently available buildable types in the current epoch. If you are in prehistoric age, then, this
 * method will not return buildable types from nano age.
 */
vector<UnitType> unit_AllBuildableTypes(Unit unit);
/*
 * Gets the population that this unit takes.
 */
int32_t unit_Population(Unit unit);
/*
 * Checks if a unit is neutral.
 */
uint8_t unit_IsNeutral(Unit unit);
/*
 * Gets the name of a unit as a null terminated string.
 */
char *unit_Name(Unit unit);
/*
 * Gets the abilibites that this unit can cast.
 */
vector<AbilityTypes> unit_Abilities(Unit unit);
/*
 * Converts between 2 data types. Simple unit is just a unit that cannot be trained of build but can be casted or 
 * existed at map creation. (Hurricanes, animals, etc...)
 */
Unit unit_SimpleUnitToUnit(SimpleUnit su);
/*
 * Checks if the unit belongs to the player.
 */
uint8_t unit_IsSelf(Unit unit);
/*
 * Gets all the buildings that belong to the player in "ply" param.
 */
vector<Unit> unit_Player_GetBuildings(Player ply);
/*
 * Gets all the units (non buildings) that belong to the player in "ply" param.
 */
vector<Unit> unit_Player_GetUnits(Player ply);
/*
 * Returns 1 if the "unit" from param belongs to an enemy player.
 */
uint8_t unit_IsEnemy(Unit unit);
/*
 * Returns the maximum fuel a flying unit has. This will return 0 if the unit does not have fuel attribute.
 */
int32_t unit_GetMaxFuel(Unit unit);
/*
 * Returns the current fuel a flying unit has. This will return 0 if the unit does not have fuel attribute.
 */
int32_t unit_GetCurrentFuel(Unit unit);
/*
 * Casts an ability to "target" unit. This works for point and click abilities such as ABILITY_MECH_HADES_MP_TIMEWARP and ABILITY_MECH_HADES_MP_NANOVIRUS.
 */
void unit_Object_CastAbility(Unit unit, Unit target, AbilityTypes ability);
/*
 * Returns 1 if a building can build the required "type" in param, 0 otherwise.
 */
uint8_t unit_CanBuild(Unit building, UnitType type);
/*
 * Returns 1 if a citizen can build a building type at a position. This also takes into account if the citizen
 * can reach the required position. (This is the method that the game calls in order to tell the user if it can build at a specific tile).
 * It however does not work with airports yet since they are a special case.
 */
uint8_t unit_Building_CanBuildAt(Unit citizen, UnitType buildingType, TilePoint tile);
/*
 * Finds a buildable position close to "tile". If it cannot find one, it will return an invalid tile. (Same return as the method geom_Tile_Invalid()).
 * Also, does not work correctly with airports.
 */
TilePoint unit_Building_FindBuildablePosition(Unit citizen, UnitType buildingType, TilePoint tile);
/*
 * Orders a unit to attack a target unit. This works only if attacker belongs to the "self" player and if target is attackable at all. (aka whether its enemy or ally)
 */
void unit_AttackTarget(Unit attacker, Unit target);
/*
 * Orders a building to build a unit type. It works only if the building can do that.
 */
void unit_Build(Unit building, UnitType type);
/*
 * Orders a citizen to start a building at a position. This only works if the worker can actually build it there as if it was the user ordering that operation
 */
void unit_Building_Build(Unit citizen, TilePoint tile, UnitType unitType);
/*
 * Orders a transport to unload a group of units (if it has any) at a tile position.
 */
void unit_Transport_Unload(Unit transport, TilePoint tile);
/*
 * Orders a transport unit to load the units in the array param "units".
 */
void unit_Transport_Load(Unit transport, vector<Unit> &units);
/*
 * Orders a transport unit to load the units in the array param "units".
 */
void unit_Convert(Unit src, Unit dst);
/*
 * Returns 1 if a unit can cast a specific ability.
 */
uint8_t unit_CanCast(Unit unit, AbilityTypes ability);
/*
 * Orders a unit to execute an action such as move (UNIT_MOVE) or attack (UNIT_ATTACK) to a specific position on the map.
 */
void unit_Action(Unit unit, Point point, UnitAction action);
/*
 * Orders a citizen to repair a building.
 */
void unit_Repair(Unit unit, Unit target);
/*
 * Orders a citizen/fishing boat to gather the resource in param.
 */
void unit_Farm(Unit unit, Resource resource);
/*
 * Creates a unit instance from a refernce.
 */
Unit unit_FromRerence(PVOID reference);
/*
 * Returns 1 if the unit exists and if its alive.
 */
uint8_t unit_IsValid(Unit unit);
/*
 * This method will fill an array of resources that this unit costs. *resCount will be set with the amount
 * of different resources it needs.
 * Example for citizen it will be set to 1 since citizens only require food.
 */
void unit_Costs(Unit unit, ResourceCost *costs, uint8_t *resCount);
/*
 * Orders a unit to stop its current action.
 */
void unit_Stop(Unit unit);