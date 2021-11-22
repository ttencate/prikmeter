#pragma once

#include <ArduinoModbus.h>

#include <Arduino.h>

#define ARDUINO_SUNSPEC_DEBUG_LOGGING 1

#if defined(ARDUINO_SUNSPEC_DEBUG_LOGGING) && (ARDUINO_SUNSPEC_DEBUG_LOGGING)
#  define ARDUINO_SUNSPEC_DEBUG_LOG(x) Serial.print(x)
#  define ARDUINO_SUNSPEC_DEBUG_LOGLN(x) Serial.println(x)
#else
#  define ARDUINO_SUNSPEC_DEBUG_LOG(x) do {} while(0)
#  define ARDUINO_SUNSPEC_DEBUG_LOGLN(x) do {} while(0)
#endif

/**
 * Client for the SunSpec protocol. It acts as an iterator over models, and
 * allows to either parse them or skip over them.
 */
class SunSpec {
  public:
    explicit SunSpec(ModbusClient *client);

    /**
     * Assumes that the client is connected. Searches for the start address
     * where SunSpec models are to be found. After this returns success
     * (`true`), according to specifications the current model should be the
     * `common` model with ID 1.
     */
    bool begin();

    /**
     * Returns whether we are currently at a valid model.
     */
    bool hasCurrentModel();

    /**
     * Returns the model ID of the current model.
     */
    uint16 currentModelId();

    /**
     * Returns true if the current model is of the given type.
     */
    template<typename ModelType>
    bool currentModelIs() {
      return currentModelId_ == ModelType::id();
    }

    /**
     * Wraps the current model into the given type, which must match the
     * current model ID.
     * The `ModelType` template argument must be one of the generated ones
     * deriving from `SunSpecModel`.
     */
    template<typename ModelType>
    bool parseCurrentModel(ModelType *out) {
      if (!currentModelIs<ModelType>()) {
        ARDUINO_SUNSPEC_DEBUG_LOG("Tried to parse model ");
        ARDUINO_SUNSPEC_DEBUG_LOG(currentModelId_);
        ARDUINO_SUNSPEC_DEBUG_LOG(" as ");
        ARDUINO_SUNSPEC_DEBUG_LOGLN(ModelType::id());
        return false;
      }

      uint16 const count = currentModelLength_;
      uint16 *const buffer = new uint16[count];
      if (!request(currentModelAddress_ + 2, count)) {
        delete[] buffer;
        return false;
      }
      for (uint16 i = 0; i < count; i++) {
        if (!read(&buffer[i])) {
          delete[] buffer;
          return false;
        }
      }

      out->setBuffer(buffer, count);
      return true;
    }

    /**
     * Moves on to the next model.
     */
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
