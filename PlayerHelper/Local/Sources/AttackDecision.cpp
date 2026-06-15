#include "AttackDecision.h"
#include "Geometry.h"
#include "EETwa.h"
#include "EETypes.h"
#include "PlayerState.h"
#include <math.h>
#include <map>
#include "Unit.h"
#include "Player.h"
#include "SimpleUnit.h"
#include "Action.h"

static map<pair<float, float>, uint8_t> attackedUnits;

uint8_t isMechUnit(Unit unit);

uint8_t idleAttackingWaterUnits(Unit unit);
uint8_t isFlyingBomber(Unit unit);
uint8_t attackingWaterUnits(Unit unit);
uint8_t isSelfUnit(Unit unit);
uint8_t attackingGroundUnits(Unit unit);

size_t min(size_t a, size_t b) {
  return a < b ? a : b;
}

uint8_t navalAttackFilter(Unit unit) {
  UnitType def = unit_Type(unit);
  return attackingWaterUnits(unit) && eeTa_Player(unit) == eeTa_SelfPlayer();
}

uint8_t priestsFilters(Unit unit) {
  UnitType def = unit_Type(unit);
  return eeTypes_IsPriest(def);
}

uint8_t enemyFilter(Unit unit) {
  uint8_t target = eeTa_Player(unit);
  uint8_t self = eeTa_SelfPlayer();

  return target != self && !ply_Index_AreAllies(target, self);
}

uint8_t enemyTransporters(Unit unit) {
  UnitType def = unit_Type(unit);
  int8_t unitPlayer = eeTa_Player(unit);
  int8_t playerTeam = eeTa_SelfPlayer();
  return eeTypes_IsTransport(def) &&
         unitPlayer != playerTeam && !ply_Index_AreAllies(unitPlayer, playerTeam);
}

uint8_t idleSelfTransporters(Unit unit) {
  UnitType def = unit_Type(unit);
  int8_t unitPlayer = eeTa_Player(unit);
  int8_t playerTeam = eeTa_SelfPlayer();
  return unit_IsIdle(unit) && eeTypes_IsTransport(def) &&
         unitPlayer == playerTeam && unit_Transport_Population(unit) <= 8;
}

uint8_t enemyUnits(Unit unit) {
  UnitType def = unit_Type(unit);
  int8_t unitPlayer = eeTa_Player(unit);
  int8_t playerTeam = eeTa_SelfPlayer();
  return unitPlayer != playerTeam && !ply_Index_AreAllies(unitPlayer, playerTeam) &&
         !unit_IsNeutral(unit);
}

uint8_t idleAttackingGroundUnits(Unit unit) {
  UnitType def = unit_Type(unit);
  return unit_IsIdle(unit) && eeTypes_IsGroundUnit(def) && !eeTypes_IsFromClass(CLASS_CITIZENS, def);
}

uint8_t attackingGroundUnits(Unit unit) {
  UnitType def = unit_Type(unit);
  return isSelfUnit(unit) && eeTypes_IsGroundUnit(def) && !eeTypes_IsFromClass(CLASS_CITIZENS, def);
}

uint8_t idleAttackingAirUnits(Unit unit) {
  UnitType def = unit_Type(unit);
  return unit_IsIdle(unit) && eeTypes_IsAirUnit(def) && !eeTypes_IsFromClass(CLASS_AIR_TRANSPORT, def);
}

uint8_t idleAttackingWaterUnits(Unit unit) {
  UnitType def = unit_Type(unit);
  return unit_IsIdle(unit) && (eeTypes_IsFromClass(CLASS_WATER_BOATS, def) || eeTypes_IsFromClass(CLASS_SUBMARINES, def));
}

uint8_t idleAttackingSpaceUnits(Unit unit) {
  UnitType def = unit_Type(unit);
  return unit_IsIdle(unit) && (eeTypes_IsFromClass(CLASS_SPACE_FIGHTER, def) || eeTypes_IsFromClass(CLASS_SPACE_SHIP_FIGHTERS, def));
}

