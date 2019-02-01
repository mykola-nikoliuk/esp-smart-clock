#include <SPI.h>
#include <Wire.h>
#include <String.h>

#include "src/lib/HTU21D/HTU21D.h"
#include "src/connection.h"
#include "src/screen.h"
#include "src/time.h"
#include "src/weather.h"
#include "src/youtube.h"
#include "src/font5x8.h"
#include "src/font3x5.h"

#define TIME_UPDATE_PERIOD        500
#define SLIDER_UPDATE_PERIOD      3000
#define SLIDER_COUNT              4

HTU21D localClimate;
Screen matrix;

time_t lastTimePeriod = -1;
time_t lastSliderPeriod = -1;

Weather *weather;
Youtube *youtube;

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

    if (period % 2 == 0) {
      setTime(time(nullptr));

//      printTime();
    }

    char *timeStr = getShortTime();

    matrix.fillRect(0, 0, matrix.width(), 8, 0);

    if (!(period % 2)) {
      draw5x8(5, 0, ':');
    }
    draw5x8(-3, 0, timeStr[0]);
    draw5x8(3, 0, timeStr[1]);
    draw5x8(11, 0, timeStr[3]);
    draw5x8(17, 0, timeStr[4]);

    char *secondsStr = getSeconds();
//    Serial.println(secondsStr);
    matrix.setRotation(3, 0);
    draw3x5(25, 3, secondsStr[0]);
    draw3x5(29, 3, secondsStr[1]);
    matrix.setRotation(3, 1);

    matrix.write();
  }
}

void dateUpdate() {
    char *dateStr = getShortDate();
    draw3x5String(2, 9, dateStr);
//    Serial.println(dateStr);
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

//  Serial.println("LOCAL:");
//  Serial.print("Temperature:");
//  Serial.print(temp);
//  Serial.print("*");
//  Serial.print(" | Humidity:");
//  Serial.print(humd);
//  Serial.println("%");
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

//  Serial.println("EXTERNAL:");
//  Serial.print("Temperature:");
//  Serial.print(weather->temperature);
//  Serial.print("C");
//  Serial.print(" | Humidity:");
//  Serial.print(weather->humidity);
//  Serial.print("%");
//  Serial.print(" | Wind speed:");
//  Serial.print(weather->windSpeed);
//  Serial.println("km/h");
}

void youtubeUpdate() {
  char buffer[16];

  youtube->update();

  sprintf(buffer, "%d>%d", youtube->subscribers, youtube->views);
  draw3x5String(0, 9, buffer);
}

void sliderUpdate(time_t currentTime) {
  time_t period = currentTime / SLIDER_UPDATE_PERIOD;

  if (lastSliderPeriod != period) {
    lastSliderPeriod = period;
    matrix.fillRect(0, 8, matrix.width(), 8, 0);

    int lineLength = matrix.width() / SLIDER_COUNT;
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
        youtubeUpdate();
        break;
      case 3:
        dateUpdate();
        break;
    }

    matrix.write();
  }
}

void setup() {
  Serial.begin(115200);

  matrix.initScreen();
  Connection::Init();
  matrix.setupScreen();

  configTime(TZ_HOURS * 3600, DST_HOURS * 3600, "time.google.com", "time.windows.com", "time.nist.gov");

  localClimate.begin();
  weather = new Weather();
  youtube = new Youtube();
}

void loop() {
  time_t currentTime = millis();
  matrix.brightnessUpdate(currentTime);
  timeUpdate(currentTime);
  sliderUpdate(currentTime);
}
