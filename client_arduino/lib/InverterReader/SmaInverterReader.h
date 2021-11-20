#pragma once

#include "Config.h"
#include "errors.h"
#include "InverterReader.h"

/**
 * InverterReaderImpl for SMA Sunny Boy inverters.
 */
class SmaInverterReader : public InverterReaderImpl {
  public:
    SmaInverterReader(Config const &config);

    ErrorCode update() override;

  private:
    String host_;
    String accountType_;
    String password_;
};