uint8_t attackingWaterUnits(Unit unit) {
  UnitType def = unit_Type(unit);
  return eeTypes_IsFromClass(CLASS_WATER_BOATS, def) || eeTypes_IsFromClass(CLASS_SUBMARINES, def);
}

void att_AddDamagedUnits(Unit unit) {
  if(eeTa_Player(unit) != eeTa_SelfPlayer()) {
    return ;
  }
  UnitType def = unit_Type(unit);
  if(!unit_IsBuilding(unit) && !eeTypes_IsCitizen(def)) {
    return ;
  }
  Point pos = unit_Point_Position(unit);
  attackedUnits[make_pair(pos.x, pos.y)] = 1;
}

void att_AttackTransportWithNavals(vector<Unit> &units) {
  vector<Unit> filteredUnits = unit_FilterFromArray(units, navalAttackFilter);

  for(uint32_t i = 0, c = min(6, filteredUnits.size()); i < c; i++) {
    vector<Unit> enemyTransports = unit_FilterFromArray(units, enemyTransporters);

    for(uint32_t j = 0, p = enemyTransports.size(); j < p; j++) {
      Point currPos = unit_Point_Position(filteredUnits[i]);
      Point enemyPos = unit_Point_Position(enemyTransports[j]);

      Circle circle = (Circle) {
        .p = currPos,
        .radius = 15.0f
      };

      if(geom_IsPointInCircle(enemyPos, circle)) {
        unit_Action(filteredUnits[i], enemyPos, UNIT_ATTACK);
        break;
      }
    }
  }
}

uint8_t isAirPositionValid(Unit unit, TilePoint tile) {
  const UnitType type = unit_Type(unit);
  if(map_Tile_GetPlaneID(tile) == INVALID_TILE_ID) {
    return 0;
  }
  if(map_IsSpaceMap()) {
    if(eeTypes_IsFromClass(CLASS_SPACE_FIGHTER, type) || type == A_FIGHTER15_PLANETARY_FIGHTER) {
      return 1;
    }
    return !map_Tile_IsSpace(tile);
  }
  return 1;
}

Point airNextPosition(Unit unit) {
  Point currentPosition = unit_Point_Position(unit);
  Point copyPosition = currentPosition;
  currentPosition.x = copyPosition.x + sinf(rand()) * 20.0f;
  currentPosition.y = copyPosition.y + sinf(rand()) * 20.0f;
  int32_t index = 5;
  while(index && !isAirPositionValid(unit, geom_Tile_FromPoint(currentPosition))) {
    currentPosition.x = copyPosition.x + sinf(rand()) * 20.0f;
    currentPosition.y = copyPosition.y + sinf(rand()) * 20.0f;
    index--;
  }
  if(!index) {
    return geom_Point_Invalid();
  }
  return currentPosition;
}

Point getNextPosition(Unit unit) {
  if(eeTypes_IsAirUnit(unit_Type(unit))) {
    return airNextPosition(unit);
  }
  Point currentPosition = unit_Point_Position(unit);
  Point copyPosition = currentPosition;
  currentPosition.x += sinf(rand()) * 20.0f;
  currentPosition.y += sinf(rand()) * 20.0f;
  uint16_t tilePoint = map_Tile_GetPlaneID(geom_Tile_FromPoint(copyPosition));
  int32_t index = 5;
  while(index && tilePoint != INVALID_TILE_ID && tilePoint != map_Tile_GetPlaneID(geom_Tile_FromPoint(currentPosition))) {
    currentPosition.x = copyPosition.x + sinf(rand()) * 20.0f;
    currentPosition.y = copyPosition.y + sinf(rand()) * 20.0f;
    index--;
  }
  if(!index) {
    return geom_Point_Invalid();
  }
  return currentPosition;
}

Point att_RandomMove(Unit unit) {
  return getNextPosition(unit);
}

