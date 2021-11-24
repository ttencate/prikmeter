#include "SunSpecInverterReader.h"

SunSpecInverterReader::SunSpecInverterReader(String const &host, uint16 port) :
  host_(host),
  port_(port),
  tcpClient_(),
  modbusClient_(tcpClient_),
  sunSpec_(&modbusClient_)
{
}

ErrorCode SunSpecInverterReader::connect() {
  // TODO if the host is a dotted quad, parse it into an IP address
  IPAddress ipAddress;
  if (!WiFi.hostByName(host_.c_str(), ipAddress)) {
    return MODBUS_DNS_ERROR;
  }
  Serial.print("Resolved ");
  Serial.print(host_.c_str());
  Serial.print(" to ");
  Serial.println(ipAddress);

  if (!modbusClient_.begin(ipAddress, port_)) {
    Serial.print("Failed to connect to ");
    Serial.println(ipAddress);
    return MODBUS_CONNECT_ERROR;
  }

  sunSpec_.begin();

  SunSpecModels::Common model = sunSpec_.findModel<SunSpecModels::Common>();
  if (!model.isValid()) {
    Serial.println("Failed to parse SunSpec common model");
    return SUNSPEC_PROTOCOL_ERROR;
  }
  Serial.print("Connected to inverter: ");
  Serial.print(model.manufacturer());
  Serial.print(" ");
  Serial.println(model.model());

  return NO_ERROR;
}

bool SunSpecInverterReader::isConnected() {
  return modbusClient_.connected();
}

ErrorCode SunSpecInverterReader::update() {
  if (!isConnected()) {
    ErrorCode error = connect();
    if (error) {
      return error;
    }
  }

  sunSpec_.restart();
  while (sunSpec_.hasCurrentModel()) {
    // Serial.print("Found model ");
    // Serial.println(sunSpec.currentModelId());

    if (sunSpec_.currentModelIs<SunSpecModels::InverterSinglePhase>()) {
      SunSpecModels::InverterSinglePhase model = sunSpec_.currentModelAs<SunSpecModels::InverterSinglePhase>();
      if (!model.isValid()) {
        continue;
      }
      powerWatts_ = model.watts();
      totalEnergyWattHours_ = model.wattHours();
    }
    // TODO add split-phase and three-phase inverters as well as all their FLOAT counterparts

    if (!sunSpec_.nextModel()) {
      break;
    }
  }

  return NO_ERROR;
}
