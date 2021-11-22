#pragma once

#include "errors.h"
#include "InverterReader.h"

/**
 * InverterReaderImpl for inverters supporting the SunSpec TCP protocol.
 */
class SunspecInverterReader : public InverterReaderImpl {
  public:
    SunspecInverterReader(String const &host, uint16 port);

    ErrorCode update() override;

  private:
    String host_;
    uint16 port_;
};
