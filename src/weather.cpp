#include "weather.h"

MatchState weatherRegex;

Weather::Weather() {
//  char str[] = "<current><city id=\"696377\" name=\"Poznyaki\"><coord lon=\"30.63\" lat=\"50.39\"/><country>UA</country><sun rise=\"2019-01-21T05:46:06\" set=\"2019-01-21T14:32:01\"/></city><temperature value=\"-3\" min=\"-3\" max=\"-3\" unit=\"metric\"/><humidity value=\"73\" unit=\"%\"/><pressure value=\"1018\" unit=\"hPa\"/><wind><speed value=\"5\" name=\"Gentle Breeze\"/><gusts/><direction value=\"330\" code=\"NNW\" name=\"North-northeast\"/></wind><clouds value=\"75\" name=\"broken clouds\"/><visibility value=\"10000\"/><precipitation mode=\"no\"/><weather number=\"803\" value=\"broken clouds\" icon=\"04d\"/><lastupdate value=\"2019-01-21T11:00:00\"/></current>";
  char buffer[100];
  unsigned int index = 0;
//<temperature value=\"

//  weatherRegex.Target(str);
  char result = weatherRegex.Match ("(%d+)\"", index);

  if (result == REGEXP_MATCHED)
  {

    Serial.print("buffer: ");
    Serial.println(buffer);
    weatherRegex.GetCapture(buffer, 0);
    temperature = atoi(buffer);
//    ms.GetCapture(buffer, 1);
//    humidity = atoi(buffer);
//    ms.GetCapture(buffer, 2);
//    windSpeed = atoi(buffer);
  }

  temperature = -10;
  humidity = 75;
  windSpeed = 15;
}