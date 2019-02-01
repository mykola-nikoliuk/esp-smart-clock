#ifndef __CONNECTION__
#define __CONNECTION__

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "lib/Regexp/Regexp.h"
#include "config.h"

#define CONNECTION_BUFFER_SIZE 1024

#define NTP_SERVER = "time.nist.gov";

struct Header {
  Header();
  uint32_t contentLength;
};

class Connection {
  static void ReadHeader(Client *client, Header *header);
  static void ReadBody(Client *client, Header *header);
  static bool GetRequest(Client *client, char *url, uint32_t port);
  public:
  static char ConnectionBuffer[CONNECTION_BUFFER_SIZE];
  static void Init();
  static bool IsConnected();
  static char* GetBufferPointer();
  static bool GetJSON(char *url);
  static bool GetJSON(char *url, const char *fingerPrint);
};

#endif
