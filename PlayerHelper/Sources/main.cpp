#include <math.h>
#include "AttackDecision.h"
#include "PlayerState.h"
#include "ResourceManager.h"
#include "EETwa.h"
#include "Unit.h"
#include "EETypes.h"
#include "CitizenManager.h"
#include <iostream>

typedef struct SpawnLocation_t {
  TilePoint point;
  uint8_t player;
} SpawnLocation;

vector<SpawnLocation> capitolsLocations;

void att_LoadTransports(PVOID _);

void cleanDataChecker() {
  if(GetAsyncKeyState('J') & 0x8000) {
    Beep (300, 250);
  }
}

void randomIndexing(size_t *indexes, size_t sz) {
  for(size_t i = 0; i < sz; i++) {
    indexes[i] = i;
  }
  for(size_t i = 0; i < sz; i++) {
    size_t randomIndex = (rand() * rand()) % (sz - i) + i;
    swap(indexes[i], indexes[randomIndex]);
  }
}

uint8_t nonWorkersFilter(Unit unit) {
  UnitType def = unit_Type(unit);
  return !eeTypes_IsCitizen(def);
}

uint8_t workersFilter(Unit unit) {
  UnitType def = unit_Type(unit);
  return eeTypes_IsCitizen(def);
}

int8_t shouldBuildAttackUnit() {
  vector<Unit> units = unit_Player_GetUnits(ply_Self());
  vector<Unit> filteredUnits = unit_FilterFromArray(units, nonWorkersFilter);
  size_t currentPop = 0;
  for(size_t i = 0, c = filteredUnits.size(); i < c; i++) {
    currentPop += unit_Population(filteredUnits[i]);
  }
  return (float)currentPop < (float)ply_TotalPop(ply_Self()) * 0.6f;
}

int8_t shouldBuildWorkers() {
  vector<Unit> units = unit_Player_GetUnits(ply_Self());
  vector<Unit> filteredUnits = unit_FilterFromArray(units, workersFilter);
  size_t currentPop = 0;
  for(size_t i = 0, c = filteredUnits.size(); i < c; i++) {
    currentPop += unit_Population(filteredUnits[i]);
  }
  return (float)currentPop < (float)ply_TotalPop(ply_Self()) * 0.4f;
}

uint8_t type_IsCarrier(UnitType type) {
  return eeTypes_IsFromClass(CLASS_WATER_CARRIERS, type) || eeTypes_IsFromClass(CLASS_SPACE_CARRIER, type);
}

uint8_t navalAirCarrierFilter(Unit unit) {
  return att_IsUnitCarrier(unit);
}

uint8_t myCarriers(Unit unit) {
  return unit_IsSelf(unit) && att_IsUnitCarrier(unit);
}

UnitType navalYardExtractWithoutCarriers(Unit building, vector<UnitType> &types) {
  vector<UnitType> currentTypes;
  for(size_t i = 0, c = types.size(); i < c; i++) {
    if(!type_IsCarrier(types[i])) {
      currentTypes.push_back(types[i]);
    }
  }
  if(!currentTypes.size()) {
    return UNIT_UNDEFINED;
  }
  return currentTypes[rand() % currentTypes.size()];
}

UnitType navalYardBuildables(Unit building) {
  vector<Unit> carriers = unit_Filter(myCarriers);
  vector<UnitType> types = unit_AllBuildableTypes(building);
  if(!types.size()) {
    return UNIT_UNDEFINED;
  }
  if(carriers.size() >= 1) {
    return navalYardExtractWithoutCarriers(building, types);
  }
  return types[rand() % types.size()];
}

UnitType getTypeToBuild(Unit building) {
  if(unit_Type(building) == B_NAVY_YARD) {
    return navalYardBuildables(building);
  }
  vector<UnitType> types = unit_AllBuildableTypes(building);
  if(!types.size()) {
    return UNIT_UNDEFINED;
  }
  return types[rand() % types.size()];
}

void buildUnit_t(PVOID attr) {
  vector<Unit> buildings = unit_IdleBuildings(eeTa_SelfPlayer());
  if(!buildings.size() || !shouldBuildAttackUnit()) {
    return ;
  }
  size_t *indexes = (size_t *)malloc(sizeof(size_t) * buildings.size());
  randomIndexing(indexes, buildings.size());
  int32_t maxBuildings = 4;
  for(int32_t i = 0, c = buildings.size(); i < c; i++) {
    Unit building = buildings[indexes[i]];
    if(eeTypes_CanProduceWorkers(unit_Type(building))) {
      continue;
    }
    UnitType type = getTypeToBuild(building);
    if(type == UNIT_UNDEFINED) {
      continue;
    }
    unit_Building_Train(building, type);
    if(!maxBuildings) {
      break;
    }
    maxBuildings--;
  }
  free(indexes);
}

