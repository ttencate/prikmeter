#pragma once

#include <memory>

#include "Config.h"
#include "errors.h"

class InverterReaderImpl {
  public:
    /**
     * Implementations should write into the appropriate protected fields.
     */
    virtual ErrorCode update() = 0;

  protected:
    InverterReaderImpl() {}

    double powerWatts_ = 0;
    double totalEnergyWattHours_ = 0;

  private:
    InverterReaderImpl(InverterReaderImpl const &) = delete;
    InverterReaderImpl &operator=(InverterReaderImpl const &) = delete;

    friend class InverterReader;
};

class NullInverterReader : public InverterReaderImpl {
  public:
    NullInverterReader() {}

    ErrorCode update() override;
};

class InverterReader {
  public:
    /**
     * Must be called before any other methods on this object.
     */
    ErrorCode begin(Config const &config);

    ErrorCode update() { return impl_->update(); }

    double powerWatts() const { return impl_->powerWatts_; }
    double totalEnergyWattHours() const { return impl_->totalEnergyWattHours_; }

  private:
    std::unique_ptr<InverterReaderImpl> impl_;
};
