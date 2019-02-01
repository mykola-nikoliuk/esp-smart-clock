#ifndef __YOUTUBE__
#define __YOUTUBE__

#include <Arduino.h>
#include "lib/Regexp/Regexp.h"
#include "abstractUpdate.h"
#include "connection.h"
#include "config.h"

class Youtube : public AbstractUpdate {

  bool updateData();

public:

  Youtube();
  uint32_t subscribers;
  uint32_t views;

};

#endif
