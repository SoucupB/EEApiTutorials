#pragma once
#include <vector>
#include <map>
#include <windows.h>
#include "EETypesStruct.h"

using namespace std;

uint8_t eeTypes_IsFromClass(UnitClassType unitClass, UnitType unitType);
uint8_t eeTypes_IsWaterUnit(UnitType unitType);
uint8_t eeTypes_IsCitizen(UnitType unitType);
uint8_t eeTypes_IsWaterTransport(UnitType unitType);
uint8_t eeTypes_IsTransport(UnitType unitType);
uint8_t eeTypes_IsGroundUnit(UnitType unitType);
uint8_t eeTypes_IsFishBoat(UnitType unitType);
uint8_t eeTypes_IsPriest(UnitType type);
uint8_t eeTypes_IsAirUnit(UnitType unitType);
uint8_t eeTypes_CanProduceWorkers(UnitType type);
PVOID eeTypes_GetTemplate(UnitType typeIndex);
char *eeTypes_Name(UnitType type);
vector<AbilityTypes> eeTypes_Abilities(UnitType type);
size_t eeTypes_BuildingSize(UnitType type);

UnitClassType eeTypes_UnitClass(UnitType unitType);
uint8_t eeTypes_IsBuilding(UnitType unitType);

NeutralClassType eeTypes_Neutral_Type(NeutralUnitType unitType);
uint8_t eeTypes_Neutral_IsResource(NeutralUnitType unitType);