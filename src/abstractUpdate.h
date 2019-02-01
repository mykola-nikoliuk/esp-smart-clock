#ifndef __ABSTRACT_UPDATE__
#define __ABSTRACT_UPDATE__

#include "arduino.h"

class AbstractUpdate {

  bool _init;
  time_t _period;
  time_t _lastUpdate;
  virtual bool updateData() = 0;

public:

  AbstractUpdate(time_t period);
  bool update(time_t currentTime);

};

#endif
