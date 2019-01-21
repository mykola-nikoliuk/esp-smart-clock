#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

#include <Wire.h>
#include <HTU21D.h>
#include <String.h>

#include "src/time.h"
#include "src/weather.h"
#include "src/font5x8.h"
#include "src/font3x5.h"

#define TIME_UPDATE_PERIOD    500
#define SLIDER_UPDATE_PERIOD  5000
#define SLIDER_COUNT          4

HTU21D localClimate;


/*
 HTU21D Humidity Sensor Example Code
 By: Nathan Seidle
 SparkFun Electronics
 Date: September 15th, 2013
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 Uses the HTU21D library to display the current humidity and temperature

 Open serial monitor at 9600 baud to see readings. Errors 998 if not sensor is detected. Error 999 if CRC is bad.

 Hardware Connections (Breakoutboard to Arduino):
 -VCC = 3.3V
 -GND = GND
 -SDA = A4 (use inline 10k resistor if your board is 5V)
 -SCL = A5 (use inline 10k resistor if your board is 5V)

 */


int pinCS = 9; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays = 2;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int pinRandom = A0;

int wait = 20; // In milliseconds




time_t unixTime = 0;
time_t lastTimePeriod = -1;
time_t lastTimeUpdate = 0;
time_t lastTimeSync = 0;


time_t lastSliderPeriod = -1;



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
    char *secondsStr = getSeconds();
    Serial.print(timeStr);
    Serial.print(" | ");
    Serial.println(secondsStr);

    matrix.fillRect(0, 0, numberOfHorizontalDisplays * 8, 8, 0);

    if (!(period % 2)) {
      draw5x8(5, 0, ':');
    }
    draw5x8(-3, 0, timeStr[0]);
    draw5x8(3, 0, timeStr[1]);
    draw5x8(11, 0, timeStr[3]);
    draw5x8(17, 0, timeStr[4]);

    matrix.setRotation(3, 0);
    draw3x5(25, 3, secondsStr[0]);
    draw3x5(29, 3, secondsStr[1]);
    matrix.setRotation(3, 1);

    matrix.write();

    delete timeStr;
    delete secondsStr;
  }
}

void dateUpdate() {
    char *dateStr = getShortDate();
    draw3x5String(2, 9, dateStr);
    Serial.println(dateStr);
    delete dateStr;
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
  Weather *climate = new Weather();
  char buffer[5];

  sprintf(buffer, "%d*", climate->temperature);
  draw3x5String(0, 9, buffer);
  sprintf(buffer, "%d%%", climate->humidity);
  draw3x5String(13, 9, buffer);
  sprintf(buffer, "%d", climate->windSpeed);
  draw3x5String(25, 9, buffer);

  Serial.println("EXTERNAL:");
  Serial.print("Temperature:");
  Serial.print(climate->temperature);
  Serial.print("C");
  Serial.print(" | Humidity:");
  Serial.print(climate->humidity);
  Serial.print("%");
  Serial.print(" | Wind speed:");
  Serial.print(climate->windSpeed);
  Serial.println("km/h");
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
        dateUpdate();
        break;
      case 1:
        localClimateUpdate();
        break;
      case 2:
        externalClimateUpdate();
        break;
      case 3:
        draw3x5String(6, 9, "-------");
        break;
    }

    matrix.write();
  }
}

void setup() {
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

  randomSeed(analogRead(pinRandom)); // Initialize random generator

  Serial.begin (115200);
  localClimate.begin();


  char *str = "{\"week_number\":\"04\",\"utc_offset\":\"+02:00\",\"unixtime\":\"1548094898\",\"timezone\":\"Europe/Kiev\",\"dst_until\":null,\"dst_from\":null,\"dst\":false,\"day_of_year\":21,\"day_of_week\":1,\"datetime\":\"2019-01-21T20:21:38.975520+02:00\",\"abbreviation\":\"EET\"}";
  unixTime = parseTime(str);
  setTime(unixTime);
}

void loop() {

//  matrix.drawPixel(x, y, HIGH);
//  matrix.write(); // Send bitmap to display

//  delay(wait);

//  matrix.drawPixel(x, y, LOW); // Erase the old position of our dot

//  do {
//    switch ( random(4) ) {
//      case 0: xNext = constrain(x + 1, 0, matrix.width() - 1); yNext = y; break;
//      case 1: xNext = constrain(x - 1, 0, matrix.width() - 1); yNext = y; break;
//      case 2: yNext = constrain(y + 1, 0, matrix.height() - 1); xNext = x; break;
//      case 3: yNext = constrain(y - 1, 0, matrix.height() - 1); xNext = x; break;
//    }
//  }
//  while ( x == xNext && y == yNext ); // Repeat until we find a new coordinate

//  matrix.drawChar(0, 0, 'A', HIGH, LOW, 1);

//  x = xNext;
//  y = yNext;


//  matrix.fillScreen(LOW);

//  matrix.drawChar(0, 0, 'A', HIGH, LOW, 1);



  time_t currentTime = millis();


  timeUpdate(currentTime);
  sliderUpdate(currentTime);
}