uint8_t att_ScanAndAttack(vector<Unit> &selfUnits) {
  if(!attackedUnits.size()) {
    return 0;
  }
  int32_t maxCommands = 12;
  for(auto &it : attackedUnits) {
    uint8_t hasTroopsBeenSent = 0;
    for(size_t i = 0, c = selfUnits.size(); i < c; i++) {
      if(idleAttackingGroundUnits(selfUnits[i]) || idleAttackingAirUnits(selfUnits[i])) {
        hasTroopsBeenSent = 1;
        maxCommands--;
      }
      else {
        continue;
      }
      Point p = (Point) {
        .x = it.first.first,
        .y = it.first.second
      };
      unit_Action(selfUnits[i], p, UNIT_ATTACK);
      if(!maxCommands) {
        return 1;
      }
    }
    attackedUnits.erase(it.first);
    return hasTroopsBeenSent;
  }
  return 0;
}

void att_PatrolRandomPositions_t(vector<Unit> &selfUnits, uint8_t (*checker)(Unit), int32_t maxCommands) {
  vector<Unit> filteredUnits = unit_FilterFromArray(selfUnits, checker);
  if(!filteredUnits.size()) {
    return ;
  }
  if(att_ScanAndAttack(filteredUnits)) {
    return ;
  }
  Point randomPos = att_RandomMove(filteredUnits[0]);
  if(geom_Point_IsInvalid(randomPos)) {
    return ;
  }
  for(size_t i = 0, c = filteredUnits.size(); i < c; i++) {
    maxCommands--;
    unit_Action(filteredUnits[i], randomPos, UNIT_ATTACK);
    if(!maxCommands) {
      return ;
    }
  }
}

uint8_t isFlyingBomber(Unit unit) {
  UnitType type = unit_Type(unit);
  switch (type)
  {
    case A_BOMBERNUC13_TITAN:
      return 1;
    case A_BOMBER_10_GOTHA:
      return 1;
    case A_BOMBER_13_B_122_WYVERN:
      return 1;
    case A_BOMBERNUC12_B_52:
      return 1;
    case A_BOMBER_11_B_17:
      return 1;
    case A_BOMBERNUC11_B_29:
      return 1;
    case A_BOMBER_12_STEALTH_B_2:
      return 1;
    
    default:
      break;
  }
  return 0;
}

uint8_t isEnemyFlyingBomber(Unit unit) {
  return isFlyingBomber(unit) && unit_GetPlayerIndex(unit) != eeTa_SelfPlayer() && !ply_Index_AreAllies(unit_GetPlayerIndex(unit), eeTa_SelfPlayer());
}

uint8_t isSelfFlyingCombustionUnit(Unit unit) {
  const UnitType type = unit_Type(unit);
  return isSelfUnit(unit) && (eeTypes_IsFromClass(CLASS_AIR_COMBUSTION_FLYEIR, type) || eeTypes_IsFromClass(CLASS_SPACE_FIGHTER, type));
}

void attackEnemyBombers(Unit bomber[], size_t bomberCount) {
  if(!bomberCount) {
    return ;
  }
  vector<Unit> airplanes = unit_Filter(isSelfFlyingCombustionUnit);
  for(size_t i = 0, c = airplanes.size(); i < c; i++) {
    const size_t bomberIndex = i / bomberCount;
    if(bomberIndex >= bomberCount) {
      break;
    }
    unit_Action(airplanes[i], unit_Point_Position(bomber[bomberIndex]), UNIT_ATTACK);
  }
}

void processEnemyBomber(Unit bomber, Unit bombers[], size_t *count) {
  Action currentAction = act_Get(bomber);
  if(currentAction.type == ACTION_ATTACK_TARGET && isSelfUnit(currentAction.target)) {
    bombers[(*count)++] = bomber;
  }
}

