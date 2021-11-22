#include "ArduinoSunSpec.h"

namespace {

// Some libraries only look at 40000 and 50000, but the SunSpec specification
// really also allows 0.
uint16 const START_ADDRESSES[] = {0, 40000, 50000};

uint16 const START_ADDRESS_MARKER_A = 0x5375;
uint16 const START_ADDRESS_MARKER_B = 0x6e53;

}

SunSpec::SunSpec(ModbusClient *client) :
  client_(client)
{
}

bool SunSpec::begin() {
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
  client_->requestFrom(1, HOLDING_REGISTERS, 42109, 4);
  // Physical serial number (2 registers).
  client_->read();
  client_->read();
  // Physical SusyId (1 register).
  client_->read();
  // Unit ID (1 register).
  const long unitId = client_->read();
  if (unitId >= 0) {
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
  client_->requestFrom(serverId_, HOLDING_REGISTERS, start_, 2);
  const long a = client_->read();
  const long b = client_->read();
  if (a == START_ADDRESS_MARKER_A && b == START_ADDRESS_MARKER_B) {
    ARDUINO_SUNSPEC_DEBUG_LOG("Found SunSpec start address at server ID ");
    ARDUINO_SUNSPEC_DEBUG_LOG(serverId_);
    ARDUINO_SUNSPEC_DEBUG_LOG(" address ");
    ARDUINO_SUNSPEC_DEBUG_LOGLN(start_);
    return true;
  }
  return false;
}
