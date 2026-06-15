#pragma once

#include <Windows.h>
#include <stdint.h>
#include <vector>
#include "EETwa.h"
#include "MapData.h"
#include "Player.h"

using namespace std;

void att_AttackTransportWithNavals(vector<Unit> &units);
void att_PatrolRandomPositions(vector<Unit> &selfUnits);
void att_AddDamagedUnits(Unit unit);
uint8_t att_IsUnitCarrier(Unit unit);
void att_ProcessSpecialAbilityUnits(PVOID _);
void att_AttackWithBombers(PVOID _);
void att_AttackWithShips(PVOID _);
void att_FillTransports(TilePoint capitolPoint);
void att_HuntWithHurricane(PVOID _);
void att_HuntWithHades(PVOID _);
void initBomberHunters(PVOID _);
void replaceMoveCommandForFliers(PVOID _);
void att_HuntWithAirplaneStorm(PVOID _);