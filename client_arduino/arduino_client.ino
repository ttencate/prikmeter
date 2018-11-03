#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <time.h>
#include <WiFiClientSecure.h>

#include "config.h"
#include "root_cert.h"
#include "telegram_reader.h"

#define SERIAL_RX_PIN D5
#define LED_PIN D4
#define P1_BUFFER_SIZE_BYTES 64

#define READ_TIMEOUT_MILLIS 5000

#ifndef SERVER_PORT
#define SERVER_PORT 443
#endif
#ifndef USER_AGENT
#define USER_AGENT "prikmeter"
#endif

#define VERSION "1.0.0"

// Change to Serial for easy debugging, p1 for release.
#define P1_INPUT Serial
// #define P1_INPUT p1

SoftwareSerial p1(SERIAL_RX_PIN, -1, true, P1_BUFFER_SIZE_BYTES);
TelegramReader telegramReader;
WiFiClientSecure httpsClient;

void setLed(bool on) {
  digitalWrite(LED_PIN, on ? LOW : HIGH);
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

  setLed(true);

  if (!httpsClient.verify(SERVER_CERTIFICATE_FINGERPRINT, SERVER_HOST)) {
    Serial.println("Certificate verification failed");
    return false;
  }

  httpsClient.print(
      "POST /telegrams HTTP/1.1\r\n"
      "Host: " SERVER_HOST "\r\n"
      "User-Agent: " USER_AGENT " " VERSION "\r\n"
      "Content-Type: text/plain\r\n"
      "Content-Length: ");
  httpsClient.print(size);
  httpsClient.print("\r\n"
      "X-Auth-Token: " AUTH_TOKEN "\r\n"
      "Connection: close\r\n"
      "\r\n");
  httpsClient.write(buffer, size);

  Serial.print("Uploaded telegram of ");
  Serial.print(size);
  Serial.println(" bytes");

  return true;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  setLed(true);

  Serial.begin(115200);

  Serial.println("Opening P1 port");
  p1.begin(115200);

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
  now = time(nullptr);
  Serial.print("Current time: ");
  Serial.print(ctime(&now));
  Serial.println();

  // Serial.println("Loading SSL root CA certificate");
  // if (!httpsClient.setCACert(root_cert, root_cert_len)) {
  //   Serial.println("Failed to set root CA certificate");
  // }

  Serial.println("Up and running");

  // Serial.println("Sending test telegram");
  // char const *testTelegram = "/hello\r\n!world\r\n";
  // uploadTelegram((byte const *) testTelegram, strlen(testTelegram));

  setLed(false);
}

void loop() {
  // If we still don't have a complete telegram seconds after the start, assume
  // read error and reset the reader for the next one.
  static unsigned long telegramStartTime = millis();
  if (!telegramReader.isEmpty() && millis() - telegramStartTime > READ_TIMEOUT_MILLIS) {
    Serial.print("Telegram still not completed after ");
    Serial.print(READ_TIMEOUT_MILLIS);
    Serial.println(" ms");
    telegramReader.reset();
    setLed(false);
  }

  int b = P1_INPUT.read();
  if (b >= 0) {
    if (telegramReader.isEmpty()) {
      telegramStartTime = millis();
      setLed(true);
    }

    telegramReader.addByte((byte) b);

    if (telegramReader.isComplete()) {
      setLed(false);

      unsigned int size = telegramReader.getSize();
      Serial.print("Received telegram of ");
      Serial.print(size);
      Serial.println(" bytes");

      bool success = uploadTelegram(telegramReader.getBuffer(), size);
      if (success) {
        setLed(false);
      }

      telegramReader.reset();
    }
  }
}
