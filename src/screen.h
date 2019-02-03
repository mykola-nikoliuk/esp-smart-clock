#ifndef __LED_SCREEN__
#define __LED_SCREEN__

#include "./lib/Adafruit_GFX/Adafruit_GFX.h"
#include "./lib/Max72xxPanel/Max72xxPanel.h"
#include "./utils/average.h"

#define BRIGHTNESS_UPDATE_PERIOD  50

class Screen : public Max72xxPanel {

private:
  time_t lastBrightnessUpdate = -BRIGHTNESS_UPDATE_PERIOD;
  Average *brightness;

public:
  Screen();
  void initScreen();
  void setupScreen();
  void brightnessUpdate(time_t currentTime);

};

#endif
