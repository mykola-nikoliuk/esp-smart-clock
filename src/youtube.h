#ifndef __YOUTUBE__
#define __YOUTUBE__

#include <Arduino.h>
#include "lib/Regexp/Regexp.h"
#include "connection.h"
#include "config.h"

struct Youtube {
  Youtube();
  uint32_t subscribers;
  uint32_t views;
  void update();
};

#endif
