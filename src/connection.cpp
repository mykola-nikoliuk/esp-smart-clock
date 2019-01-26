#include "connection.h"

char connectionBuffer[1024];
const uint8_t fingerprint[20] = {0x95, 0xa7, 0x69, 0x0d, 0x58, 0x4a, 0x35, 0x57, 0xb6, 0x4e, 0xe1, 0x72, 0xe7, 0xc1, 0x01, 0xbd, 0xca, 0xa8, 0xe7, 0x7a};

void Connection::init() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("trying to connect: ");
    Serial.println(WiFi.status());
  }
}

bool Connection::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}

void Connection::getRequest(HTTPClient *http) {
//  Serial.print("[CONNECTION] GET...\n");
  // start connection and send HTTP header
  int httpCode = http->GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
//    Serial.printf("[CONNECTION] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String response = http->getString();
      strcpy(connectionBuffer, response.c_str());
//      Serial.println(connectionBuffer);
    }
  } else {
    Serial.printf("[CONNECTION] GET... failed, error: %s\n", http->errorToString(httpCode).c_str());
  }

  http->end();
}

char* Connection::get(char *url) {
  Serial.print("GET: ");
  Serial.println(url);

  connectionBuffer[0] = '\0';

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, url)) {  // HTTP
      getRequest(&http);
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }

  return connectionBuffer;
}

char* Connection::getHTTPS(char *url) {
  Serial.print("GET: ");
  Serial.println(url);

  connectionBuffer[0] = '\0';

  if (WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);
    HTTPClient http;

    Serial.print("[HTTPS] begin...\n");
    if (http.begin(*client, url)) {  // HTTP
      getRequest(&http);
    } else {
      Serial.printf("[HTTPS} Unable to connect\n");
    }
  }

  return connectionBuffer;
}
