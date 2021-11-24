#pragma once

#include "errors.h"
#include "InverterReader.h"

#include <ArduinoModbus.h>
#include <ArduinoSunSpec.h>
#include <ESP8266WiFi.h>

/**
 * InverterReaderImpl for inverters supporting the SunSpec TCP protocol.
 */
class SunSpecInverterReader : public InverterReaderImpl {
  public:
    SunSpecInverterReader(String const &host, uint16 port);

    ErrorCode update() override;

  private:
    String host_;
    uint16 port_;

    WiFiClient tcpClient_;
    ModbusTCPClient modbusClient_;
    SunSpec sunSpec_;

    bool isConnected();
    ErrorCode connect();
};
