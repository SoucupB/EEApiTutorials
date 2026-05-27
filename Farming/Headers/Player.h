#pragma once
#include <windows.h>
#include <stdint.h>
#include "Unit.h"

uint8_t ply_GetPlayerIndex(Unit unit);
Player ply_GetPlayer(Unit unit);
PVOID ply_Reference(Player self);
Player ply_Null();
uint8_t ply_AreAllies(Player a, Player b);
Player ply_Self();
Player ply_Neutral();
int8_t ply_PlayerCount();
vector<Player> ply_All();
int8_t ply_Index_AreAllies(uint8_t plySrc, uint8_t plyDst);
uint8_t ply_PlayerIndex(Player player);
int32_t ply_CurrentPopulation(Player player);
int32_t ply_TotalPop(Player player);
TechTree ply_TechTree(Player player);
