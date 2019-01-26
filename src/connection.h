#ifndef __CONNECTION__
#define __CONNECTION__

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "config.h"

class Connection {
  static void getRequest(HTTPClient *http);
  public:
  static void init();
  static bool isConnected();
  static char* get(char *url);
  static char* getHTTPS(char *url);
};

#endif
