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
//  char *payload = Connection::getHTTPS(YOUTUBE_URL);
//  char *payload = Connection::get(YOUTUBE_URL);

  char buffer[16];
  unsigned int index = 0;

//  youtubeRegex.Target(payload);
//  char result = youtubeRegex.Match ("viewCount\": \"(%d+)\".+subscriberCount\": \"(%d+)\"", index);
//
//  if (result == REGEXP_MATCHED)
//  {
//    youtubeRegex.GetCapture(buffer, 0);
//    views = atoi(buffer);
//    youtubeRegex.GetCapture(buffer, 1);
//    subscribers = atoi(buffer);
//  }

    views = 5048;
    subscribers = 34;

}
