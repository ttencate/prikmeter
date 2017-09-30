#include <Arduino.h>
#include <SoftwareSerial.h>

#define SERIAL_RX_PIN D5
#define BUFFER_SIZE_BYTES 64

SoftwareSerial p1(SERIAL_RX_PIN, -1, true, BUFFER_SIZE_BYTES);

void setup() {
  p1.begin(115200);

  Serial.begin(115200);
  Serial.println("Up and running");
}

void loop() {
  if (p1.available()) {
    int byte = p1.read();
    Serial.write(byte);
  }
}
