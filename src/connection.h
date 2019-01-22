#ifndef __CONNECTION__
#define __CONNECTION__

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "config.h"

class Connection {
  Connection();
  static Connection *instance;
  static Connection* GetInstance();
  static void getRequest(HTTPClient *http);
  public:
  static int isConnected();
  static char* get(char *url);
  static char* getHTTPS(char *url);
};

#endif
