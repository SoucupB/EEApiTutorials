#include <math.h>
#include "Unit.h"
#include "Resource.h"

uint8_t isIdleCitizen(Unit unit) {
  const UnitType type = unit_Type(unit);
  return eeTypes_IsCitizen(type) && unit_IsIdle(unit) && unit_IsSelf(unit);
}

void bt_OnFrame() {
  vector<Unit> units = unit_Filter(isIdleCitizen);
  for(size_t i = 0; i < units.size(); i++) {
    vector<Resource> resources = res_All();
    float minDist = 100000.0f;
    Resource currentRes = res_Null();
    for(size_t j = 0; j < resources.size(); j++) {
      if(res_Type(resources[j]) == RES_FISH) {
        continue;
      }
      float currentDist = distanceEuclidf(unit_Point_Position(units[i]), res_Point_Position(resources[j]));
      if(minDist > currentDist) {
        currentRes = resources[j];
        minDist = currentDist;
      }
    }
    if(res_Reference(currentRes)) {
      unit_Farm(units[i], currentRes);
    }
  }
}

void bt_OnInit() {
  
}

void bt_OnUnitDestroy(Unit unit) {

}

void bt_OnUnitCreate(Unit unit) {
  
}