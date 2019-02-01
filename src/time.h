#ifndef __CLOCK_TIME__
#define __CLOCK_TIME__

#include <Arduino.h>
#include <time.h>
#include "lib/Time/TimeLib.h"
#include "lib/Regexp/Regexp.h"

void printTime();
char* getShortTime();
char* getSeconds();
char* getShortDate();

#endif
