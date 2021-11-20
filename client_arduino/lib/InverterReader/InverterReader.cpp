#include <cstring>

#include "InverterReader.h"
#include "SmaInverterReader.h"

ErrorCode NullInverterReader::update() {
  return NO_ERROR;
}

ErrorCode InverterReader::begin(Config const &config) {
  char const *const inverterType = config.inverterType();
  if (!strcmp(inverterType, "sma")) {
    impl_.reset(new SmaInverterReader(config));
  } else if (!strcmp(inverterType, "")) {
    impl_.reset(new NullInverterReader());
  } else {
    Serial.print("Unknown inverter type: ");
    Serial.println(inverterType);
    impl_.reset(new NullInverterReader());
    return CONFIG_VALUE_ERROR;
  }
  return NO_ERROR;
}
