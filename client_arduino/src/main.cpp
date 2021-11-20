#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <InverterReader.h>
#include <LittleFS.h>
#include <SMAReader.h>
#include <SoftwareSerial.h>
#include <time.h>
#include <WiFiClientSecure.h>

#include "Config.h"
#include "errors.h"
#include "InverterReader.h"
#include "Led.h"
#include "TelegramReader.h"

#define P1_PIN D5
#define P1_BAUD 115200
#define P1_CONFIG SWSERIAL_8N1
#define P1_INVERT true
// Receive buffer size for P1 port. We'd like to set this higher, but even 1024
// (which would fit a typical telegram in its entirety) results in OOMs at
// startup.
#define P1_BUFFER_SIZE_BYTES 128

#define READ_TIMEOUT_MILLIS 5000

#define MIN_TELEGRAM_INTERVAL_MILLIS 9500

#define USER_AGENT "prikmeter"

#ifdef READ_FROM_SERIAL
#  define P1_INPUT Serial // Debugging aid.
#else
#  define P1_INPUT p1
#endif

Led led;
SoftwareSerial p1;
Config config;
TelegramReader telegramReader;
InverterReader inverterReader;
Session tlsSession;
WiFiClientSecure httpsClient;

void printTelegram(byte const *buffer, unsigned int size) {
  Serial.write(buffer, size);
}

/**
 * Uploads the telegram in the given `buffer` of `size` bytes to the server.
 * Returns `true` on success.
 */
ErrorCode uploadTelegram(byte const *buffer, uint16 size) {
  if (!httpsClient.connect(config.serverHost(), config.serverPort())) {
    Serial.print("Failed to connect to ");
    Serial.print(config.serverHost());
    Serial.print(":");
    Serial.print(config.serverPort());
    if (httpsClient.getLastSSLError()) {
      char sslError[256];
      httpsClient.getLastSSLError(sslError, sizeof(sslError) / sizeof(char));
      Serial.print(" due to SSL error: ");
      Serial.print(sslError);
      Serial.println();
      return SERVER_SSL_ERROR;
    } else {
      Serial.println();
      return SERVER_CONNECT_ERROR;
    }
  }

  httpsClient.print(
      "POST /telegrams HTTP/1.1\r\n"
      "Host: ");
  httpsClient.print(config.serverHost());
  httpsClient.print("\r\n"
      "User-Agent: " USER_AGENT " ");
  httpsClient.print(GIT_VERSION);
  httpsClient.print("\r\n"
      "Content-Type: text/plain\r\n"
      "Content-Length: ");
  httpsClient.print(size);
  httpsClient.print("\r\n"
      "X-Auth-Token: ");
  httpsClient.print(config.authToken());
  httpsClient.print("\r\n"
      "Connection: close\r\n"
      "\r\n");
  httpsClient.write(buffer, size);

  // "HTTPS/1.1 ", we stop reading after the space.
  while (true) {
    byte b = httpsClient.read();
    if (b < 0) {
      httpsClient.stop();
      return SERVER_READ_ERROR;
    }
    if (b == ' ') {
      break;
    }
  }
  // "200 ", we stop reading after the space.
  int statusCode = 0;
  while (true) {
    byte b = httpsClient.read();
    if (b < 0) {
      httpsClient.stop();
      return SERVER_READ_ERROR;
    }
    if (!isDigit(b)) {
      break;
    }
    statusCode = (statusCode * 10) + (b - '0');
    if (statusCode > 999) {
      httpsClient.stop();
      return SERVER_PROTOCOL_ERROR;
    }
  }
  if (statusCode != 200) {
    Serial.print("Non-success HTTP response code from server: ");
    Serial.print(statusCode);
    Serial.print(" ");
    // "OK" or whatever descriptive message there is.
    while (true) {
      int b = httpsClient.read();
      if (b < 0) {
        httpsClient.stop();
        return SERVER_READ_ERROR;
      }
      if (b == '\r') {
        break;
      }
      Serial.write((byte) b);
    }
    Serial.println();
    httpsClient.stop();
    if (statusCode == 400) {
      return TELEGRAM_CHECKSUM_ERROR;
    } else {
      return SERVER_RESPONSE_ERROR;
    }
  }

  Serial.print("Uploaded telegram of ");
  Serial.print(size);
  Serial.println(" bytes");
  return NO_ERROR;
}

