#pragma once
#include <windows.h>
#include <stdint.h>
#include "Unit.h"

/*
 * Gets the player index that this unit is belonging to.
 */
uint8_t ply_GetPlayerIndex(Unit unit);
/*
 * Gets the player object that this unit is belonging to.
 */
Player ply_GetPlayer(Unit unit);
/*
 * Gets the player reference. (A unique 32bit number).
 */
PVOID ply_Reference(Player self);
/*
 * Gets a player with reference 0. This is an invalid player and it should not be used with other queries.
 */
Player ply_Null();
/*
 * A method that returns 1 if the player a is allied with player b.
 */
uint8_t ply_AreAllies(Player a, Player b);
/*
 * Gets the current player.
 */
Player ply_Self();
/*
 * Gets the neutral player.
 */
Player ply_Neutral();
/*
 * Gets the total amount of players. Includes the neutral player.
 */
int8_t ply_PlayerCount();
/*
 * Gets an array with all active players.
 */
vector<Player> ply_All();
/*
 * Checks whether 2 players are allies but instead of having player refs as params, they have players indexes.
 */
int8_t ply_Index_AreAllies(uint8_t plySrc, uint8_t plyDst);
/*
 * Gets the player index.
 */
uint8_t ply_PlayerIndex(Player player);
/*
 * Gets the player current population count.
 */
int32_t ply_CurrentPopulation(Player player);
/*
 * Gets the player total population count.
 */
int32_t ply_TotalPop(Player player);
/*
 * Gets the player tech tree object.
 */
TechTree ply_TechTree(Player player);
/*
 * Gets the resources of a player.
 */
int32_t ply_GetResources(Player player, ResourceIndex resource);
/*
 * Sets the resources of a player.
 */
void ply_SetResources(Player player, ResourceIndex resource, int32_t value);
