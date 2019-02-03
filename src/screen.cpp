#include "screen.h"

const int pinCS = D8; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
const int numberOfHorizontalDisplays = 4;
const int numberOfVerticalDisplays = 2;

Screen::Screen() : Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays) {
  brightness = new Average(5);
}

void Screen::initScreen() {
  this->fillScreen(0);
  this->setIntensity(1);
  this->write();
}

void Screen::setupScreen() {
  this->setPosition(4, 3, 1);
  this->setPosition(5, 2, 1);
  this->setPosition(6, 1, 1);
  this->setPosition(7, 0, 1);

  this->setRotation(0, 1);
  this->setRotation(1, 1);
  this->setRotation(2, 1);
  this->setRotation(3, 1);
  this->setRotation(4, 3);
  this->setRotation(5, 3);
  this->setRotation(6, 3);
  this->setRotation(7, 3);
}

void Screen::brightnessUpdate(time_t currentTime) {
  if (lastBrightnessUpdate + BRIGHTNESS_UPDATE_PERIOD <= currentTime) {
    lastBrightnessUpdate = currentTime;

    int b = analogRead(A0) / 800. * 14 + 1;
    b = b > 15 ? 15 : b;
    brightness->update(b);
    this->setIntensity(brightness->value());

//    Serial.print(analogRead(A0));
//    Serial.print(" | ");
//    Serial.println(brightness->value());
  }
}