void setup() {
  led.begin();
  led.set(true);

  Serial.begin(115200);

  Serial.println("Mounting file system");
  LittleFSConfig fsConfig;
  fsConfig.setAutoFormat(false); // TODO set to true once we have a configuration UI
  LittleFS.setConfig(fsConfig);
  LittleFS.begin();

  Serial.println("Reading configuration");
  ErrorCode error = config.begin();
  if (error) {
    led.flashNumber(error);
    return;
  }

  httpsClient.setSession(&tlsSession);
  httpsClient.setFingerprint(config.serverCertificateFingerprint());

  Serial.println("Opening P1 port");
  p1.begin(P1_BAUD, P1_CONFIG, P1_PIN, -1, P1_INVERT, P1_BUFFER_SIZE_BYTES);

  Serial.print("Connecting to wifi access point \"");
  Serial.print(config.wifiSsid());
  Serial.print("\" [");
  WiFi.mode(WIFI_STA);
  WiFi.begin(config.wifiSsid(), config.wifiPassword());
  while (WiFi.status() != WL_CONNECTED) {
    led.flash(250);
    delay(250);
    Serial.print(".");
  }
  Serial.println("]");
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Synchronizing system clock to NTP server [");
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  while (now < 1000) {
    led.flash(125);
    delay(125);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("]");
  now = time(nullptr);
  Serial.print("Current time: ");
  Serial.print(ctime(&now));

  Serial.println("Setting up inverter reader");
  inverterReader.begin(config);

  Serial.println("Enabling auto sleep");
  WiFi.setSleepMode(WIFI_MODEM_SLEEP);

  Serial.println("Up and running");

  led.set(false);

  // Serial.println("Sending test telegram");
  // char const *testTelegram = "/hello\r\n!world\r\n";
  // uploadTelegram((byte const *) testTelegram, strlen(testTelegram));

  inverterReader.update();
  Serial.print("Current power (W): ");
  Serial.println(inverterReader.powerWatts());
  Serial.print("Total energy (kWh): ");
  Serial.println(inverterReader.totalEnergyWattHours() / 1000.0);
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
    led.flashNumber(TELEGRAM_READ_TIMEOUT);
  }

  int b = P1_INPUT.read();
  if (b >= 0) {
    bool wasEmpty = telegramReader.isEmpty();

    telegramReader.addByte((byte) b);

    if (wasEmpty && !telegramReader.isEmpty()) {
      telegramStartTime = millis();
    }

    if (telegramReader.hasError()) {
      Serial.println("Telegram read error");
      telegramReader.reset();
      led.flashNumber(TELEGRAM_READ_ERROR);
    }

    if (telegramReader.isComplete()) {
      // Stop receiving data that we'll throw away anyway.
#ifndef READ_FROM_SERIAL
      P1_INPUT.enableRx(false);
#endif

      byte const *buffer = telegramReader.getBuffer();
      unsigned int size = telegramReader.getSize();
      Serial.print("Received telegram of ");
      Serial.print(size);
      Serial.println(" bytes");

#ifdef PRINT_TELEGRAM
      printTelegram(buffer, size);
#endif
#ifndef DONT_SEND_TELEGRAM
      ErrorCode uploadError = uploadTelegram(buffer, size);
      if (uploadError) {
        led.flashNumber(static_cast<uint16>(uploadError));
      } else {
        led.flash(50);
      }
#endif

      telegramReader.reset();

      // This works even if the clock wrapped around.
      unsigned long telegramReadTimeMillis = millis() - telegramStartTime;
      if (MIN_TELEGRAM_INTERVAL_MILLIS > telegramReadTimeMillis) {
        // The Arduino Core does continue processing system-level events during
        // delay():
        // https://arduino-esp8266.readthedocs.io/en/2.4.0-rc1/reference.html#timing-and-delays
        delay(MIN_TELEGRAM_INTERVAL_MILLIS - telegramReadTimeMillis);
      }

      P1_INPUT.flush();
#ifndef READ_FROM_SERIAL
      P1_INPUT.enableRx(true);
#endif
    }
  }
}
