#include <LittleFS.h>

#include "Config.h"

#define CONFIG_FILE_NAME "/prikmeter.json"

ErrorCode Config::begin() {
  File config = LittleFS.open(CONFIG_FILE_NAME, "r");
  if (!config) {
    Serial.println("Failed to open " CONFIG_FILE_NAME);
    return CONFIG_OPEN_ERROR;
  }

  DeserializationError error = deserializeJson(doc_, config);
  config.close();
  if (error) {
    Serial.print("Failed to parse " CONFIG_FILE_NAME ": ");
    Serial.println(error.c_str());
    return CONFIG_PARSE_ERROR;
  }

  wifiSsid_ = doc_["wifiSsid"] | "";
  wifiPassword_ = doc_["wifiPassword"] | "";

  serverHost_ = doc_["serverHost"] | "";
  serverPort_ = doc_["serverPort"] | 443;
  serverCertificateFingerprint_ = doc_["serverCertificateFingerprint"] | "";
  authToken_ = doc_["authToken"] | "";

  inverterProtocol_ = doc_["inverterProtocol"] | "";
  inverterHost_ = doc_["inverterHost"] | "";
  inverterPort_ = doc_["inverterPort"] | 502;

  return NO_ERROR;
}
