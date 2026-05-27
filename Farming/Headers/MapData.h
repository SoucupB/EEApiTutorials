#pragma once
#include <stdint.h>
#include "Geometry.h"

#define INVALID_TILE_ID UINT16_MAX

uint8_t map_Tile_IsWater(TilePoint self);
uint16_t map_Tile_GetPlaneID(TilePoint tile);
uint8_t map_Tile_IsEdge(TilePoint self);
float map_Tile_GetZ(TilePoint self);
uint8_t map_IsSpaceMap();
uint8_t map_Tile_IsSpace(TilePoint self);