void replaceMoveCommandForFliers(PVOID _) {
  vector<Unit> airplanes = unit_Filter(isSelfFlyingCombustionUnit);
  for(size_t i = 0, c = airplanes.size(); i < c; i++) {
    Action currentAction = act_Get(airplanes[i]);
    if(currentAction.type == ACTION_MOVE) {
      unit_Action(airplanes[i], currentAction.targetPoint, UNIT_ATTACK);
    }
  }
}

void initBomberHunters(PVOID _) {
  vector<Unit> airplanes = unit_Filter(isEnemyFlyingBomber);
  Unit units[64];
  size_t count = 0;
  for(size_t i = 0, c = airplanes.size(); i < c; i++) {
    if(count >= sizeof(units) / sizeof(units[0])) {
      break;
    }
    processEnemyBomber(airplanes[i], units, &count);
  }
  attackEnemyBombers(units, count);
}

uint8_t isTower(Unit unit) {
  UnitType type = unit_Type(unit);
  switch (type)
  {
    case B_GUARD_TOWER_BAMBOO:
      return 1;
    case B_GUARD_TOWER_BRONZE:
      return 1;
    case B_GUARD_TOWER_COPPER:
      return 1;
    case B_GUARD_TOWER_DIGITAL:
      return 1;
    case B_GUARD_TOWER_IMPERIAL:
      return 1;
    case B_GUARD_TOWER_MIDDLE:
      return 1;
    case B_GUARD_TOWER_PALEO:
      return 1;
    case B_GUARD_TOWER_PALISADES:
      return 1;
    case B_GUARD_TOWER_SPACE:
      return 1;
    case B_GUARD_TOWER_WW1:
      return 1;
    case B_SPACE_TURRET:
      return 1;
    
    default:
      break;
  }
  return 0;
}

uint8_t isEnemyTower(Unit unit) {
  return unit_GetPlayerIndex(unit) != eeTa_SelfPlayer() && !ply_Index_AreAllies(unit_GetPlayerIndex(unit), eeTa_SelfPlayer()) && unit_GetPlayerIndex(unit) != eeTa_NeutralPlayer() && isTower(unit);
}

uint8_t att_IsUnitCarrier(Unit unit) {
  return eeTypes_IsFromClass(CLASS_WATER_CARRIERS, unit_Type(unit)) || eeTypes_IsFromClass(CLASS_SPACE_CARRIER, unit_Type(unit));
}

uint8_t isPriest(Unit unit) {
  return unit_GetPlayerIndex(unit) == eeTa_SelfPlayer() && unit_Type(unit) == PRIEST;
}

uint8_t isProphet(Unit unit) {
  return unit_GetPlayerIndex(unit) == eeTa_SelfPlayer() && unit_Type(unit) == PROPHET;
}

uint8_t isHades(Unit unit) {
  return unit_GetPlayerIndex(unit) == eeTa_SelfPlayer() && unit_Type(unit) == MECH_HADES;
}

uint8_t isTempest(Unit unit) {
  return isSelfUnit(unit) && unit_Type(unit) == MECH_TEMPEST;
}

uint8_t isSelfUnit(Unit unit) {
  return unit_GetPlayerIndex(unit) == eeTa_SelfPlayer();
}

uint8_t isEnemyUnit(Unit unit) {
  return unit_GetPlayerIndex(unit) != eeTa_SelfPlayer() && !ply_Index_AreAllies(unit_GetPlayerIndex(unit), eeTa_SelfPlayer()) && unit_GetPlayerIndex(unit) != eeTa_NeutralPlayer();
}

uint8_t enemyNonFlyingUnit(Unit unit) {
  return isEnemyUnit(unit) && !unit_IsBuilding(unit) && !eeTypes_IsAirUnit(unit_Type(unit));
}

uint8_t enemyUnitFilter(Unit unit) {
  return enemyUnits(unit) && !eeTypes_IsBuilding(unit_Type(unit));
}

uint8_t enemyFlyingUnits(Unit unit) {
  UnitType type = unit_Type(unit);
  return isEnemyUnit(unit) && eeTypes_IsAirUnit(type);
}

