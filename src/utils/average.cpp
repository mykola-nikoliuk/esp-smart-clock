#include "average.h"

Average::Average(int size) {
  length = size;
  index = 0;
  for (int i = 0; i < length; i++) {
    history[i] = 0;
  }
}

void Average::update(int value) {
  history[index] = value;
  index = ++index % length;
}

int Average::value() {
  int result = history[0];
  for (int i = 1; i < length; i++) {
    result += history[i];
  }
  return result / length;
}
