#ifndef __CONFIG__
#define __CONFIG__

#include "credentials.h"

// time
#define TZ_HOURS 2
#define DST_HOURS 1

// internet api
#define WEATHER_URL "http://api.openweathermap.org/data/2.5/weather?lat=50.3873046&lon=30.624428&units=metric&appid=" OPENWEATHERMAP_ID
#define YOUTUBE_URL "https://www.googleapis.com/youtube/v3/channels?part=statistics&id=" YOUTUBE_ID "&key=" YOUTUBE_KEY
const char GOOGLE_API_FINGERPRINT[] PROGMEM = "7C 63 08 E2 83 2B 4C B0 94 E3 34 D3 7E 80 E6 61 5B 5A 26 0B";

#endif