void buildAirCarrierUnits(PVOID attr) {
  vector<Unit> units = unit_Player_GetUnits(ply_Self());
  vector<Unit> filteredUnits = unit_FilterFromArray(units, navalAirCarrierFilter);
  for(int32_t i = 0, c = filteredUnits.size(); i < c; i++) {
    Unit naval = filteredUnits[i];
    if(!unit_Building_IsIdle(naval)) {
      continue;
    }
    vector<UnitType> types = unit_AllBuildableTypes(naval);
    if(!types.size()) {
      continue;
    }
    unit_Building_Train(naval, types[rand() % types.size()]);
  }
}

uint8_t capitolFilter(Unit unit) {
  UnitType def = unit_Type(unit);
  return eeTypes_CanProduceWorkers(def);
}

void bt_BuildWorkers(PVOID attr) {
  if(!shouldBuildWorkers()) {
    return ;
  }
  vector<Unit> buildings = unit_IdleBuildings(eeTa_SelfPlayer());
  vector<Unit> capitols = unit_FilterFromArray(buildings, capitolFilter);
  if(!capitols.size()) {
    return ;
  }
  int32_t maxBuildings = 3;
  for(int32_t i = 0, c = capitols.size(); i < c; i++) {
    Unit building = capitols[i];
    unit_Building_Train(building, (UnitType)CITIZEN);
    if(!maxBuildings) {
      break;
    }
    maxBuildings--;
  }
}

std::string convertToHex(int number) {
  char buffer[100] = {0};
  sprintf(buffer, "%x", number);
  return std::string(buffer);
}

void initCapitolPositions();

void checkEpochPointer() {
  if(GetAsyncKeyState('P') & 0x8000) {
    Beep (400, 250);
  }
}

void bt_HuntTransports(PVOID attributes) {
  vector<Unit> units = unit_Player_GetUnits(ply_Null());
  att_AttackTransportWithNavals(units);
}

void convert();

void bt_OnFrame() {
}

void bt_InitBuildingCreation() {
  TimeAtom atom;
  atom.method = (PVOID)buildUnit_t;
  atom.arguments = NULL;
  atom.time = 3567;
  eeTa_AddFrameMethod(atom);
}

void bt_InitTransportHunting() {
  TimeAtom atom;
  atom.method = (PVOID)bt_HuntTransports;
  atom.arguments = NULL;
  atom.time = 3548;
  eeTa_AddFrameMethod(atom);
}

void patrolCommand(PVOID attr) {
  vector<Unit> units = unit_Player_GetUnits(ply_Self());
  att_PatrolRandomPositions(units);
}

void bt_InitUnitMovement() {
  TimeAtom atom;
  atom.method = (PVOID)patrolCommand;
  atom.arguments = NULL;
  atom.time = 1115;
  eeTa_AddFrameMethod(atom);
}

void bt_InitBomberHunters() {
  TimeAtom atom;
  atom.method = (PVOID)initBomberHunters;
  atom.arguments = NULL;
  atom.time = 5700;
  eeTa_AddFrameMethod(atom);
}

void bt_InitWorkerCreators() {
  TimeAtom atom;
  atom.method = (PVOID)bt_BuildWorkers;
  atom.arguments = NULL;
  atom.time = 1928;
  eeTa_AddFrameMethod(atom);
}

void bt_InitAirplaneActions() {
  TimeAtom atom;
  atom.method = (PVOID)replaceMoveCommandForFliers;
  atom.arguments = NULL;
  atom.time = 3214;
  eeTa_AddFrameMethod(atom);
}

void bt_InitPortNaval() {
  TimeAtom atom;
  atom.method = (PVOID)buildAirCarrierUnits;
  atom.arguments = NULL;
  atom.time = 1528;
  eeTa_AddFrameMethod(atom);
}

void bt_InitHurricaneHunt() {
  TimeAtom atom;
  atom.method = (PVOID)att_HuntWithHurricane;
  atom.arguments = NULL;
  atom.time = 2678;
  eeTa_AddFrameMethod(atom);
}

void bt_InitPlaneHunterStorm() {
  TimeAtom atom;
  atom.method = (PVOID)att_HuntWithAirplaneStorm;
  atom.arguments = NULL;
  atom.time = 3678;
  eeTa_AddFrameMethod(atom);
}

void startFishCommand(PVOID attr) {
  res_MoveResourceBoats();
}

