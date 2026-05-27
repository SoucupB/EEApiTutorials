#pragma once
#include "EETypes.h"

typedef struct Point_t {
  float x;
  float y;
} Point;

typedef struct TilePoint_t {
  int32_t x;
  int32_t y;
} TilePoint;

typedef struct Circle_t {
  Point p;
  float radius;
} Circle;

Unit geom_GetClosestUnitFrom(Unit unit, Player ply, uint8_t (*filter)(Unit));
uint8_t geom_IsPointInCircle(Point p, Circle c);
TilePoint geom_Tile_FromPoint(Point target);
Point geom_Point_Invalid();
uint8_t geom_Point_IsInvalid(Point point);
float distanceEuclidf(Point a, Point b);
Point geom_Point_FromTile(TilePoint target);
TilePoint geom_Tile_Invalid();
uint8_t geom_Tile_IsInvalid(TilePoint tile);