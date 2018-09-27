#include <Arduino.h>

#include "telegram_reader.h"

TelegramReader::TelegramReader() {
  reset();
}

void TelegramReader::reset() {
  size = 0;
  error = false;
  atStartOfLine = true;
  seenCr = false;
  inLastLine = false;
  complete = false;
}

bool TelegramReader::addByte(byte b) {
  if (complete || error) {
    return false;
  }

  if (atStartOfLine) {
    if (b == '/') {
      // Start of telegram: truncate buffer.
      reset();
    } else if (b == '!') {
      // Last line (checksum): remember this.
      inLastLine = true;
    }
    atStartOfLine = false;
  }

  if (seenCr) {
    seenCr = false;
    if (b == '\n') {
      if (inLastLine) {
        complete = true;
      }
      atStartOfLine = true;
    }
  } else {
    if (b == '\r') {
      seenCr = true;
    }
  }

  if (size >= MAX_TELEGRAM_SIZE) {
    Serial.print("Telegram buffer overflow after ");
    Serial.print(MAX_TELEGRAM_SIZE);
    Serial.println(" bytes");
    error = true;
    return false;
  }

  buffer[size] = b;
  size++;
  return true;
}
