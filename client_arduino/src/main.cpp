#include <Arduino.h>
#include <ArduinoHttpServer.h>
#include <ESP8266WiFi.h>
#include <InverterReader.h>
#include <LittleFS.h>
#include <SoftwareSerial.h>
#include <time.h>
#include <WiFiClientSecure.h>

#include "Config.h"
#include "errors.h"
#include "InverterReader.h"
#include "Led.h"
#include "TelegramReader.h"

#include "dist_files.cpp" // Headers? We don't need no stinkin' headers!

#define P1_PIN D5
#define P1_BAUD 115200
#define P1_CONFIG SWSERIAL_8N1
#define P1_INVERT true
// Receive buffer size for P1 port. We'd like to set this higher, but even 1024
// (which would fit a typical telegram in its entirety) results in OOMs at
// startup.
// TODO We might want to tweak this according to
// https://github.com/plerup/espsoftwareserial#resource-optimization
// because there is a second buffer, the signal edge detection buffer, which is
// probably the one that fills up between read() calls. We'd need about
// 10*1024*4 = 40 kB for that to hold an entire telegram.
#define P1_BUFFER_SIZE_BYTES 128

#define TELEGRAM_READ_TIMEOUT_MILLIS 5000
#define MIN_TELEGRAM_INTERVAL_MILLIS 9500

#define INVERTER_READ_INTERVAL_MILLIS 10000

#define USER_AGENT "prikmeter"

#define HTTP_PORT 80

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
WiFiServer httpServer(HTTP_PORT);

// TODO store all strings in PROGMEM using the F() macro:
// https://arduino-esp8266.readthedocs.io/en/3.0.2/PROGMEM.html

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

  Serial.print("Starting to listen on http://");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.println(HTTP_PORT);
  httpServer.begin();

  Serial.println("Enabling auto sleep");
  WiFi.setSleepMode(WIFI_MODEM_SLEEP);

  Serial.println("Up and running");

  led.set(false);

  // Serial.println("Sending test telegram");
  // char const *testTelegram = "/hello\r\n!world\r\n";
  // uploadTelegram((byte const *) testTelegram, strlen(testTelegram));
}

void readP1() {
  // If we still don't have a complete telegram seconds after the start, assume
  // read error and reset the reader for the next one.
  static unsigned long telegramStartTime = millis();
  if (!telegramReader.isEmpty() && millis() - telegramStartTime > TELEGRAM_READ_TIMEOUT_MILLIS) {
    Serial.print("Telegram still not completed after ");
    Serial.print(TELEGRAM_READ_TIMEOUT_MILLIS);
    Serial.println(" ms");
    telegramReader.reset();
    led.flashNumber(TELEGRAM_READ_TIMEOUT);
  }

  // Read as many bytes as we can at once, so that the buffer is empty again
  // for new ones.
  while (true) {
    int b = P1_INPUT.read();
    if (b < 0) {
      break;
    }

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

void readInverter() {
  static unsigned long lastRead = 0;

  unsigned long const now = millis();
  if (now - lastRead >= INVERTER_READ_INTERVAL_MILLIS) {
    lastRead = now;
    inverterReader.update();
    Serial.print("Current power (W): ");
    Serial.println(inverterReader.powerWatts());
    Serial.print("Total energy (kWh): ");
    Serial.println(inverterReader.totalEnergyWattHours() / 1000.0);
  }
}

/**
 * Simple HTTP response wrapper. We don't use the ArduinoHttpServer reply
 * classes because they are very inflexible; for example, they don't let us
 * send custom headers.
 */
// TODO extract to separate file
class HttpResponse {
  public:
    HttpResponse(WiFiClient &client) :
      client_(client)
    {
    }

    ~HttpResponse() {
      client_.stop();
    }

    void sendStatus(int code, char const *text) {
      client_.print("HTTP/1.1 ");
      client_.print(code);
      client_.print(" ");
      client_.print(text);
      client_.print("\r\n");
    }

    void sendHeader(char const *name, char const *value) {
      client_.print(name);
      client_.print(": ");
      client_.print(value);
      client_.print("\r\n");
    }

    void sendData(String const &data) {
      if (!headersEnded_) {
        client_.print("\r\n");
        headersEnded_ = true;
      }
      client_.print(data);
    }

    void sendData_P(PGM_P data, size_t length) {
      if (!headersEnded_) {
        client_.print("\r\n");
        headersEnded_ = true;
      }
      client_.write_P(data, length);
    }

    void sendError(int code, char const *text) {
      sendStatus(code, text);
      sendHeader("Content-Type", "text/plain; charset=UTF-8");
      sendData(String(code));
      sendData(" ");
      sendData(text);
    }

  private:
    WiFiClient &client_;
    bool headersEnded_ = false;
};

void handleRequest(WiFiClient &client, ArduinoHttpServer::StreamHttpRequest<1024> &request) {
  HttpResponse response(client);
  if (request.getMethod() != ArduinoHttpServer::Method::Get) {
    response.sendError(405, "Method Not Allowed");
    return;
  }
  String path = request.getResource().toString();
  if (path == "/") {
    response.sendStatus(200, "OK");
    response.sendHeader("Content-Type", "text/html; charset=UTF-8");
    response.sendData_P(dist_files::index_html, dist_files::index_html_len);
  } else if (path == "/style.css") {
    response.sendStatus(200, "OK");
    response.sendHeader("Content-Type", "text/css; charset=UTF-8");
    response.sendData_P(dist_files::style_css, dist_files::style_css_len);
  } else if (path == "/favicon.ico") {
    // TODO draw a favicon
    response.sendError(404, "Not Found");
  } else {
    response.sendError(404, "Not Found");
  }
}

void serveHttp() {
  WiFiClient client = httpServer.available();
  if (client) {
    // TODO I'm not sure what this buffer size is for. Do we even need this lib?
    ArduinoHttpServer::StreamHttpRequest<1024> request(client);
    if (request.readRequest()) {
      handleRequest(client, request);
    }
  }
}

void loop() {
  readP1();
  readInverter();
  serveHttp();
}