uint8_t enemyBuilding(Unit unit) {
  return unit_GetPlayerIndex(unit) != eeTa_SelfPlayer() && !ply_Index_AreAllies(unit_GetPlayerIndex(unit), eeTa_SelfPlayer()) && unit_GetPlayerIndex(unit) != eeTa_NeutralPlayer() && unit_IsBuilding(unit);
}

void att_AttackWithBombers(PVOID _) {
  vector<Unit> airplanes = unit_Filter(isFlyingBomber);
  vector<Unit> enemyBuildings = unit_Filter(enemyBuilding);
  for(size_t i = 0, t = enemyBuildings.size(), c = airplanes.size(); i < c; i++) {
    if(unit_IsIdle(airplanes[i])) {
      float currentDist = 100000.0f;
      Unit currentBuilding = unit_Null();
      for(size_t j = 0; j < t; j++) {
        float dist = unit_Distance(airplanes[i], enemyBuildings[j]);
        if(currentDist > dist) {
          currentDist = dist;
          currentBuilding = enemyBuildings[j];
        }
      }
      if(!unit_Reference(currentBuilding)) {
        continue;
      }
      unit_Action(airplanes[i], unit_Point_Position(currentBuilding), UNIT_ATTACK);
    }
  }
}

uint8_t groundUnitsCapitor(Unit unit, PVOID buffer) {
  uint16_t pointerID = *(uint16_t *)buffer;
  return attackingGroundUnits(unit) && map_Tile_GetPlaneID(unit_Tile_Position(unit)) == pointerID;
}

vector<vector<Unit> > getSplittedUnits(vector<Unit> &units, uint32_t totalUnits) {
  vector<vector<Unit> > response;
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(!(i % totalUnits)) {
      response.push_back(vector<Unit>());
    }
    response[response.size() - 1].push_back(units[i]);
  }
  return response;
}

void att_FillTransports(TilePoint capitolPoint) {
  uint16_t capitolID = map_Tile_GetPlaneID(capitolPoint);
  vector<Unit> insularUnits = unit_FilterWithBuffer(groundUnitsCapitor, (PVOID)&capitolID);
  uint32_t totalUnits = 6;
  if(insularUnits.size() <= 25) {
    return ;
  }
  vector<Unit> transports = unit_Filter(idleSelfTransporters);
  vector<vector<Unit> > units = getSplittedUnits(insularUnits, totalUnits);
  for(size_t i = 0, c = min(transports.size(), units.size()); i < c; i++) {
    unit_Transport_Load(transports[i], units[i]);
  }
}

void att_AttackWithShips(PVOID _) {
  vector<Unit> waterUnits = unit_Filter(navalAttackFilter);
  if(waterUnits.size() < 8) {
    return ;
  }
  vector<Unit> enemyBuildings = unit_Filter(isEnemyTower);
  for(size_t i = 0, c = waterUnits.size(), t = enemyBuildings.size(); i < c; i++) {
    Unit currentBuilding = unit_Null();
    float currentDist = 100000.0f;
    for(size_t j = 0; j < t; j++) {
      float dist = unit_Distance(waterUnits[i], enemyBuildings[j]);
      if(currentDist > dist) {
        currentDist = dist;
        currentBuilding = enemyBuildings[j];
      }
    }
    if(!unit_Reference(currentBuilding)) {
      continue;
    }
    unit_Action(waterUnits[i], unit_Point_Position(currentBuilding), UNIT_ATTACK);
  }
}

void pickRandomUnits(vector<Unit> &units, int32_t count, uint32_t *index, size_t indexSize) {
  int32_t countArr = min(indexSize, min(count, units.size()));
  int32_t maxCountArr = min(units.size(), indexSize);
  for(int32_t i = 0; i < maxCountArr; i++) {
    index[i] = i;
  }
  for(int32_t i = 0; i < countArr; i++) {
    if(maxCountArr - i - 1 <= 0) {
      break;
    }
    int32_t newIndex = (rand() * rand()) % (maxCountArr - i - 1) + i + 1;
    swap(index[i], index[newIndex]);
  }
}

