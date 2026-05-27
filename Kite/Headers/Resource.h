#pragma once
#include <windows.h>
#include "Geometry.h"
#include "EETypes.h"

vector<Resource> res_All();
PVOID res_Reference(Resource resource);
char *res_Name(Resource self);
NeutralUnitType res_Type(Resource unit);
Resource res_Null();
TilePoint res_Tile_Position(Resource self);
Point res_Point_Position(Resource self);
uint8_t res_IsPresent(Resource resource);