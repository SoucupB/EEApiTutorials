#pragma once
#include <windows.h>
#include "EETwa.h"

/*
 * A method which is used to set another method as a callback for when a unit takes damage.
 */
void pls_OnInit(void (*remoteMethod)(Unit));