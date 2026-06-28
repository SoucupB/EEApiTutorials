#pragma once
#include <windows.h>
#include "Geometry.h"
#include "EETypes.h"

/*
 * A method that returns all the resources on the map.
 */
vector<Resource> res_All();
/*
 * Gets the reference of a resource. The reference is a unique 32 bit integer.
 */
PVOID res_Reference(Resource resource);
/*
 * Gets the resource name
 */
char *res_Name(Resource self);
/*
 * Gets the type of a resource. The types for resources have the prefix RES_
 */
NeutralUnitType res_Type(Resource unit);
/*
 * Returns a Resource with a null reference.
 */
Resource res_Null();
/*
 * Gets the tile position of the resource from param.
 */
TilePoint res_Tile_Position(Resource self);
/*
 * Gets the point position of the resource from param.
 */
Point res_Point_Position(Resource self);
/*
 * Checks if a resource still exists. Usually this is used for fishes, trees and spawning farm vegetables since they can get mined out quickly.
 */
uint8_t res_IsPresent(Resource resource);
/*
 * Gets the resource count from param for this specific player.
 */
int32_t res_GetResource(Player ply, ResourceIndex res);