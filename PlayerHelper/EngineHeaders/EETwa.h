#pragma once
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include "Geometry.h"
#include "TimerHelper.h"
#include "EETypes.h"

enum Cheat {
  CHEAT_COST_REDUCTION = (1<<0)
};

/*
 * Returns the current frame from the start of the game
 */
int64_t eeTa_CurrentFrame();

/*
 * Returns the player index from the unit parameter.
 */
int8_t eeTa_Player(Unit unit);
/*
 * Returns the current player index
 */
int8_t eeTa_SelfPlayer();
/*
 * Will return a number which can be used for any filter in order to ignore the player id requirement.
 */
int8_t eeTa_AllPlayers();
/*
 * Set the computer vs computer aggression, bot index is the index of the player and aggression is a value between 0 an 100 (default is 10).
 */
void eeTa_SetCvCAggression(uint8_t botIndex, float aggression);
/*
 * Adds a method that can loop run after a set amount of MS. 
 */
void eeTa_AddFrameMethod(TimeAtom atom);
/*
 * Neutral player which has the resources and neutral units such as farm animals.
 */
uint8_t eeTa_NeutralPlayer();
/*
 * Gets the current player count.
 */
int8_t eeTa_PlayerCount();
/*
 * Method to print to a file data, works similar to printf.
 */
void eeTa_FilePrintf(const char *format, ...);
/*
 * Add a cheat for the self bot.
 */
void eeTa_AddCheat(Cheat cheat);
/*
 * Method to print to the game chat
 */
void eeTa_PrintfLine(const char *format, ...);