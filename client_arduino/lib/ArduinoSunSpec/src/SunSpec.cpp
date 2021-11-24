#include "ArduinoSunSpec.h"

#include <Arduino.h>

namespace {

// Some libraries only look at 40000 and 50000, but the SunSpec specification
// really also allows 0.
uint16 const START_ADDRESSES[] = {0, 40000, 50000};

uint16 const START_ADDRESS_MARKER_A = 0x5375;
uint16 const START_ADDRESS_MARKER_B = 0x6e53;

uint16 const LAST_MODEL_ID = 0xffff;

}

SunSpec::SunSpec(ModbusClient *client) :
  client_(client)
{
}

bool SunSpec::begin() {
  if (!findServerId()) {
    return false;
  }
  return restart();
}

bool SunSpec::hasCurrentModel() {
  return currentModelId_ != LAST_MODEL_ID;
}

uint16 SunSpec::currentModelId() {
  return currentModelId_;
}

bool SunSpec::nextModel() {
  currentModelAddress_ += 2 + currentModelLength_;
  if (!readModelHeader()) {
    ARDUINO_SUNSPEC_DEBUG_LOGLN("Failed to read model header");
    return false;
  }
  return true;
}

bool SunSpec::restart() {
  currentModelAddress_ = start_ + 2;
  return readModelHeader();
}

bool SunSpec::findServerId() {
  // Server ID is usually 0 but some inverters get creative.
  serverId_ = 0;
  if (findStartAddress()) {
    return true;
  }

  // SMA Sunny Boy inverters work differently: the server ID is the Unit ID +
  // 123. The Unit ID is configurable, and its current value can be found from
  // four registers starting at 42109 at server ID 1. Note that simply reading
  // the one register we care about does not work; we have to read all four of
  // them!
  // See section 3.5.1 and 3.5.2 in the SMA Modbus documentation:
  // https://files.sma.de/downloads/SMA-Modbus-general-TI-en-10.pdf
  uint16 unitId;
  if (request(42109, 4, 1) &&
      // Physical serial number (2 registers).
      read(nullptr) &&
      read(nullptr) &&
      // Physical SusyId (1 register).
      read(nullptr) &&
      // Unit ID (1 register).
      read(&unitId))
  {
    serverId_ = unitId + 123;
    if (findStartAddress()) {
      return true;
    }
  }

  ARDUINO_SUNSPEC_DEBUG_LOGLN("Could not find SunSpec start address");
  return false;
}

bool SunSpec::findStartAddress() {
  // Find start address. It is identified by a pair of registers containing
  // magic numbers.
  for (size_t i = 0; i < sizeof(START_ADDRESSES) / sizeof(*START_ADDRESSES); i++) {
    start_ = START_ADDRESSES[i];
    if (checkStartAddress()) {
      return true;
    }
  }
  return false;
}

bool SunSpec::checkStartAddress() {
  uint16 a;
  uint16 b;
  if (request(start_, 2) &&
      read(&a) &&
      read(&b) &&
      a == START_ADDRESS_MARKER_A &&
      b == START_ADDRESS_MARKER_B)
  {
    ARDUINO_SUNSPEC_DEBUG_LOG("Found SunSpec start address at server ID ");
    ARDUINO_SUNSPEC_DEBUG_LOG(serverId_);
    ARDUINO_SUNSPEC_DEBUG_LOG(" address ");
    ARDUINO_SUNSPEC_DEBUG_LOGLN(start_);
    return true;
  }
  return false;
}

bool SunSpec::readModelHeader() {
  return
    request(currentModelAddress_, 2) &&
    read(&currentModelId_) &&
    read(&currentModelLength_);
}

bool SunSpec::request(uint16 address, uint16 count) {
  return request(address, count, serverId_);
}

bool SunSpec::request(uint16 address, uint16 count, uint16 serverId) {
  int readCount = client_->requestFrom(serverId, HOLDING_REGISTERS, address, count);
  if (readCount < count) {
    ARDUINO_SUNSPEC_DEBUG_LOG("Read error: requested ");
    ARDUINO_SUNSPEC_DEBUG_LOG(count);
    ARDUINO_SUNSPEC_DEBUG_LOG(" holding registers at ");
    ARDUINO_SUNSPEC_DEBUG_LOG(address);
    ARDUINO_SUNSPEC_DEBUG_LOG(" from server ID ");
    ARDUINO_SUNSPEC_DEBUG_LOG(serverId);
    ARDUINO_SUNSPEC_DEBUG_LOG(" but got only ");
    ARDUINO_SUNSPEC_DEBUG_LOGLN(readCount);
    return false;
  }
  return true;
}

bool SunSpec::read(uint16 *result) {
  long const value = client_->read();
  if (value < 0) {
    ARDUINO_SUNSPEC_DEBUG_LOG("Failed to read value");
    return false;
  }
  if (result) {
    *result = static_cast<uint16>(value);
  }
  return true;
}

uint16 *SunSpec::readArray(uint16 offset, uint16 count) {
  if (!request(offset, count)) {
    return nullptr;
  }

  uint16 *const buffer = new uint16[count];

  for (uint16 i = 0; i < count; i++) {
    if (!read(&buffer[i])) {
      delete[] buffer;
      return nullptr;
    }
  }

  return buffer;
}
