#pragma once

#include <ArduinoModbus.h>

#define ARDUINO_SUNSPEC_DEBUG_LOGGING 1

#if defined(ARDUINO_SUNSPEC_DEBUG_LOGGING) && (ARDUINO_SUNSPEC_DEBUG_LOGGING)
#  define ARDUINO_SUNSPEC_DEBUG_LOG(x) Serial.print(x)
#  define ARDUINO_SUNSPEC_DEBUG_LOGLN(x) Serial.println(x)
#else
#  define ARDUINO_SUNSPEC_DEBUG_LOG(x) do {} while(0)
#  define ARDUINO_SUNSPEC_DEBUG_LOGLN(x) do {} while(0)
#endif

/**
 * Client for the SunSpec protocol.
 */
class SunSpec {
  public:
    explicit SunSpec(ModbusClient *client);

    bool begin();

  private:
    ModbusClient *const client_;

    uint16 serverId_ = 0;
    uint16 start_ = 0;

    bool findStartAddress();
    bool checkStartAddress();
};
