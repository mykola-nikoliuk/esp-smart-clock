#include <SPI.h>
#include <Wire.h>
#include <String.h>

#include "src/lib/Adafruit_GFX/Adafruit_GFX.h"
#include "src/lib/Max72xxPanel/Max72xxPanel.h"
#include "src/lib/HTU21D/HTU21D.h"

#include "src/connection.h"
#include "src/time.h"
#include "src/weather.h"
#include "src/youtube.h"
#include "src/font5x8.h"
#include "src/font3x5.h"

#define TIME_UPDATE_PERIOD        500
#define TIME_SYNC_PERIOD          5 * 60
#define BRIGHTNESS_UPDATE_PERIOD  50
#define SLIDER_UPDATE_PERIOD      5000
#define SLIDER_COUNT              4

HTU21D localClimate;

int pinCS = D8; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays = 2;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int tonePin = 12;


time_t unixTime = 0;
time_t lastTimePeriod = -1;
time_t lastTimeUpdate = 0;
time_t lastTimeSync = -TIME_SYNC_PERIOD;
time_t lastSliderPeriod = -1;

time_t lastBrightnessUpdate = -BRIGHTNESS_UPDATE_PERIOD;

Weather *weather;
Youtube *youtube;


int x = numberOfHorizontalDisplays * 8 / 2;
int y = numberOfVerticalDisplays * 8 / 2;
int xNext, yNext;

void draw5x8(int x, int y, int index) {
  matrix.drawBitmap(x, y, font5x8(index), 8, 8, HIGH);
}

int draw3x5(int x, int y, int index) {
  matrix.drawBitmap(x - 4, y, font3x5(index), 8, 5, HIGH);
  return font3x5SymbolWidth(index);
}

void draw3x5String(int x, int y, char *str) {
  int offset = x;
  for (int i = 0; i < strlen(str); i++) {
    offset += draw3x5(offset, y, str[i]) + 1;
  }
}

void timeUpdate(time_t currentTime) {
  time_t period = currentTime / TIME_UPDATE_PERIOD;

  if (lastTimePeriod != period) {
    lastTimePeriod = period;
    time_t seconds = currentTime / 1000;

    // update time id needed
    if (lastTimeUpdate < seconds) {
      time_t delta = seconds - lastTimeUpdate;
      lastTimeUpdate = seconds;
      unixTime += delta;
      setTime(unixTime);
    }

    char *timeStr = getShortTime();
    Serial.print(timeStr);
    Serial.print(" | ");

    matrix.fillRect(0, 0, numberOfHorizontalDisplays * 8, 8, 0);

    if (!(period % 2)) {
      draw5x8(5, 0, ':');
    }
    draw5x8(-3, 0, timeStr[0]);
    draw5x8(3, 0, timeStr[1]);
    draw5x8(11, 0, timeStr[3]);
    draw5x8(17, 0, timeStr[4]);

    char *secondsStr = getSeconds();
    Serial.println(secondsStr);
    matrix.setRotation(3, 0);
    draw3x5(25, 3, secondsStr[0]);
    draw3x5(29, 3, secondsStr[1]);
    matrix.setRotation(3, 1);

    matrix.write();

    Serial.print(lastTimeSync);
    Serial.print(" | ");
    Serial.println(unixTime);
    Serial.print("sensor: ");
    Serial.println(analogRead(A0));
    Serial.print("connection: ");
    Serial.println(WiFi.status());
  }
}

void dateUpdate() {
    char *dateStr = getShortDate();
    draw3x5String(2, 9, dateStr);
    Serial.println(dateStr);
}

void localClimateUpdate() {
  int humd = localClimate.readHumidity();
  int temp = localClimate.readTemperature();
  char buffer[5];

  temp = temp > 99 ? 99 : temp;
  humd = humd > 99 ? 99 : humd;

  sprintf(buffer, "%d*", temp);
  draw3x5String(5, 9, buffer);
  sprintf(buffer, "%d%%", humd);
  draw3x5String(18, 9, buffer);

  Serial.println("LOCAL:");
  Serial.print("Temperature:");
  Serial.print(temp);
  Serial.print("*");
  Serial.print(" | Humidity:");
  Serial.print(humd);
  Serial.println("%");
}