void priest_ConvertIfPossible(Unit priest) {
  vector<Unit> units = unit_Filter(enemyNonFlyingUnit);
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(unit_Distance(priest, units[i]) <= unit_Range(priest)) {
      unit_Convert(priest, units[i]);
      return ;
    }
  }
}

void att_ProcessPriests() {
  size_t total = 5;
  vector<Unit> units = unit_Filter(isPriest);
  uint32_t newIndexes[256];
  pickRandomUnits(units, total, newIndexes, sizeof(newIndexes) / sizeof(newIndexes[0]));
  for(size_t i = 0, c = min(total, units.size()); i < c; i++) {
    priest_ConvertIfPossible(units[newIndexes[i]]);
  }
}

void prophet_CastMalaria(Unit prophet, vector<Unit> &units, uint8_t *casted) {
  if(*casted) {
    return ;
  }
  for(size_t i = 0, c = units.size(); i < c; i++) {
    const UnitType type = unit_Type(units[i]);
    if(unit_Distance(prophet, units[i]) <= unit_Range(prophet) && !isMechUnit(units[i]) && eeTypes_IsGroundUnit(type) && unit_CanCast(prophet, ABILITY_PROPHET_FAMINE_)) {
      unit_Point_CastAbility(prophet, unit_Point_Position(units[i]), ABILITY_PROPHET_FAMINE_);
      *casted = 1;
      return ;
    }
  }
}

void prophet_CastTornado(Unit prophet, vector<Unit> &units, uint8_t *casted) {
  if(*casted) {
    return ;
  }
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(unit_Distance(prophet, units[i]) <= unit_Range(prophet) && eeTypes_IsWaterUnit(unit_Type(units[i])) && unit_CanCast(prophet, ABILITY_PROPHET_HURRICANE_)) {
      unit_Point_CastAbility(prophet, unit_Point_Position(units[i]), ABILITY_PROPHET_HURRICANE_);
      *casted = 1;
      return ;
    }
  }
}

void prophet_CastEarthquake(Unit prophet, uint8_t *casted) {
  if(*casted) {
    return ;
  }
  vector<Unit> units = unit_Filter(enemyBuilding);
  for(size_t i = 0, c = units.size(); i < c; i++) {
    if(unit_Distance(prophet, units[i]) <= unit_Range(prophet) && unit_CanCast(prophet, ABILITY_PROPHET_EARTHQUAKE_)) {
      unit_Point_CastAbility(prophet, unit_Point_Position(units[i]), ABILITY_PROPHET_EARTHQUAKE_);
      *casted = 1;
      return ;
    }
  }
}

void prophet_CastAbilities(Unit prophet) {
  vector<Unit> units = unit_Filter(enemyNonFlyingUnit);
  uint8_t casted = 0;
  prophet_CastTornado(prophet, units, &casted);
  prophet_CastMalaria(prophet, units, &casted);
  prophet_CastEarthquake(prophet, &casted);
}

void att_ProcessProphets() {
  size_t total = 5;
  vector<Unit> units = unit_Filter(isProphet);
  uint32_t newIndexes[256];
  pickRandomUnits(units, total, newIndexes, sizeof(newIndexes) / sizeof(newIndexes[0]));
  for(size_t i = 0, c = min(total, units.size()); i < c; i++) {
    prophet_CastAbilities(units[newIndexes[i]]);
  }
}

