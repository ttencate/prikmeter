#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <time.h>
#include <WiFiClientSecure.h>

#include PRIKMETER_CONFIG_INCLUDE
#include "telegram_reader.h"

#define SERIAL_RX_PIN D5
#define LED_PIN D4
#define P1_BUFFER_SIZE_BYTES 64

#define READ_TIMEOUT_MILLIS 5000

#ifndef SERVER_PORT
#define SERVER_PORT 443
#endif
#ifndef USER_AGENT
#define USER_AGENT "prikmeter" // TODO Add Git hash from buildinfo.h.
#endif

#define VERSION "1.0.0"

#ifdef READ_FROM_SERIAL
#  define P1_INPUT Serial // Debugging aid.
#else
#  define P1_INPUT p1
#endif

SoftwareSerial p1(SERIAL_RX_PIN, -1, true, P1_BUFFER_SIZE_BYTES);
TelegramReader telegramReader;
WiFiClientSecure httpsClient;

enum ErrorCode {
  NO_ERROR = 0,
  WIFI_CONNECT_ERROR = 1,
  NTP_ERROR = 2,
  TELEGRAM_READ_ERROR = 3,
  TELEGRAM_CHECKSUM_ERROR = 4,
  SERVER_CONNECT_ERROR = 5,
  SERVER_SSL_ERROR = 6,
  SERVER_READ_ERROR = 7,
  SERVER_PROTOCOL_ERROR = 8,
  SERVER_RESPONSE_ERROR = 9,
};

void setLed(bool on) {
  digitalWrite(LED_PIN, on ? LOW : HIGH);
}

void blinkLed(int ms) {
  setLed(true);
  delay(ms);
  setLed(false);
}

/**
 * Flashes out an error in a kind of "decimal morse code", for example:
 * 502 = ..... _ ..
 */
void flashNumber(uint16 number) {
  byte digits[5]; // Maximum 65535.
  byte numDigits = 0;
  while (number) {
    digits[numDigits] = number % 10;
    numDigits++;
    number /= 10;
  }
  if (numDigits == 0) {
    digits[0] = 0;
    numDigits = 1;
  }
  while (numDigits) {
    numDigits--;
    setLed(false);
    delay(500);

    byte digit = digits[numDigits];
    if (digit) {
      while (digit) {
        digit--;
        blinkLed(150);
        delay(150);
      }
    } else {
      blinkLed(500);
      delay(150);
    }
  }
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
    flashNumber(SERVER_CONNECT_ERROR);
    return false;
  }

  if (!httpsClient.verify(SERVER_CERTIFICATE_FINGERPRINT, SERVER_HOST)) {
    Serial.println("Certificate verification failed");
    httpsClient.stop();
    flashNumber(SERVER_SSL_ERROR);
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

  // "HTTPS/1.1 ", we stop reading after the space.
  while (true) {
    byte b = httpsClient.read();
    if (b < 0) {
      httpsClient.stop();
      flashNumber(SERVER_READ_ERROR);
      return false;
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
      flashNumber(SERVER_READ_ERROR);
      return false;
    }
    if (!isDigit(b)) {
      break;
    }
    statusCode = (statusCode * 10) + (b - '0');
    if (statusCode > 999) {
      httpsClient.stop();
      flashNumber(SERVER_PROTOCOL_ERROR);
      return false;
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
        flashNumber(SERVER_READ_ERROR);
        return false;
      }
      if (b == '\r') {
        break;
      }
      Serial.write((byte) b);
    }
    Serial.println();
    httpsClient.stop();
    if (statusCode == 400) {
      flashNumber(TELEGRAM_CHECKSUM_ERROR);
      return false;
    } else {
      flashNumber(SERVER_RESPONSE_ERROR);
      delay(500);
      flashNumber(statusCode);
      return false;
    }
  }

  Serial.print("Uploaded telegram of ");
  Serial.print(size);
  Serial.println(" bytes");
  flashNumber(NO_ERROR);

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

  Serial.println("Enabling auto sleep");
  WiFi.setSleepMode(WIFI_MODEM_SLEEP);

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
    flashNumber(TELEGRAM_READ_ERROR);
  }

  int b = P1_INPUT.read();
  if (b >= 0) {
    if (telegramReader.isEmpty()) {
      telegramStartTime = millis();
      setLed(true);
    }

    telegramReader.addByte((byte) b);
#ifdef PRINT_TELEGRAM
    Serial.write((byte) b);
#endif

    if (telegramReader.isComplete()) {
      setLed(false);

      unsigned int size = telegramReader.getSize();
      Serial.print("Received telegram of ");
      Serial.print(size);
      Serial.println(" bytes");

#ifndef DONT_SEND_TELEGRAM
      uploadTelegram(telegramReader.getBuffer(), size);
#endif

      telegramReader.reset();
    }
  }
}
