#include "time.h"

MatchState timeRegex;
char timeBuffer[10];

void printTime() {
  Serial.print(year());
  Serial.print("/");
  Serial.print(month());
  Serial.print("/");
  Serial.print(day());
  Serial.println("");

  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.println(second());
  Serial.println("");
}

char* getShortTime() {
  int h = hour();
  int m = minute();
//  int s = second();

  int hourTens = h / 10;
  int minuteTens = m / 10;

  sprintf(
    timeBuffer,
    "%d%d:%d%d",
    hourTens,
    h % 10,
    minuteTens,
    m % 10
  );

  return timeBuffer;
}

char* getSeconds() {
  int s = second();
  sprintf(timeBuffer, "%d%d", s / 10, s % 10);
  return timeBuffer;
}

char* getShortDate() {
  int y = year() % 100;
  int m = month();
  int d = day();

  int yearTens = y / 10;
  int monthTens = m / 10;
  int dayTens = d / 10;

  sprintf(
    timeBuffer,
    "%d%d.%d%d.%d%d",
    dayTens,
    d % 10,
    monthTens,
    m % 10,
    yearTens,
    y % 10
  );

  return timeBuffer;
}
