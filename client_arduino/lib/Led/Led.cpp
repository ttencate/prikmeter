#include "Led.h"

#define LED_PIN D4

void Led::begin() {
  pinMode(LED_PIN, OUTPUT);
}

void Led::set(bool on) {
  digitalWrite(LED_PIN, on ? LOW : HIGH);
}

void Led::flash(int ms) {
  set(true);
  delay(ms);
  set(false);
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
void Led::flashNumber(uint16 number) {
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
    set(false);
    delay(CHARACTER_SEPARATOR_DURATION);

    byte digit = digits[numDigits];
    for (byte i = 0; i < 5; i++) {
      // In case of underflow, this wraps and becomes greater than 5.
      if (static_cast<byte>(digit - 1 - i) < 5) {
        flash(DOT_DURATION);
      } else {
        flash(DASH_DURATION);
      }
      if (i < 4) {
        delay(INTERVAL_DURATION);
      }
    }
  }
}
