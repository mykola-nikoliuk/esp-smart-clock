#ifndef __WEATHER__
#define __WEATHER__

#include <Arduino.h>
#include "lib/Regexp/Regexp.h"
#include "abstractUpdate.h"
#include "connection.h"
#include "config.h"

class Weather : public AbstractUpdate {

  bool updateData();

public:

  Weather();
  int temperature;
  int humidity;
  int windSpeed;

};

#endif
