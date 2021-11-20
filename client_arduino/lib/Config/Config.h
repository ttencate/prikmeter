#pragma once

#include <ArduinoJson.h>

#include "errors.h"

/**
 * Reads configuration file from /prikmeter.json and stores its values.
 * We use JSON because we depend on ArduinoJson anyway, for talking to
 * inverters.
 */
class Config {
  public:
    ErrorCode begin();

    char const *wifiSsid() const { return wifiSsid_; }
    char const *wifiPassword() const { return wifiPassword_; }

    char const *serverHost() const { return serverHost_; }
    uint16 serverPort() const { return serverPort_; }
    char const *serverCertificateFingerprint() const { return serverCertificateFingerprint_; }
    char const *authToken() const { return authToken_; }

    char const *inverterType() const { return inverterType_; }
    char const *inverterHost() const { return inverterHost_; }
    char const *smaAccountType() const { return smaAccountType_; }
    char const *inverterPassword() const { return inverterPassword_; }

  private:
    StaticJsonDocument<1024> doc_;

    // Strings point into the above JSON document object.
    char const *wifiSsid_ = 0;
    char const *wifiPassword_ = 0;

    char const *serverHost_ = 0;
    uint16 serverPort_ = 0;
    char const *serverCertificateFingerprint_ = 0;
    char const *authToken_ = 0;

    char const *inverterType_ = 0;
    char const *inverterHost_ = 0;
    char const *smaAccountType_ = 0; // "usr" or "istl"
    char const *inverterPassword_ = 0;
};
