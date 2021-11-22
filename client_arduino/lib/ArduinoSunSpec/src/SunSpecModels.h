#pragma once

#include "SunSpecModel.h"

namespace SunSpecModels {

class common : public SunSpecModel<1, 66> {
  public:
    inline String Mn() const { return parse_string(0, 16); }
    inline String Md() const { return parse_string(16, 16); }
    inline String opt() const { return parse_string(32, 8); }
    inline String Vr() const { return parse_string(40, 8); }
    inline String SN() const { return parse_string(48, 16); }
    inline uint16 DA() const { return parse_uint16(64); }
};

}
