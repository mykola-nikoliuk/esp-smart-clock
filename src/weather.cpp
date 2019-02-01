#include "weather.h"

MatchState weatherRegex;

Weather::Weather() : AbstractUpdate(WEATHER_UPDATE_PERIOD) {
  temperature = 0;
  humidity = 0;
  windSpeed = 0;
}

bool Weather::updateData() {
  bool result = Connection::GetJSON(WEATHER_URL);

  Serial.println("Request WEATHER");

  if (result) {

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

  return result;
}
