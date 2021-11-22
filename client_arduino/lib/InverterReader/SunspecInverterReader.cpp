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

    if (sunSpec.currentModelIs<SunSpecModels::common>()) {
      SunSpecModels::common commonModel;
      if (!sunSpec.parseCurrentModel(&commonModel)) {
        continue;
      }
      Serial.print("Manufacturer: ");
      Serial.println(commonModel.Mn());
      Serial.print("Device: ");
      Serial.println(commonModel.Md());
      Serial.print("Options: ");
      Serial.println(commonModel.opt());
    }

    if (!sunSpec.nextModel()) {
      break;
    }
  }

  // powerWatts_ = values[0];
  // totalEnergyWattHours_ = values[1];

  modbusClient.end();

  return NO_ERROR;
}