void hades_CastTimeWarp(Unit hades, vector<Unit> &enemyUnits, uint8_t *casted) {
  if(*casted) {
    return ;
  }
  for(size_t i = 0, c = enemyUnits.size(); i < c; i++) {
    const float unitDistance = unit_Distance(enemyUnits[i], hades);
    if(unitDistance <= unit_Range(hades) && unit_CanCast(hades, ABILITY_MECH_HADES_MP_TIMEWARP_)) {
      unit_Object_CastAbility(hades, enemyUnits[i], ABILITY_MECH_HADES_MP_TIMEWARP_);
      *casted = 1;
      return ;
    }
  }
}

uint8_t isMechUnit(Unit unit) {
  const UnitType type = unit_Type(unit);
  switch (type)
  {
    case MECH_APOLLO:
    case MECH_ARES:
    case MECH_ARES_II:
    case MECH_FURIES:
    case MECH_HYPERION:
    case MECH_HYPERION_II:
    case MECH_MINOTAUR:
    case MECH_MINOTAUR_II:
    case MECH_PANDORA:
    case MECH_PANDORA_II:
    case MECH_POSEIDON:
    case MECH_TEMPEST:
    case MECH_ZEUS:
      return 1;
    
    default:
      break;
  }
  return 0;
}

void hades_CastNanoVirus(Unit hades, vector<Unit> &enemyUnits, uint8_t *casted) {
  if(*casted) {
    return ;
  }
  for(size_t i = 0, c = enemyUnits.size(); i < c; i++) {
    if(!isMechUnit(enemyUnits[i])) {
      continue;
    }
    const float unitDistance = unit_Distance(enemyUnits[i], hades);
    if(unitDistance <= unit_Range(hades) && unit_CanCast(hades, ABILITY_MECH_HADES_MP_NANOVIRUS_)) {
      unit_Object_CastAbility(hades, enemyUnits[i], ABILITY_MECH_HADES_MP_NANOVIRUS_);
      *casted = 1;
      return ;
    }
  }
}

void hades_CastAbilities(Unit hades) {
  vector<Unit> units = unit_Filter(enemyUnitFilter);
  uint8_t casted = 0;
  hades_CastNanoVirus(hades, units, &casted);
  hades_CastTimeWarp(hades, units, &casted);
}

void att_ProcessHades() {
  size_t total = 5;
  vector<Unit> units = unit_Filter(isHades);
  uint32_t newIndexes[256];
  pickRandomUnits(units, total, newIndexes, sizeof(newIndexes) / sizeof(newIndexes[0]));
  for(size_t i = 0, c = min(total, units.size()); i < c; i++) {
    hades_CastAbilities(units[newIndexes[i]]);
  }
}

void tempest_CastAbilities(Unit tempest, vector<Unit> &enemies) {
  if(!unit_CanCast(tempest, ABILITY_MECH_TEMPEST_MP_ANTIMATTERSTORM_)) {
    return ;
  }

  for(size_t i = 0, c = enemies.size(); i < c; i++) {
    if(unit_Range(tempest) * 4.0f <= unit_Distance(enemies[i], tempest)) {
      unit_Point_CastAbility(tempest, unit_Point_Position(enemies[i]), ABILITY_MECH_TEMPEST_MP_ANTIMATTERSTORM_);
      break;
    }
  }
}

void att_ProcessTempest() {
  size_t total = 5;
  vector<Unit> units = unit_Filter(isTempest);
  vector<Unit> enemies = unit_Filter(enemyFlyingUnits);
  uint32_t newIndexes[256];
  pickRandomUnits(units, total, newIndexes, sizeof(newIndexes) / sizeof(newIndexes[0]));
  for(size_t i = 0, c = min(total, units.size()); i < c; i++) {
    tempest_CastAbilities(units[newIndexes[i]], enemies);
  }
}

void att_ProcessSpecialAbilityUnits(PVOID _) {
  att_ProcessPriests();
  att_ProcessProphets();
  att_ProcessHades();
  att_ProcessTempest();
}

uint8_t hurricaneFilter(SimpleUnit unit) {
  return su_Type(unit) == HURRICANE;
}

