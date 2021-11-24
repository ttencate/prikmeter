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
     * Enumerates models until the one of the given type is found. Returns a
     * valid model on success, an invalid model if not found or any error
     * occurred.
     */
    template<typename ModelType>
    ModelType findModel() {
      while (hasCurrentModel()) {
        if (currentModelIs<ModelType>()) {
          return currentModelAs<ModelType>();
        }
        if (!nextModel()) {
          break;
        }
      }
      return ModelType();
    }

    /**
     * Returns the current model as the given type. Returns an invalid model if
     * the current model type does not match, or the parse failed.
     */
    template<typename ModelType>
    ModelType currentModelAs() {
      if (!currentModelIs<ModelType>()) {
        ARDUINO_SUNSPEC_DEBUG_LOG("Tried to parse model ");
        ARDUINO_SUNSPEC_DEBUG_LOG(currentModelId_);
        ARDUINO_SUNSPEC_DEBUG_LOG(" as ");
        ARDUINO_SUNSPEC_DEBUG_LOGLN(ModelType::id());
        return ModelType();
      }

      uint16 const count = currentModelLength_;
      uint16 *buffer = readArray(currentModelAddress_ + 2, count);
      if (!buffer) {
        return ModelType();
      }

      ModelType model;
      model.setBuffer(buffer, count);
      return model;
    }

    /**
     * Moves on to the next model.
     */
    bool nextModel();

    /**
     * Resets the current model pointer to the beginning.
     */
    bool restart();

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

    /**
     * Reads `count` registers into an array allocated with `new`, and returns
     * the array. Returns `nullptr` on failure.
     */
    uint16 *readArray(uint16 offset, uint16 count);

};
