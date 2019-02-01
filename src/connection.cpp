#include "connection.h"

MatchState connectionRegex;

Header::Header() : contentLength(0) {}

void Connection::Init() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
//    Serial.print("trying to connect: ");
//    Serial.println(WiFi.status());
  }
}

bool Connection::IsConnected() {
  return WiFi.status() == WL_CONNECTED;
}

void Connection::ReadHeader(Client *client, Header *header) {
  char lineBuffer[48];
  char valueBuffer[128];

  while (true) {
    String line = client->readStringUntil('\n');

    // Content-Length
    if (!header->contentLength) {
      line.toCharArray(lineBuffer, 48);
      connectionRegex.Target(lineBuffer);
      if (connectionRegex.Match("Content%-Length: (%d+)", 0) == REGEXP_MATCHED) {
         connectionRegex.GetCapture(valueBuffer, 0);
         header->contentLength = atoi(valueBuffer);
      }
    }

    if (line == "\r" || line == "\r\n") {
      Serial.println("headers received");
      break;
    }
  }
}

void Connection::ReadBody(Client *client, Header *header) {
  String line;
  bool braceInit = false;
  int braceCount = 0;
  int offset = 0;

  client->readBytes(Connection::ConnectionBuffer, header->contentLength);
  Connection::ConnectionBuffer[header->contentLength] = '\0';
}

bool Connection::GetRequest(Client *client, char *url, uint32_t port) {

  unsigned int index = 0;
  bool result = false;
  char host[32];
  char params[128];

  connectionRegex.Target(url);
  byte regexResult = connectionRegex.Match("^https?://([^/]+)(.*)", index);

  if (regexResult == REGEXP_MATCHED) {
    connectionRegex.GetCapture(host, 0);
    connectionRegex.GetCapture(params, 1);

//    Serial.printf("HOST: \"%s:%d\"\n", host, port);
//    Serial.printf("PARAMS: \"%s\"\n", params);
  }

  int connected = client->connect(host, port);
  Serial.printf("connected: %d\n", connected);


  if (WiFi.status() == WL_CONNECTED && connected) {

    client->print(String("GET ") + params + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "User-Agent: Smart-Clock test v0.2\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.println("request sent");

    Header header;
    Connection::ReadHeader(client, &header);
    Connection::ReadBody(client, &header);

//    Serial.println(Connection::ConnectionBuffer);

    result = true;
  }

  return result;
}

bool Connection::GetJSON(char *url) {
  WiFiClient client;
  return Connection::GetRequest(&client, url, 80);
}

bool Connection::GetJSON(char *url, const char *fingerPrint) {
  WiFiClientSecure client;
  client.setFingerprint(GOOGLE_API_FINGERPRINT);
  return Connection::GetRequest(&client, url, 443);
}

char Connection::ConnectionBuffer[CONNECTION_BUFFER_SIZE] = {};