void bt_InitFisherBoats() {
  TimeAtom atom;
  atom.method = (PVOID)startFishCommand;
  atom.arguments = NULL;
  atom.time = 2115;
  eeTa_AddFrameMethod(atom);
}

void bt_RepairBuildings() {
  TimeAtom atom;
  atom.method = (PVOID)citizen_RepairBuildings;
  atom.arguments = NULL;
  atom.time = 2450;
  eeTa_AddFrameMethod(atom);
}

void bt_ConvertUnits() {
  TimeAtom atom;
  atom.method = (PVOID)att_ProcessSpecialAbilityUnits;
  atom.arguments = NULL;
  atom.time = 2550;
  eeTa_AddFrameMethod(atom);
}

void bt_AttackWithBombers() {
  TimeAtom atom;
  atom.method = (PVOID)att_AttackWithBombers;
  atom.arguments = NULL;
  atom.time = 1432;
  eeTa_AddFrameMethod(atom);
}

void bt_AttackWithShipsEverywhere() {
  TimeAtom atom;
  atom.method = (PVOID)att_AttackWithShips;
  atom.arguments = NULL;
  atom.time = 25034;
  eeTa_AddFrameMethod(atom);
}

void bt_RebuildMissingBuildings() {
  TimeAtom atom;
  atom.method = (PVOID)citizen_RebuildBuilding;
  atom.arguments = NULL;
  atom.time = 1344;
  eeTa_AddFrameMethod(atom);
}

void bt_AttackWithMechUnits() {
  TimeAtom atom;
  atom.method = (PVOID)att_AttackWithMechs;
  atom.arguments = NULL;
  atom.time = 14222;
  eeTa_AddFrameMethod(atom);
}

void bt_OnUnitDestroy(Unit unit) {
  citizen_DeleteBuilding(unit);
}

void bt_InitAggresiveness() {
  // int8_t *playerIndexes = eeTa_PlayerIDs();
  // for(size_t i = 0; i < 20; i++) {
  //   if(i != 1 && playerIndexes[i]) {
  //     eeTa_SetCvCAggression(i, 50.0f);
  //   }
  // }
}

SpawnLocation getSelfCapitol() {
  for(size_t i = 0; i < capitolsLocations.size(); i++) {
    if(capitolsLocations[i].player == eeTa_SelfPlayer()) {
      return capitolsLocations[i];
    }
  }

  return (SpawnLocation){
    .point = (TilePoint) {
      .x = 0,
      .y = 0
    },
    .player = 0xFF
  };
}

void att_LoadTransports(PVOID _) {
  SpawnLocation capitol = getSelfCapitol();
  if(capitol.player == 0xFF) {
    return ;
  }
  att_FillTransports(capitol.point);
}

void bt_LoadUnits() {
  TimeAtom atom;
  atom.method = (PVOID)att_LoadTransports;
  atom.arguments = NULL;
  atom.time = 6400;
  eeTa_AddFrameMethod(atom);
}

void initCapitolPositions() {
  capitolsLocations.clear();
  vector<Unit> units = unit_Player_GetBuildings(ply_Null());
  for(size_t i = 0; i < units.size(); i++) {
    if(!eeTypes_CanProduceWorkers(unit_Type(units[i]))) {
      continue;
    }
    SpawnLocation loc = (SpawnLocation) {
      .point = unit_Tile_Position(units[i]),
      .player = unit_GetPlayerIndex(units[i])
    };
    capitolsLocations.push_back(loc);
    eeTa_PrintfLine("Player %d (%d, %d) tile ID %d", loc.player, loc.point.x, loc.point.y, unit_GetPlaneID(units[i]));
  }
}

void bt_OnUnitCreate(Unit unit) {
}

void bt_OnGamePrepare() {
  att_Build();
  citizen_Create();
}

void bt_OnInit() {
  eeTa_AddCheat(CHEAT_COST_REDUCTION);
  initCapitolPositions();
  bt_InitUnitMovement();
  bt_InitBuildingCreation();
  bt_InitTransportHunting();
  bt_InitWorkerCreators();
  bt_InitPortNaval();
  bt_InitFisherBoats();
  bt_RepairBuildings();
  bt_ConvertUnits();
  bt_AttackWithBombers();
  bt_AttackWithShipsEverywhere();
  bt_InitHurricaneHunt();
  bt_RebuildMissingBuildings();
  bt_LoadUnits();
  bt_InitAirplaneActions();
  bt_InitBomberHunters();
  bt_InitPlaneHunterStorm();
  bt_AttackWithMechUnits();
  pls_OnInit(att_AddDamagedUnits);
}