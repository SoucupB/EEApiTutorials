#pragma once
#include <windows.h>
#include <stdint.h>

typedef struct Resource_t {
  PVOID _payload;
} Resource;

typedef struct Unit_t {
  PVOID _payload;
} Unit;

typedef struct SimpleUnit_t {
  PVOID _payload;
} SimpleUnit;

typedef struct Player_t {
  PVOID _payload;
} Player;

typedef struct UnitStatistics_t {
  int32_t currentHP;
  int32_t totalHP;
  float movementSpeed;
  float attackDamage;
} UnitStatistics;

typedef struct TechTree_t {
  PVOID _payload;
} TechTree;