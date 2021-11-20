#include "SmaInverterReader.h"

#include <SMAReader.h>

#include <ESP8266WiFi.h>

SmaInverterReader::SmaInverterReader(Config const &config) :
  host_(config.inverterHost()),
  accountType_(config.smaAccountType()),
  password_(config.inverterPassword())
{
}

ErrorCode SmaInverterReader::update() {
  // TODO cache DNS results while the SMAReader is succeeding on that address
  // TODO if the host is a dotted quad, parse it into an IP address
  IPAddress ipAddress;
  if (!WiFi.hostByName(host_.c_str(), ipAddress)) {
    return DNS_ERROR;
  }
  Serial.print("Resolved ");
  Serial.print(host_.c_str());
  Serial.print(" to ");
  Serial.println(ipAddress);

  // This object is lightweight; the constructor just assigns some fields. So
  // it's okay to create it anew every time.
  SMAReader smaReader(ipAddress, accountType_.c_str(), password_.c_str());

  int const NUM_KEYS = 2;
  String keys[NUM_KEYS] = {KEY_POWER, KEY_ENERGY_TOTAL};
  int values[NUM_KEYS];
  if (!smaReader.getValues(NUM_KEYS, keys, values)) {
    Serial.println("SMA reader failed to read values");
    return INVERTER_READ_ERROR;
  }

  powerWatts_ = values[0];
  totalEnergyWattHours_ = values[1];

  return NO_ERROR;
}
