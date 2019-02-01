#include "youtube.h"

MatchState youtubeRegex;

Youtube::Youtube() {
  subscribers = 0;
  views = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void Youtube::update() {
  if (Connection::GetJSON(YOUTUBE_URL, GOOGLE_API_FINGERPRINT)) {

    char buffer[16];
    unsigned int index = 0;

    youtubeRegex.Target(Connection::ConnectionBuffer);
    char result = youtubeRegex.Match("viewCount\": \"(%d+)\".+subscriberCount\": \"(%d+)\"", index);

    if (result == REGEXP_MATCHED)
    {
      youtubeRegex.GetCapture(buffer, 0);
      views = atoi(buffer);
      youtubeRegex.GetCapture(buffer, 1);
      subscribers = atoi(buffer);
    }

//    Serial.printf("s: %d | v: %d", subscribers, views);
  }
}
