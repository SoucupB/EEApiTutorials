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

/*
 * Gets the current time in miliseconds.
 */
uint64_t tmr_Milliseconds();
/*
 * A method which can be used to add another routine that gets executed periodically. The period is stored in "time" argument of TimeAtom
 * and should be in miliseconds. Example (A method that executes each 5 seconds will have time attribute set to 5000).
 * This method is usefull for routing checks such as checking if the current player has idle workers and sending them to gather or repair stuff
 * or for ordering the army.
 */
void tmrs_AddMethod(PTimerHelper self, TimeAtom atom);