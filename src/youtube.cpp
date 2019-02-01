#include "youtube.h"

MatchState youtubeRegex;

Youtube::Youtube() : AbstractUpdate(YOUTUBE_UPDATE_PERIOD) {
  subscribers = 0;
  views = 0;
}

bool Youtube::updateData() {
  bool result = Connection::GetJSON(YOUTUBE_URL, GOOGLE_API_FINGERPRINT);
//  bool result = false;

  Serial.println("Request YOUTUBE");

  if (result) {
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
  }

  return result;
}
