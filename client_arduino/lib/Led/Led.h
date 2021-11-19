#pragma once

#include <Arduino.h>

class Led {
  public:
    void begin();
    void set(bool on);
    void flash(int ms);
    void flashNumber(uint16 number);
};

