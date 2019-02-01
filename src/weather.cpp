#include "weather.h"

MatchState weatherRegex;

Weather::Weather() {
  temperature = 0;
  humidity = 0;
  windSpeed = 0;
}

void Weather::update() {
  if (Connection::GetJSON(WEATHER_URL)) {

    char buffer[100];
    unsigned int index = 0;

    weatherRegex.Target(Connection::ConnectionBuffer);
    char result = weatherRegex.Match ("\"temp\":([-%d.]+).+\"humidity\":([%d.]+).+\"speed\":([%d.]+)", index);

    if (result == REGEXP_MATCHED)
    {
      weatherRegex.GetCapture(buffer, 0);
      temperature = atoi(buffer);
      weatherRegex.GetCapture(buffer, 1);
      humidity = atoi(buffer);
      humidity = humidity > 99 ? 99 : humidity;
      weatherRegex.GetCapture(buffer, 2);
      windSpeed = atoi(buffer);
    }
  }
}
