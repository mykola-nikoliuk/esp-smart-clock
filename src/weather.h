#ifndef __WEATHER__
#define __WEATHER__

#include <Arduino.h>
#include <Regexp.h>

struct Weather {
  int temperature;
  int humidity;
  int windSpeed;
  Weather();
};

#endif