#pragma once
#include "EETypes.h"

SimpleUnit su_Null();
PVOID su_Reference(SimpleUnit unit);
vector<SimpleUnit> su_Filter(uint8_t (*method)(SimpleUnit));
NeutralUnitType su_Type(SimpleUnit unit);