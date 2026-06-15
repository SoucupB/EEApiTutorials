#pragma once
#include <stdint.h>
#include "Geometry.h"

#define INVALID_TILE_ID UINT16_MAX

/*
 * Checks if the current tile is water tile.
 */
uint8_t map_Tile_IsWater(TilePoint self);
/*
 * Gets the plane ID from the current tile. Tiles belong to the same planes if there is connected path between them for either earth or water tiles.
 * Example: If 2 ground tiles have 2 different ID's, then, there is no passable terrain between tile 1 and tile 2.
 */
uint16_t map_Tile_GetPlaneID(TilePoint tile);
/*
 * Default method to check if a tile is an edge tile. (This is the method from Low Level Engine dll).
 */
uint8_t map_Tile_IsEdge(TilePoint self);
/*
 * Gets the depth of a tile. The depth is the taken from the center of the tile.
 */
float map_Tile_GetZ(TilePoint self);
/*
 * Checks if its a space map.
 */
uint8_t map_IsSpaceMap();
/*
 * Same functionality like map_Tile_IsWater but in space.
 */
uint8_t map_Tile_IsSpace(TilePoint self);
/*
 * This is a functionality whic either calls "map_Tile_IsSpace" or "map_Tile_IsWater" depending on the type of map.
 */
uint8_t map_Tile_IsSpecialTerrain(TilePoint self);