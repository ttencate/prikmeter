#include <cstring>

#include "InverterReader.h"
#include "SunSpecInverterReader.h"

ErrorCode NullInverterReader::update() {
  return NO_ERROR;
}

ErrorCode InverterReader::begin(Config const &config) {
  char const *const inverterProtocol = config.inverterProtocol();
  if (!strcmp(inverterProtocol, "sunspec")) {
    impl_.reset(new SunSpecInverterReader(config.inverterHost(), config.inverterPort()));
  } else if (!strcmp(inverterProtocol, "")) {
    impl_.reset(new NullInverterReader());
  } else {
    Serial.print("Unknown inverter protocol: ");
    Serial.println(inverterProtocol);
    impl_.reset(new NullInverterReader());
    return CONFIG_VALUE_ERROR;
  }
  return NO_ERROR;
}