void externalClimateUpdate() {
  char buffer[5];

  weather->update();

  sprintf(buffer, "%d*", weather->temperature);
  draw3x5String(0, 9, buffer);
  sprintf(buffer, "%d%%", weather->humidity);
  draw3x5String(13, 9, buffer);
  sprintf(buffer, "%d", weather->windSpeed);
  draw3x5String(25, 9, buffer);

  Serial.println("EXTERNAL:");
  Serial.print("Temperature:");
  Serial.print(weather->temperature);
  Serial.print("C");
  Serial.print(" | Humidity:");
  Serial.print(weather->humidity);
  Serial.print("%");
  Serial.print(" | Wind speed:");
  Serial.print(weather->windSpeed);
  Serial.println("km/h");
}

void youtubeUpdate() {
  char buffer[16];

  youtube->update();

  sprintf(buffer, "%d*%d", youtube->subscribers, youtube->views);
  draw3x5String(0, 9, buffer);
}

void sliderUpdate(time_t currentTime) {
  time_t period = currentTime / SLIDER_UPDATE_PERIOD;

  if (lastSliderPeriod != period) {
    lastSliderPeriod = period;
    matrix.fillRect(0, 8, numberOfHorizontalDisplays * 8, 8, 0);

    int lineLength = numberOfHorizontalDisplays * 8 / SLIDER_COUNT;
    int index = period % SLIDER_COUNT;
    matrix.drawLine(index * lineLength, 15, index * lineLength + lineLength - 1, 15, 1);

    switch (index) {
      case 0:
        localClimateUpdate();
        break;
      case 1:
        externalClimateUpdate();
        break;
      case 2:
//        youtubeUpdate();
        break;
      case 3:
        dateUpdate();
        break;
    }

    matrix.write();
  }
}

void syncTime() {
  if ((lastTimeSync + TIME_SYNC_PERIOD) < unixTime) {
    if (Connection::isConnected()) {
      Serial.println("Sync time");
      char *payload = Connection::get("http://worldtimeapi.org/api/ip");
      unixTime = parseTime(payload);
      setTime(unixTime);
      lastTimeUpdate = millis() / 1000;
      lastTimeSync = unixTime;
    }
  }
}

void brightness(time_t currentTime) {
  if (lastBrightnessUpdate + BRIGHTNESS_UPDATE_PERIOD < currentTime) {
    lastBrightnessUpdate = currentTime;
    int brightness = analogRead(A0) / 800. * 14 + 1;
    brightness = brightness > 15 ? 15 : brightness;
    matrix.setIntensity(brightness);
  }
}

void setup() {
//  pinMode(tonePin, OUTPUT);

//  tone(tonePin, 262, 2000);

  matrix.setIntensity(0); // Set brightness between 0 and 15

// Adjust to your own needs
  matrix.setPosition(4, 3, 1);
  matrix.setPosition(5, 2, 1);
  matrix.setPosition(6, 1, 1);
  matrix.setPosition(7, 0, 1);

  matrix.setRotation(0, 1);    // The first display is position upside down
  matrix.setRotation(1, 1);    // The same hold for the last display
  matrix.setRotation(2, 1);    // The same hold for the last display
  matrix.setRotation(3, 1);    // The same hold for the last display
  matrix.setRotation(4, 3);    // The first display is position upside down
  matrix.setRotation(5, 3);    // The same hold for the last display
  matrix.setRotation(6, 3);    // The same hold for the last display
  matrix.setRotation(7, 3);    // The same hold for the last display

  Serial.begin (115200);
  localClimate.begin();
  weather = new Weather();
}

void loop() {
  time_t currentTime = millis();
  brightness(currentTime);
  syncTime();
  timeUpdate(currentTime);
  sliderUpdate(currentTime);
}
