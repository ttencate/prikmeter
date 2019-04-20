#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <time.h>
#include <WiFiClientSecure.h>

#include PRIKMETER_CONFIG_INCLUDE
#include "buildinfo.h"

#include "telegram_reader.h"

#define SERIAL_RX_PIN D5
#define LED_PIN D4
#define P1_BUFFER_SIZE_BYTES 4096

#define READ_TIMEOUT_MILLIS 5000

#define MIN_TELEGRAM_INTERVAL_MILLIS 9500

#ifndef SERVER_PORT
#define SERVER_PORT 443
#endif
#ifndef USER_AGENT
#define USER_AGENT "prikmeter"
#endif

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
  TELEGRAM_READ_TIMEOUT = 4,
  TELEGRAM_CHECKSUM_ERROR = 5,
  SERVER_CONNECT_ERROR = 6,
  SERVER_SSL_ERROR = 7,
  SERVER_READ_ERROR = 8,
  SERVER_PROTOCOL_ERROR = 9,
  SERVER_RESPONSE_ERROR = 10,
};

void setLed(bool on) {
  digitalWrite(LED_PIN, on ? LOW : HIGH);
}

void flashLed(int ms) {
  setLed(true);
  delay(ms);
  setLed(false);
}

/**
 * Flashes out a decimal number in Morse code:
 * 0 -----
 * 1 .----
 * 2 ..---
 * 3 ...--
 * 4 ....-
 * 5 .....
 * 6 -....
 * 7 --...
 * 8 ---..
 * 9 ----.
 */
void flashNumber(uint16 number) {
  unsigned int const DOT_DURATION = 150;
  unsigned int const DASH_DURATION = 3 * DOT_DURATION;
  unsigned int const INTERVAL_DURATION = DOT_DURATION;
  unsigned int const CHARACTER_SEPARATOR_DURATION = 3 * DOT_DURATION;
  byte digits[5]; // Maximum is 65535, which is 5 digits long.
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
    delay(CHARACTER_SEPARATOR_DURATION);

    byte digit = digits[numDigits];
    for (byte i = 0; i < 5; i++) {
      // In case of underflow, this wraps and becomes greater than 5.
      if (static_cast<byte>(digit - 1 - i) < 5) {
        flashLed(DOT_DURATION);
      } else {
        flashLed(DASH_DURATION);
      }
      if (i < 4) {
        delay(INTERVAL_DURATION);
      }
    }
  }
}

void printTelegram(byte const *buffer, unsigned int size) {
  Serial.write(buffer, size);
}

/**
 * Uploads the telegram in the given `buffer` of `size` bytes to the server.
 * Returns `true` on success.
 */
ErrorCode uploadTelegram(byte const *buffer, uint16 size) {
  if (!httpsClient.connect(SERVER_HOST, SERVER_PORT)) {
    Serial.print("Failed to connect to " SERVER_HOST ":");
    Serial.print(SERVER_PORT);
    Serial.println();
    return SERVER_CONNECT_ERROR;
  }

  if (!httpsClient.verify(SERVER_CERTIFICATE_FINGERPRINT, SERVER_HOST)) {
    Serial.println("Certificate verification failed");
    httpsClient.stop();
    return SERVER_SSL_ERROR;
  }

  httpsClient.print(
      "POST /telegrams HTTP/1.1\r\n"
      "Host: " SERVER_HOST "\r\n"
      "User-Agent: " USER_AGENT " ");
  httpsClient.print(_BuildInfo.src_version);
  httpsClient.print("\r\n"
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
  pinMode(LED_PIN, OUTPUT);
  setLed(true);

  Serial.begin(115200);

  Serial.println("Opening P1 port");
  p1.begin(115200);

  Serial.print("Connecting to wifi [");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    setLed(true);
    delay(250);
    setLed(false);
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
    setLed(true);
    delay(125);
    setLed(false);
    delay(125);
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
    flashNumber(TELEGRAM_READ_TIMEOUT);
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
      flashNumber(TELEGRAM_READ_ERROR);
    }

    if (telegramReader.isComplete()) {
      // Stop receiving data that we'll throw away anyway. The SoftwareSerial
      // implementation does busy-waiting, so this might save some power too.
      P1_INPUT.enableRx(false);

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
        flashNumber(static_cast<uint16>(uploadError));
      } else {
        flashLed(50);
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
      P1_INPUT.enableRx(true);
    }
  }
}
