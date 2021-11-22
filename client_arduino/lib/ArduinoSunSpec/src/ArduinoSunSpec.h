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
    bool hasCurrentModel();
    uint16 currentModelId();
    bool nextModel();

  private:
    ModbusClient *const client_;

    uint16 serverId_ = 0;
    uint16 start_ = 0;

    uint16 currentModelAddress_ = 0;
    uint16 currentModelId_ = 0;
    uint16 currentModelLength_ = 0;

    bool findServerId();
    bool findStartAddress();
    bool checkStartAddress();
    bool readModelHeader();

    /**
     * Starts a read of `count` holding registers starting at address `address`
     * from server ID `serverId_`.
     */
    bool request(uint16 address, uint16 count);

    /**
     * Starts a read of `count` holding registers starting at address `address`
     * from server ID `serverId`.
     */
    bool request(uint16 address, uint16 count, uint16 serverId);

    /**
     * Reads the next register from the last request.
     */
    bool read(uint16 *result);
};
