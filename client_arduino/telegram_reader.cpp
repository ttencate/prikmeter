#include <Arduino.h>

#include "telegram_reader.h"

TelegramReader::TelegramReader() {
  reset();
}

void TelegramReader::reset() {
  size = 0;
  error = false;
  insideTelegram = false;
  atStartOfLine = true;
  seenCr = false;
  inChecksumLine = false;
  complete = false;
}

bool TelegramReader::addByte(byte b) {
  if (complete || error) {
    return false;
  }

  if (atStartOfLine) {
    if (b == '/') {
      // Start of telegram: truncate buffer.
      size = 0;
      insideTelegram = true;
    } else if (insideTelegram && b == '!') {
      // Last line (checksum): remember this.
      inChecksumLine = true;
    }
    atStartOfLine = false;
  }

  if (seenCr) {
    seenCr = false;
    if (b == '\n') {
      if (inChecksumLine) {
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

  if (!insideTelegram) {
    return false;
  }

  buffer[size] = b;
  size++;
  return true;
}

bool TelegramReader::justStarted() const {
  return size == 1;
}
