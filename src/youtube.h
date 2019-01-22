#ifndef __YOUTUBE__
#define __YOUTUBE__

#include <Arduino.h>
#include "lib/Regexp/Regexp.h"
#include "connection.h"
#include "config.h"

struct Youtube {
  Youtube();
  double subscribers;
  double views;
  void update();
};

#endif
