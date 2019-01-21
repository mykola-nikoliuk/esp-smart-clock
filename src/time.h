#ifndef __CLOCK_TIME__
#define __CLOCK_TIME__

#include <Arduino.h>
#include <TimeLib.h>
#include <Regexp.h>

int hoursToSeconds(int hours);

time_t parseTime(char *str);

void printTime();

char* getShortTime();

char* getSeconds();

char* getShortDate();

#endif