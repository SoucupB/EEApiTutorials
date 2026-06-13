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

/*
 * Gets the closest unit from "unit". Only the units on which filter(Unit) returns 1 are tested.
 */
Unit geom_GetClosestUnitFrom(Unit unit, uint8_t (*filter)(Unit));
/*
 * Checks if point is in circle.
 */
uint8_t geom_IsPointInCircle(Point p, Circle c);
/*
 * Converts a point to tile.
 */
TilePoint geom_Tile_FromPoint(Point target);
/*
 * Returns an invalid point.
 */
Point geom_Point_Invalid();
/*
 * Checks if the point is invalid. This method is generally used when other methods are returning points. If the methods do not find the needed point, they will return
 * an invalid point.
 */
uint8_t geom_Point_IsInvalid(Point point);
/*
 * Gets the euclidican distance between 2 points
 */
float geom_DistanceEuclidf(Point a, Point b);
/*
 * Converts a tile to a point.
 */
Point geom_Point_FromTile(TilePoint target);
/*
 * Invalid tile.
 */
TilePoint geom_Tile_Invalid();
/*
 * Checks if tile is invalid.
 */
uint8_t geom_Tile_IsInvalid(TilePoint tile);