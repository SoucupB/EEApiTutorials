#pragma once
#include <vector>
#include <map>
#include <windows.h>
#include "EETypesStruct.h"

using namespace std;

/*
 * Checks if a unit type is from a specific class.
 */
uint8_t eeTypes_IsFromClass(UnitClassType unitClass, UnitType unitType);
/*
 * Checks if a unit type is a unit that can go through water (attacking/farming/transport boats)
 */
uint8_t eeTypes_IsWaterUnit(UnitType unitType);
/*
 * Checks if a unit type is a citizen (female/male)
 */
uint8_t eeTypes_IsCitizen(UnitType unitType);
/*
 * Checks if a unit type is a water transport boat (any epoch)
 */
uint8_t eeTypes_IsWaterTransport(UnitType unitType);
/*
 * Checks if a unit type is a transport, both water and flying unit.
 */
uint8_t eeTypes_IsTransport(UnitType unitType);
/*
 * Checks if a unit type is a ground unit but buildings are not included.
 */
uint8_t eeTypes_IsGroundUnit(UnitType unitType);
/*
 * Checks if a unit type is a fish boat
 */
uint8_t eeTypes_IsFishBoat(UnitType unitType);
/*
 * Checks if a unit type is a priest
 */
uint8_t eeTypes_IsPriest(UnitType type);
/*
 * Checks if a unit type is a plane, helicopter or any other type which flies.
 */
uint8_t eeTypes_IsAirUnit(UnitType unitType);
/*
 * Checks if a unit type is either a capitol or town center.
 */
uint8_t eeTypes_CanProduceWorkers(UnitType type);
/*
 * Gets a pointer to the string which represents the name of the unit type.
 */
char *eeTypes_Name(UnitType type);
/*
 * Gets a vector of Ability types of the current unit.
 */
vector<AbilityTypes> eeTypes_Abilities(UnitType type);
/*
 * Gets the building size in tiles. (For non buildings its 0)
 */
size_t eeTypes_BuildingSize(UnitType type);
/*
 * Gets the class of the unit type. All the classes are under (enum UnitClassType) from EETypesStruct.h.
 */
UnitClassType eeTypes_UnitClass(UnitType unitType);
/*
 * Checks if the unit type is a building.
 */
uint8_t eeTypes_IsBuilding(UnitType unitType);
/*
 * Gets the class of the neutral unit type. All the classes are under (enum NeutralClassType) from EETypesStruct.h.
 */
NeutralClassType eeTypes_Neutral_Type(NeutralUnitType unitType);
/*
 * Checks if the neutral unit type is a resource.
 */
uint8_t eeTypes_Neutral_IsResource(NeutralUnitType unitType);
/*
 * Gets the costs of a unit type.
 */
void eeTypes_Costs(Player ply, UnitType type, ResourceCost *costs, uint8_t *resCount);