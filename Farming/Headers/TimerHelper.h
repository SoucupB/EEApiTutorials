#pragma once
#include <vector>
#include <Windows.h>
#include <stdint.h>

typedef struct TimeAtom_t {
  PVOID method;
  size_t time;
  PVOID arguments; // Arguments should be alocated on the heap or static memory and the calee should take care of freeing the object.
  size_t _lastCheckedTime;
} TimeAtom;

typedef struct Timerdriver_t *PTimerHelper;

uint64_t tmr_Milliseconds();
void tmrs_AddMethod(PTimerHelper self, TimeAtom atom);