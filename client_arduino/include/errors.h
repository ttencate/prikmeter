#pragma once

enum ErrorCode {
  NO_ERROR = 0,
  WIFI_CONNECT_ERROR = 1,
  NTP_ERROR = 2,
  TELEGRAM_READ_ERROR = 3,
  TELEGRAM_READ_TIMEOUT = 4,
  TELEGRAM_CHECKSUM_ERROR = 5,
  SERVER_CONNECT_ERROR = 6,
  SERVER_SSL_ERROR = 7,
  SERVER_READ_ERROR = 8,
  SERVER_PROTOCOL_ERROR = 9,
  SERVER_RESPONSE_ERROR = 10,
  CONFIG_OPEN_ERROR = 11,
  CONFIG_PARSE_ERROR = 12,
};

class Led {
  public:
    void begin();
    void set(bool on);
    void flash(int ms);
    void flashNumber(uint16 number);
};
