#ifndef __AVERAGE__
#define __AVERAGE__

#include "arduino.h"


class Average {

private:
  int length;
  int index;
  int history[10];

public:
  Average(int size);
  void update(int value);
  int value();

};

#endif
