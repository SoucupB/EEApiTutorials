#include "CitizenManager.h"
#include "Unit.h"
#include "EETwa.h"
#include <unordered_map>
#include "Action.h"

typedef struct DeletedUnit_t {
  TilePoint tile;
  UnitType unitType;
} DeletedUnit;

static unordered_map<PVOID, DeletedUnit> buildings;

uint8_t needyBuilding(Unit unit) {
  return unit_GetPlayerIndex(unit) == eeTa_SelfPlayer() && unit_IsBuilding(unit) && unit_TotalHP(unit) > unit_CurrentHp(unit);
}

uint8_t canCitizenBeRequested(Unit unit) {
  return unit_IsIdle(unit) || act_IsWaitingToGather(unit); 
}

void citizen_RepairBuildings(PVOID _) {
  vector<Unit> needyBuildings = unit_Filter(needyBuilding);
  if(!needyBuildings.size()) {
    return ;
  }
  vector<Unit> units = unit_Player_GetUnits(ply_Self());
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(canCitizenBeRequested(units[i]) && eeTypes_IsCitizen(unit_Type(units[i]))) {
      unit_Repair(units[i], needyBuildings[(rand() * rand()) % needyBuildings.size()]);
    }
  }
}

uint8_t isIdleCitizen(Unit unit) {
  return eeTypes_IsCitizen(unit_Type(unit)) && canCitizenBeRequested(unit) && ply_Reference(ply_GetPlayer(unit)) == ply_Reference(ply_Self());
}

uint8_t citizen_BuildMissingBuilding(Unit citizen) {
  for(auto &it : buildings) {
    TilePoint tile = unit_Building_FindBuildablePosition(citizen, it.second.unitType, it.second.tile);
    if(geom_Tile_IsInvalid(tile)) {
      continue;
    }
    if(unit_CanBuild(citizen, it.second.unitType)) {
      unit_Building_Build(citizen, tile, it.second.unitType);
    }
    buildings.erase(it.first);
    return 1;
  }
  return 0;
}

void citizen_RebuildBuilding(PVOID _) {
  vector<Unit> units = unit_Filter(isIdleCitizen);
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(citizen_BuildMissingBuilding(units[i])) {
      break;
    }
  }
}

uint8_t citizen_IsNeededBuilding(const UnitType type) {
  switch (type)
  {
    case B_DOCK:
    case B_SPACE_DOCK:
    case B_SPACE_TURRET:
    case B_NAVY_YARD:
    case B_GUARD_TOWER_BAMBOO:
    case B_GUARD_TOWER_BRONZE:
    case B_GUARD_TOWER_COPPER:
    case B_GUARD_TOWER_DIGITAL:
    case B_GUARD_TOWER_IMPERIAL:
    case B_GUARD_TOWER_MIDDLE:
    case B_GUARD_TOWER_PALEO:
    case B_GUARD_TOWER_PALISADES:
    case B_GUARD_TOWER_SPACE:
    case B_GUARD_TOWER_WW1:
    case B_AA10_GUN_88MM:
    case B_AA13_MISSILE_TOWER:
    case B_AA15_MISSILE_TOWER:
      return 1;
    
    default:
      break;
  }

  return 0;
}

uint8_t citizen_IsRebuilable(Unit unit) {
  const UnitType type = unit_Type(unit);
  if(citizen_IsNeededBuilding(type) && ply_Reference(ply_GetPlayer(unit)) == ply_Reference(ply_Self())) {
    return 1;
  }
  return 0;
}

void citizen_DeleteBuilding(Unit unit) {
  if(!citizen_IsRebuilable(unit)) {
    return ;
  }
  buildings[unit_Reference(unit)] = (DeletedUnit) {
    .tile = unit_Tile_Position(unit),
    .unitType = unit_Type(unit)
  };
}