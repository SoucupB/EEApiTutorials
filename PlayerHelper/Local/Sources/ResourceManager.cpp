#include "ResourceManager.h"
#include "EETwa.h"
#include <vector>
#include "Geometry.h"
#include "Unit.h"
#include "Resource.h"

using namespace std;

uint8_t fishBoatsFilter(Unit unit) {
  UnitType def = unit_Type(unit);
  return eeTypes_IsFishBoat(def);
}

void fishTheFishes(Unit fishBoat) {
  vector<Resource> fishes = res_All();
  Resource currentFish = res_Null();
  float currentDist = 100000.0f;
  for(size_t i = 0, c = fishes.size(); i < c; i++) {
    if(res_Type(fishes[i]) != RES_FISH) {
      continue;
    }
    float currentPositionDist = geom_DistanceEuclidf(res_Point_Position(fishes[i]), unit_Point_Position(fishBoat));
    if(currentDist >= currentPositionDist) {
      currentDist = currentPositionDist;
      currentFish = fishes[i];
    }
  }
  if(!res_Reference(currentFish)) {
    return ;
  }
  unit_Farm(fishBoat, currentFish);
}

void res_MoveResourceBoats() {
  vector<Unit> units = unit_Player_GetUnits(ply_Self());
  vector<Unit> fishBoats = unit_FilterFromArray(units, fishBoatsFilter);

  int32_t maxSearches = 5;

  for(size_t i = 0, c = fishBoats.size(); i < c; i++) {
    Unit unit = fishBoats[i];
    if(unit_IsIdle(unit)) {
      fishTheFishes(unit);
      maxSearches--;
    }
    if(!maxSearches) {
      return ;
    }
  }
}