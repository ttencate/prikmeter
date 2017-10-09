#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <time.h>
#include <WiFiClientSecure.h>

#include "config.h"
#include "root_cert.h"

#define SERIAL_RX_PIN D5
#define P1_BUFFER_SIZE_BYTES 64

#define MAX_TELEGRAM_SIZE 4096

#ifndef SERVER_PORT
#define SERVER_PORT 443
#endif
#ifndef USER_AGENT
#define USER_AGENT "prikmeter"
#endif

#define VERSION "1.0.0"

// Change to Serial for easy debugging, p1 for release.
#define P1_INPUT p1

SoftwareSerial p1(SERIAL_RX_PIN, -1, true, P1_BUFFER_SIZE_BYTES);

byte telegramBuffer[MAX_TELEGRAM_SIZE];

WiFiClientSecure httpsClient;

void readGarbage(bool warn) {
  uint16 count = 0;
  while (P1_INPUT.available()) {
    P1_INPUT.read();
    count++;
  }
  if (warn && count) {
    Serial.print("Found and discarded ");
    Serial.print(count);
    Serial.println(" bytes of garbage after telegram");
  }
}

/**
 * Reads a telegram from the P1 port into the given `buffer`, reading at most
 * `size` bytes. Returns number of bytes read, or 0 on error (a valid telegram
 * is never 0 bytes).
 */
uint16 readTelegram(byte *buffer, uint16 size) {
  uint16 i;
  byte state = 0;
  for (i = 0; i < size; i++) {
    int b = P1_INPUT.read();
    if (b < 0) {
      Serial.println("Read timed out before telegram end sequence");
      return 0;
    }
    buffer[i] = b;

    // Each telegram ends with a line like this:
    // !abcd\r\n
    switch (state) {
      // Preceding line
      case 0:
        if (b == '\r') state++;
        break;
      case 1:
        if (b == '\n') state++; else state = 0;
        break;
      // Exclamation point
      case 2:
        if (b == '!') state++; else state = 0;
        break;
      // 4-byte CRC (hex)
      case 3:
      case 4:
      case 5:
      case 6:
        state++;
        break;
      // End of line
      case 7:
        if (b == '\r') state++; else state = 0;
        break;
      case 8:
        if (b == '\n') state++; else state = 0;
    }

    if (state == 8) {
      readGarbage(true);
      return i;
    }
  }
  Serial.print("Telegram buffer overflow after ");
  Serial.print(size);
  Serial.println(" bytes");
  readGarbage(false);
  return 0;
}

/**
 * Uploads the telegram in the given `buffer` of `size` bytes to the server.
 * Returns `true` on success.
 */
bool uploadTelegram(byte const *buffer, uint16 size) {
  if (!httpsClient.connect(SERVER_HOST, SERVER_PORT)) {
    Serial.print("Failed to connect to " SERVER_HOST ":");
    Serial.print(SERVER_PORT);
    Serial.println();
    return false;
  }

  // TODO figure out why this fails (https://github.com/esp8266/Arduino/issues/3340?)
  // if (!httpsClient.verifyCertChain(SERVER_HOST)) {
  //   Serial.println("Certificate verification failed");
  //   return false;
  // }

  httpsClient.print(
      "POST /telegrams HTTP/1.1\r\n"
      "Host: " SERVER_HOST "\r\n"
      "User-Agent: " USER_AGENT " " VERSION "\r\n"
      "Content-Type: text/plain\r\n"
      "Content-Length: ");
  httpsClient.print(size);
  httpsClient.print("\r\n"
      "Connection: close\r\n"
      "\r\n");
  httpsClient.write(buffer, size);

  return true;
}

void lockUp() {
  while (true) {
    yield();
  }
}

void setup() {
  Serial.begin(115200);

  Serial.println("Opening P1 port");
  p1.begin(115200);
  p1.setTimeout(1000 /* milliseconds */);

  Serial.print("Connecting to wifi [");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("]");
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Synchronizing system clock to NTP server [");
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  while (now < 1000) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("]");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));

  // Serial.println("Loading SSL root CA certificate");
  // if (!httpsClient.setCACert(root_cert, root_cert_len)) {
  //   Serial.println("Failed to set root CA certificate");
  //   lockUp();
  // }

  Serial.println("Up and running");

  Serial.println("Sending test telegram");
  char const *test = "hello world\r\n!abcd\r\n";
  uploadTelegram((byte const *) test, strlen(test));
}

void loop() {
  if (P1_INPUT.available()) {
    uint16 size = readTelegram(telegramBuffer, MAX_TELEGRAM_SIZE);
    if (size > 0) {
      Serial.print("Received telegram of ");
      Serial.print(size);
      Serial.println(" bytes; uploading");
      uploadTelegram(telegramBuffer, size);
    }
  }
}
