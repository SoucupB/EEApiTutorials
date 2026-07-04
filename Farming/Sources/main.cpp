#include <math.h>
#include "Unit.h"
#include "Main.h"


uint8_t isCapitol(Unit unit) {
  return unit_IsSelf(unit) && unit_Building_IsIdle(unit) && eeTypes_CanProduceWorkers(unit_Type(unit));
}

void buildCitizens() {
  vector<Unit> idleCapitols = unit_Filter(isCapitol);
  for(size_t i = 0; i < idleCapitols.size(); i++) {
    unit_Building_Train(idleCapitols[i], CITIZEN);
  }
}

uint8_t isIdleCitizens(Unit unit) {
  return unit_IsSelf(unit) && unit_IsIdle(unit) && eeTypes_IsCitizen(unit_Type(unit));
}

Resource getClosestIron(Unit unit) {
  vector<Resource> resources = res_All();
  Resource response = res_Null();
  float minDistF = 1e9f;
  for(size_t i = 0; i < resources.size(); i++) {
    if(res_Type(resources[i]) != RES_IRON) {
      continue;
    }

    float currentDistF = geom_DistanceEuclidf(unit_Point_Position(unit), res_Point_Position(resources[i]));
    if(currentDistF < minDistF) {
      minDistF = currentDistF;
      response = resources[i];
    }
  }
  return response;
}

void farmIron() {
  vector<Unit> idleCitizens = unit_Filter(isIdleCitizens);
  for(size_t i = 0; i < idleCitizens.size(); i++) {
    Resource currentRes = getClosestIron(idleCitizens[i]);
    if(!res_Reference(currentRes)) {
      continue;
    }

    unit_Farm(idleCitizens[i], currentRes);
  }
}

/*
 * A method that will execute on each frame. The overall game state will not change until this method
 * finish its execution.
 */
void bt_OnFrame() {
  buildCitizens();
  farmIron();
}

/*
 * A method that will execute on the first game state. When this method executes, all the initial units
 * are available on things like "vector<Unit> unit_Filter(uint8_t (*method)(Unit));"
 */
void bt_OnInit() {

}

/*
 * A method that will execute on the when a unit is destroyed. The responsability of having a consistent state
 * regarding the implementation of the Bot needs to be implemented by the enginer user. In other words, if you issue
 * commands to a unit that does no longer exist anymore, it will result in "undefined" behaviour.
 * This method is not called only when a unit dies, it might happen with other events.
 */
void bt_OnUnitDestroy(Unit unit) {

}

/*
 * A method that will execute when a unit is created.
 */
void bt_OnUnitCreate(Unit unit) {
  
}

/*
 * This method executes when the game loads. This method should used to free and initialize different datastructures
 * that the developer will use to code behaviour. Please note that at this state of initialization there are not units
 * initialized
 */
void bt_OnGamePrepare() {
  
}