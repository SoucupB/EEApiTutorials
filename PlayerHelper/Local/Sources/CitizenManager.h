#pragma once
#include <windows.h>
#include "EETwa.h"
#include "Player.h"
#include "MapData.h"

void citizen_RepairBuildings(PVOID _);
void citizen_RebuildBuilding(PVOID _);
void citizen_RegisterBuilding(Unit unit);
void citizen_DeleteBuilding(Unit unit);