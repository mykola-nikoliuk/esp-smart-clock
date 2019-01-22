#ifndef __CLOCK_TIME__
#define __CLOCK_TIME__

#include <Arduino.h>
#include "lib/Time/TimeLib.h"
#include "lib/Regexp/Regexp.h"

int hoursToSeconds(int hours);

time_t parseTime(char *str);

void printTime();
char* getShortTime();
char* getSeconds();
char* getShortDate();

#endif