SimpleUnit getHurricane() {
  vector<SimpleUnit> hurricanes = su_Filter(hurricaneFilter);
  if(!hurricanes.size()) {
    return su_Null();
  }
  return hurricanes[rand() % hurricanes.size()];
}

uint8_t antimatterStormFilter(SimpleUnit unit) {
  return su_Type(unit) == ANTI_MATTER_STORM;
}

SimpleUnit getAntimatterStorm() {
  vector<SimpleUnit> storms = su_Filter(antimatterStormFilter);
  if(!storms.size()) {
    return su_Null();
  }
  return storms[rand() % storms.size()];
}

Unit getClosestEnemyShip(Unit hurricane) {
  vector<Unit> units = unit_GetUnits(eeTa_AllPlayers());
  float closest = 1000000.0f;
  Unit enemy = unit_Null();
  for(size_t i = 0; i < units.size(); i++) {
    UnitType type = unit_Type(units[i]);
    if(eeTypes_IsWaterUnit(type) && !ply_Index_AreAllies(unit_GetPlayerIndex(units[i]), eeTa_SelfPlayer()) && 
       ply_Reference(ply_GetPlayer(units[i])) != ply_Reference(ply_Self()) && 
       ply_Reference(ply_GetPlayer(units[i])) != ply_Reference(ply_Neutral())) {
      float currentDist = unit_Distance(hurricane, units[i]);
      if(closest > currentDist) {
        closest = currentDist;
        enemy = units[i];
      }
    }
  }
  return enemy;
}

void att_HuntWithHurricane(PVOID _) {
  SimpleUnit hurricane = getHurricane();
  if(!su_Reference(hurricane)) {
    return ;
  }
  Unit hurricaneUnit = unit_SimpleUnitToUnit(hurricane);
  Unit enemyShip = getClosestEnemyShip(hurricaneUnit);
  if(!unit_Reference(enemyShip)) {
    return ;
  }
  unit_Action(hurricaneUnit, unit_Point_Position(enemyShip), UNIT_MOVE);
}

Unit getClosestEnemyFlyier(Unit hurricane) {
  vector<Unit> units = unit_GetUnits(eeTa_AllPlayers());
  float closest = 1000000.0f;
  Unit enemy = unit_Null();
  for(size_t i = 0; i < units.size(); i++) {
    UnitType type = unit_Type(units[i]);
    if(eeTypes_IsAirUnit(type) && !ply_Index_AreAllies(unit_GetPlayerIndex(units[i]), eeTa_SelfPlayer()) && 
       ply_Reference(ply_GetPlayer(units[i])) != ply_Reference(ply_Self()) && 
       ply_Reference(ply_GetPlayer(units[i])) != ply_Reference(ply_Neutral())) {
      float currentDist = unit_Distance(hurricane, units[i]);
      if(closest > currentDist) {
        closest = currentDist;
        enemy = units[i];
      }
    }
  }
  return enemy;
}

void att_HuntWithAirplaneStorm(PVOID _) {
  SimpleUnit hurricane = getAntimatterStorm();
  if(!su_Reference(hurricane)) {
    return ;
  }
  Unit hurricaneUnit = unit_SimpleUnitToUnit(hurricane);
  Unit enemyShip = getClosestEnemyFlyier(hurricaneUnit);
  if(!unit_Reference(enemyShip)) {
    return ;
  }
  unit_Action(hurricaneUnit, unit_Point_Position(enemyShip), UNIT_MOVE);
}

void att_PatrolRandomPositions(vector<Unit> &selfUnits) {
  att_PatrolRandomPositions_t(selfUnits, idleAttackingAirUnits, 6);
  att_PatrolRandomPositions_t(selfUnits, idleAttackingGroundUnits, 7);
  att_PatrolRandomPositions_t(selfUnits, idleAttackingWaterUnits, 3);
  if(map_IsSpaceMap()) {
    att_PatrolRandomPositions_t(selfUnits, idleAttackingSpaceUnits, 5);
  }
}

void att_OnFrame() {

}