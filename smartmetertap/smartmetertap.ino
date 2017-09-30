#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

#include "config.h"

#define SERIAL_RX_PIN D5
#define BUFFER_SIZE_BYTES 64

SoftwareSerial p1(SERIAL_RX_PIN, -1, true, BUFFER_SIZE_BYTES);

void setup() {
  Serial.begin(115200);

  Serial.println("Opening P1 port");
  p1.begin(115200);

  Serial.println("Connecting to wifi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Up and running");
}

void loop() {
  if (p1.available()) {
    int byte = p1.read();
    Serial.write(byte);
  }
}
