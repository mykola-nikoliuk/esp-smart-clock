#include "abstractUpdate.h"

AbstractUpdate::AbstractUpdate(time_t period) : _period(period), _init(false), _lastUpdate(0) {}

bool AbstractUpdate::update(time_t currentTime) {
  bool updated = false;

  if (_lastUpdate + _period < currentTime || !_init) {
    updated = this->updateData();
    if (updated) {
      _init = true;
      _lastUpdate = currentTime;
    }
  }

  return updated;
};
