#include "time.h"

MatchState timeRegex;

int hoursToSeconds(int hours) {
  return hours * 3600;
}

time_t parseTime(char *str) {
  // http://worldtimeapi.org/api/ip
  int timeOffset = 2;
  time_t unixTime = 1548007590 + hoursToSeconds(timeOffset);
  unsigned int index = 0;
  char buffer[100];

  timeRegex.Target(str);

  char result = timeRegex.Match ("\"utc_offset\":\"%+(%d+):00\".+\"unixtime\":\"(%d+)\"", index);

  if (result == REGEXP_MATCHED)
  {
    timeRegex.GetCapture(buffer, 0);
    timeOffset = atoi(buffer);
    timeRegex.GetCapture(buffer, 1);
    unixTime = atol(buffer);
  }

  return unixTime + hoursToSeconds(timeOffset);
}

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
  char *buffer = new char[6];
  int h = hour();
  int m = minute();
//  int s = second();

  int hourTens = h / 10;
  int minuteTens = m / 10;

  sprintf(
    buffer,
    "%d%d:%d%d",
    hourTens,
    h % 10,
    minuteTens,
    m % 10
  );

  return buffer;
}

char* getSeconds() {
  char *buffer = new char[3];
  int s = second();
  sprintf(buffer, "%d%d", s / 10, s % 10);
  return buffer;
}

char* getShortDate() {
  char *buffer = new char[9];
  int y = year() % 100;
  int m = month();
  int d = day();

  int yearTens = y / 10;
  int monthTens = m / 10;
  int dayTens = d / 10;

  sprintf(
    buffer,
    "%d%d.%d%d.%d%d",
    dayTens,
    d % 10,
    monthTens,
    m % 10,
    yearTens,
    y % 10
  );

  return buffer;
}