#pragma once
#include "EETypes.h"
/*
 * Simple units are units that cannot be produce by buildings or workers such as resources, animals, prophet calamities and more.
 */
/*
 * Creates a simple unit with reference NULL.
 */
SimpleUnit su_Null();
/*
 * Returns the reference of the simple unit (a 32 bit number).
 */
PVOID su_Reference(SimpleUnit unit);
/*
 * Gets the simple unit by a filter method. method should return 1 if the unit should be in the array or 0 otherwise.
 */
vector<SimpleUnit> su_Filter(uint8_t (*method)(SimpleUnit));
/*
 * Gets the unit type.
 */
NeutralUnitType su_Type(SimpleUnit unit);