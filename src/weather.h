#ifndef __WEATHER__
#define __WEATHER__

#include <Arduino.h>
#include "lib/Regexp/Regexp.h"
#include "connection.h"
#include "config.h"

struct Weather {
  Weather();
  int temperature;
  int humidity;
  int windSpeed;
  void update();
};

#endif
