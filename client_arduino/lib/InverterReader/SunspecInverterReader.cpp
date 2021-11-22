#include "SunspecInverterReader.h"

#include "ArduinoSunSpec.h"

#include <ArduinoModbus.h>
#include <ESP8266WiFi.h>

SunspecInverterReader::SunspecInverterReader(String const &host, uint16 port) :
  host_(host),
  port_(port)
{
}

ErrorCode SunspecInverterReader::update() {
  // TODO if the host is a dotted quad, parse it into an IP address
  IPAddress ipAddress;
  if (!WiFi.hostByName(host_.c_str(), ipAddress)) {
    return MODBUS_DNS_ERROR;
  }
  Serial.print("Resolved ");
  Serial.print(host_.c_str());
  Serial.print(" to ");
  Serial.println(ipAddress);

  // TODO retain connection while it is functioning
  WiFiClient tcpClient;
  ModbusTCPClient modbusClient(tcpClient);
  if (!modbusClient.begin(ipAddress, port_)) {
    return MODBUS_CONNECT_ERROR;
  }

  SunSpec sunSpec(&modbusClient);
  sunSpec.begin();
  while (sunSpec.hasCurrentModel()) {
    Serial.print("Found model ");
    Serial.println(sunSpec.currentModelId());

    if (sunSpec.currentModelIs<SunSpecModels::Common>()) {
      SunSpecModels::Common model;
      if (!sunSpec.parseCurrentModel(&model)) {
        continue;
      }
      Serial.print("Manufacturer: ");
      Serial.println(model.manufacturer());
      Serial.print("Device: ");
      Serial.println(model.model());
      Serial.print("Options: ");
      Serial.println(model.options());
    } else if (sunSpec.currentModelIs<SunSpecModels::InverterSinglePhase>()) {
      SunSpecModels::InverterSinglePhase model;
      if (!sunSpec.parseCurrentModel(&model)) {
        continue;
      }
      powerWatts_ = model.watts();
      totalEnergyWattHours_ = model.wattHours();
    }
    // TODO add split-phase and three-phase inverters as well as all their FLOAT counterparts

    if (!sunSpec.nextModel()) {
      break;
    }
  }

  // powerWatts_ = values[0];
  // totalEnergyWattHours_ = values[1];

  modbusClient.end();

  return NO_ERROR;
}
