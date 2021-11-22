#pragma once

#include "SunSpecModel.h"

namespace SunSpecModels {

/**
 * All SunSpec compliant devices must include this as the first model
 */
class Common : public SunSpecModel<1, 66> {
  public:
    /**
     * Well known value registered with SunSpec for compliance
     */
    inline String manufacturer() const { return parse_string(0, 16); }

    /**
     * Manufacturer specific value (32 chars)
     */
    inline String model() const { return parse_string(16, 16); }

    /**
     * Manufacturer specific value (16 chars)
     */
    inline String options() const { return parse_string(32, 8); }

    /**
     * Manufacturer specific value (16 chars)
     */
    inline String version() const { return parse_string(40, 8); }

    /**
     * Manufacturer specific value (32 chars)
     */
    inline String serialNumber() const { return parse_string(48, 16); }

    /**
     * Modbus device address
     */
    inline uint16_t deviceAddress() const { return parse_uint16(64); }

};

/**
 * Aggregates a collection of models for a given model id
 */
class BasicAggregator : public SunSpecModel<2, 14> {
  public:
    /**
     * Aggregated model id
     */
    inline uint16_t AID() const { return parse_uint16(0); }

    /**
     * Number of aggregated models
     */
    inline uint16_t N() const { return parse_uint16(1); }

    /**
     * Update Number.  Incrementing number each time the mapping is changed.  If the number is not changed from the last reading the direct access to a specific offset will result in reading the same logical model as before.  Otherwise the entire model must be read to refresh the changes
     */
    inline uint16_t UN() const { return parse_uint16(2); }

    /**
     * Enumerated status code
     */
    inline uint16_t status() const { return parse_enum16(3); }

    /**
     * Vendor specific status code
     */
    inline uint16_t vendorStatus() const { return parse_enum16(4); }

    /**
     * Bitmask event code
     */
    inline uint32_t eventCode() const { return parse_bitfield32(5); }

    /**
     * Vendor specific event code
     */
    inline uint32_t vendorEventCode() const { return parse_bitfield32(7); }

    /**
     * Control register for all aggregated devices
     */
    inline uint16_t control() const { return parse_enum16(9); }

    /**
     * Vendor control register for all aggregated devices
     */
    inline uint32_t vendorControl() const { return parse_enum32(10); }

    /**
     * Numerical value used as a parameter to the control
     */
    inline uint32_t controlValue() const { return parse_enum32(12); }

};

/**
 * To be included first for a complete interface description
 */
class CommunicationInterfaceHeader : public SunSpecModel<10, 4> {
  public:
    /**
     * Overall interface status
     */
    inline uint16_t interfaceStatus() const { return parse_enum16(0); }

    /**
     * Overall interface control (TBD)
     */
    inline uint16_t interfaceControl() const { return parse_uint16(1); }

    /**
     * Enumerated value.  Type of physical media
     */
    inline uint16_t physicalAccessType() const { return parse_enum16(2); }

};

/**
 * Include to support a wired ethernet port
 */
class EthernetLinkLayer : public SunSpecModel<11, 13> {
  public:
    /**
     * Interface speed in Mb/s [Mbps]
     */
    inline uint16_t ethernetLinkSpeed() const { return parse_uint16(0); }

    /**
     * Bitmask values Interface flags.
     */
    inline uint16_t interfaceStatusFlags() const { return parse_bitfield16(1); }

    /**
     * Enumerated value. State information for this interface
     */
    inline uint16_t linkState() const { return parse_enum16(2); }

    /**
     * IEEE MAC address of this interface
     */
    inline uint64_t MAC() const { return parse_eui48(3); }

    /**
     * Interface name (8 chars)
     */
    inline String name() const { return parse_string(7, 4); }

    /**
     * Control flags
     */
    inline uint16_t control() const { return parse_bitfield16(11); }

    /**
     * Forced interface speed in Mb/s when AUTO is disabled [Mbps]
     */
    inline uint16_t forcedSpeed() const { return parse_uint16(12); }

};

/**
 * Include to support an IPv4 protocol stack on this interface
 */
class IPv4 : public SunSpecModel<12, 98> {
  public:
    /**
     * Interface name
     */
    inline String name() const { return parse_string(0, 4); }

    /**
     * Enumerated value.  Configuration status
     */
    inline uint16_t configStatus() const { return parse_enum16(4); }

    /**
     * Bitmask value.  A configuration change is pending
     */
    inline uint16_t changeStatus() const { return parse_bitfield16(5); }

    /**
     * Bitmask value. Identify capable sources of configuration
     */
    inline uint16_t configCapability() const { return parse_bitfield16(6); }

    /**
     * Enumerated value.  Configuration method used.
     */
    inline uint16_t iPv4Config() const { return parse_enum16(7); }

    /**
     * Configure use of services
     */
    inline uint16_t control() const { return parse_enum16(8); }

    /**
     * IPv4 numeric address as a dotted string xxx.xxx.xxx.xxx
     */
    inline String IP() const { return parse_string(9, 8); }

    /**
     * IPv4 numeric netmask as a dotted string xxx.xxx.xxx.xxx
     */
    inline String netmask() const { return parse_string(17, 8); }

    /**
     * IPv4 numeric gateway address as a dotted string xxx.xxx.xxx.xxx
     */
    inline String gateway() const { return parse_string(25, 8); }

    /**
     * IPv4 numeric DNS address as a dotted string xxx.xxx.xxx.xxx
     */
    inline String DNS1() const { return parse_string(33, 8); }

    /**
     * IPv4 numeric DNS address as a dotted string xxx.xxx.xxx.xxx
     */
    inline String DNS2() const { return parse_string(41, 8); }

    /**
     * IPv4 numeric NTP address as a dotted string xxx.xxx.xxx.xxx
     */
    inline String NTP1() const { return parse_string(49, 12); }

    /**
     * IPv4 numeric NTP address as a dotted string xxx.xxx.xxx.xxx
     */
    inline String NTP2() const { return parse_string(61, 12); }

    /**
     * Domain name (24 chars max)
     */
    inline String domain() const { return parse_string(73, 12); }

    /**
     * Host name (24 chars max)
     */
    inline String hostName() const { return parse_string(85, 12); }

};

/**
 * Include to support an IPv6 protocol stack on this interface
 */
class IPv6 : public SunSpecModel<13, 174> {
  public:
    /**
     * Interface name
     */
    inline String name() const { return parse_string(0, 4); }

    /**
     * Enumerated value.  Configuration status
     */
    inline uint16_t configStatus() const { return parse_enum16(4); }

    /**
     * Bitmask value.  A configuration change is pending
     */
    inline uint16_t changeStatus() const { return parse_bitfield16(5); }

    /**
     * Bitmask value. Identify capable sources of configuration
     */
    inline uint16_t configCapability() const { return parse_bitfield16(6); }

    /**
     * Enumerated value.  Configuration method used.
     */
    inline uint16_t iPv6Config() const { return parse_enum16(7); }

    /**
     * Bitmask value.  Configure use of services
     */
    inline uint16_t control() const { return parse_enum16(8); }

    /**
     * IPv6 numeric address as a dotted string xxxx.xxxx.xxxx.xxxx
     */
    inline String IP() const { return parse_string(9, 20); }

    /**
     * Classless Inter-Domain Routing Number
     */
    inline String CIDR() const { return parse_string(29, 20); }

    /**
     * IPv6 numeric address as a dotted string xxxx.xxxx.xxxx.xxxx
     */
    inline String gateway() const { return parse_string(49, 20); }

    /**
     * IPv6 numeric DNS address as a dotted string xxxx.xxxx.xxxx.xxxx
     */
    inline String DNS1() const { return parse_string(69, 20); }

    /**
     * IPv6 numeric DNS address as a dotted string xxxx.xxxx.xxxx.xxxx
     */
    inline String DNS2() const { return parse_string(89, 20); }

    /**
     * IPv6 numeric NTP address as a name or dotted string xxxx.xxxx.xxxx.xxxx
     */
    inline String NTP1() const { return parse_string(109, 20); }

    /**
     * IPv6 numeric NTP address as a name or dotted string xxxx.xxxx.xxxx.xxxx
     */
    inline String NTP2() const { return parse_string(129, 20); }

    /**
     * Domain name (24 chars max)
     */
    inline String domain() const { return parse_string(149, 12); }

    /**
     * Host name (24 chars max)
     */
    inline String hostName() const { return parse_string(161, 12); }

};

/**
 * Include this block to allow for a proxy server
 */
class ProxyServer : public SunSpecModel<14, 52> {
  public:
    /**
     * Interface name (8 chars)
     */
    inline String name() const { return parse_string(0, 4); }

    /**
     * Bitmask value.  Proxy configuration capabilities
     */
    inline uint16_t capabilities() const { return parse_bitfield16(4); }

    /**
     * Enumerated value.  Set proxy address type
     */
    inline uint16_t config() const { return parse_enum16(5); }

    /**
     * Enumerate value.  Proxy server type
     */
    inline uint16_t type() const { return parse_bitfield16(6); }

    /**
     * IPv4 or IPv6 proxy hostname or dotted address (40 chars)
     */
    inline String address() const { return parse_string(7, 20); }

    /**
     * Proxy port number
     */
    inline uint16_t port() const { return parse_uint16(27); }

    /**
     * Proxy user name
     */
    inline String username() const { return parse_string(28, 12); }

    /**
     * Proxy password
     */
    inline String password() const { return parse_string(40, 12); }

};

/**
 * Interface counters
 */
class InterfaceCountersModel : public SunSpecModel<15, 24> {
  public:
    /**
     * Write a "1" to clear all counters
     */
    inline uint16_t clear() const { return parse_uint16(0); }

    /**
     * Number of bytes received
     */
    inline uint32_t inputCount() const { return parse_acc32(1); }

    /**
     * Number of Unicast packets received
     */
    inline uint32_t inputUnicastCount() const { return parse_acc32(3); }

    /**
     * Number of non-Unicast packets received
     */
    inline uint32_t inputNonUnicastCount() const { return parse_acc32(5); }

    /**
     * Number of inbound packets received on the interface but discarded
     */
    inline uint32_t inputDiscardedCount() const { return parse_acc32(7); }

    /**
     * Number of inbound packets that contain errors (excluding discards)
     */
    inline uint32_t inputErrorCount() const { return parse_acc32(9); }

    /**
     * Number of inbound packets with unknown protocol
     */
    inline uint32_t inputUnknownCount() const { return parse_acc32(11); }

    /**
     * Total number of bytes transmitted on this interface
     */
    inline uint32_t outputCount() const { return parse_acc32(13); }

    /**
     * Number of Unicast packets transmitted
     */
    inline uint32_t outputUnicastCount() const { return parse_acc32(15); }

    /**
     * Number of Non-Unicast packets transmitted
     */
    inline uint32_t outputNonUnicastCount() const { return parse_acc32(17); }

    /**
     * Number of Discarded output packets
     */
    inline uint32_t outputDiscardedCount() const { return parse_acc32(19); }

    /**
     * Number of outbound error packets
     */
    inline uint32_t outputErrorCount() const { return parse_acc32(21); }

};

/**
 * Include this model for a simple IPv4 network stack
 */
class SimpleIPNetwork : public SunSpecModel<16, 52> {
  public:
    /**
     * Interface name.  (8 chars)
     */
    inline String name() const { return parse_string(0, 4); }

    /**
     * Enumerated value.  Force IPv4 configuration method
     */
    inline uint16_t config() const { return parse_enum16(4); }

    /**
     * Bitmask value Configure use of services
     */
    inline uint16_t control() const { return parse_bitfield16(5); }

    /**
     * IP address
     */
    inline String address() const { return parse_string(6, 8); }

    /**
     * Netmask
     */
    inline String netmask() const { return parse_string(14, 8); }

    /**
     * Gateway IP address
     */
    inline String gateway() const { return parse_string(22, 8); }

    /**
     * 32 bit IP address of DNS server
     */
    inline String DNS1() const { return parse_string(30, 8); }

    /**
     * 32 bit IP address of DNS server
     */
    inline String DNS2() const { return parse_string(38, 8); }

    /**
     * IEEE MAC address of this interface
     */
    inline uint64_t MAC() const { return parse_eui48(46); }

    /**
     * Bitmask value.  Link control flags
     */
    inline uint16_t linkControl() const { return parse_bitfield16(50); }

};

/**
 * Include this model for serial interface configuration support
 */
class SerialInterface : public SunSpecModel<17, 12> {
  public:
    /**
     * Interface name (8 chars)
     */
    inline String name() const { return parse_string(0, 4); }

    /**
     * Interface baud rate in bits per second [bps]
     */
    inline uint32_t rate() const { return parse_uint32(4); }

    /**
     * Number of data bits per character
     */
    inline uint16_t bits() const { return parse_uint16(6); }

    /**
     * Bitmask value.  Parity setting
     */
    inline uint16_t parity() const { return parse_enum16(7); }

    /**
     * Enumerated value.  Duplex mode
     */
    inline uint16_t duplex() const { return parse_enum16(8); }

    /**
     * Flow Control Method
     */
    inline uint16_t flowControl() const { return parse_enum16(9); }

    /**
     * Enumerated value.  Interface type
     */
    inline uint16_t interfaceType() const { return parse_enum16(10); }

    /**
     * Enumerated value. Serial protocol selection
     */
    inline uint16_t protocol() const { return parse_enum16(11); }

};

/**
 * Include this model to support a cellular interface link
 */
class CellularLink : public SunSpecModel<18, 22> {
  public:
    /**
     * Interface name
     */
    inline String name() const { return parse_string(0, 4); }

    /**
     * International Mobile Equipment Identifier for the interface
     */
    inline uint32_t IMEI() const { return parse_uint32(4); }

    /**
     * Access Point Name for the interface
     */
    inline String APN() const { return parse_string(6, 4); }

    /**
     * Phone number for the interface
     */
    inline String number() const { return parse_string(10, 6); }

    /**
     * Personal Identification Number for the interface
     */
    inline String PIN() const { return parse_string(16, 6); }

};

/**
 * Include this model to configure a Point-to-Point Protocol link
 */
class PPPLink : public SunSpecModel<19, 30> {
  public:
    /**
     * Interface name
     */
    inline String name() const { return parse_string(0, 4); }

    /**
     * Interface baud rate in bits per second [bps]
     */
    inline uint32_t rate() const { return parse_uint32(4); }

    /**
     * Number of data bits per character
     */
    inline uint16_t bits() const { return parse_uint16(6); }

    /**
     * Bitmask value.  Parity setting
     */
    inline uint16_t parity() const { return parse_enum16(7); }

    /**
     * Enumerated value.  Duplex mode
     */
    inline uint16_t duplex() const { return parse_enum16(8); }

    /**
     * Flow Control Method
     */
    inline uint16_t flowControl() const { return parse_enum16(9); }

    /**
     * Enumerated value.  Authentication method
     */
    inline uint16_t authentication() const { return parse_enum16(10); }

    /**
     * Username for authentication
     */
    inline String username() const { return parse_string(11, 12); }

    /**
     * Password for authentication
     */
    inline String password() const { return parse_string(23, 6); }

};

/**
 * Include this model for single phase inverter monitoring
 */
class InverterSinglePhase : public SunSpecModel<101, 50> {
  public:
    /**
     * AC Current [A]
     */
    inline uint16_t amps() const { return parse_uint16(0); }

    /**
     * Phase A Current [A]
     */
    inline uint16_t ampsPhaseA() const { return parse_uint16(1); }

    /**
     * Phase B Current [A]
     */
    inline uint16_t ampsPhaseB() const { return parse_uint16(2); }

    /**
     * Phase C Current [A]
     */
    inline uint16_t ampsPhaseC() const { return parse_uint16(3); }

    inline int16_t A_SF() const { return parse_sunssf(4); }

    /**
     * Phase Voltage AB [V]
     */
    inline uint16_t phaseVoltageAB() const { return parse_uint16(5); }

    /**
     * Phase Voltage BC [V]
     */
    inline uint16_t phaseVoltageBC() const { return parse_uint16(6); }

    /**
     * Phase Voltage CA [V]
     */
    inline uint16_t phaseVoltageCA() const { return parse_uint16(7); }

    /**
     * Phase Voltage AN [V]
     */
    inline uint16_t phaseVoltageAN() const { return parse_uint16(8); }

    /**
     * Phase Voltage BN [V]
     */
    inline uint16_t phaseVoltageBN() const { return parse_uint16(9); }

    /**
     * Phase Voltage CN [V]
     */
    inline uint16_t phaseVoltageCN() const { return parse_uint16(10); }

    inline int16_t V_SF() const { return parse_sunssf(11); }

    /**
     * AC Power [W]
     */
    inline int16_t watts() const { return parse_int16(12); }

    inline int16_t W_SF() const { return parse_sunssf(13); }

    /**
     * Line Frequency [Hz]
     */
    inline uint16_t hz() const { return parse_uint16(14); }

    inline int16_t hz_SF() const { return parse_sunssf(15); }

    /**
     * AC Apparent Power [VA]
     */
    inline int16_t VA() const { return parse_int16(16); }

    inline int16_t VA_SF() const { return parse_sunssf(17); }

    /**
     * AC Reactive Power [var]
     */
    inline int16_t vAr() const { return parse_int16(18); }

    inline int16_t vAr_SF() const { return parse_sunssf(19); }

    /**
     * AC Power Factor [Pct]
     */
    inline int16_t PF() const { return parse_int16(20); }

    inline int16_t PF_SF() const { return parse_sunssf(21); }

    /**
     * AC Energy [Wh]
     */
    inline uint32_t wattHours() const { return parse_acc32(22); }

    inline int16_t WH_SF() const { return parse_sunssf(24); }

    /**
     * DC Current [A]
     */
    inline uint16_t DCAmps() const { return parse_uint16(25); }

    inline int16_t DCA_SF() const { return parse_sunssf(26); }

    /**
     * DC Voltage [V]
     */
    inline uint16_t DCVoltage() const { return parse_uint16(27); }

    inline int16_t DCV_SF() const { return parse_sunssf(28); }

    /**
     * DC Power [W]
     */
    inline int16_t DCWatts() const { return parse_int16(29); }

    inline int16_t DCW_SF() const { return parse_sunssf(30); }

    /**
     * Cabinet Temperature [C]
     */
    inline int16_t cabinetTemperature() const { return parse_int16(31); }

    /**
     * Heat Sink Temperature [C]
     */
    inline int16_t heatSinkTemperature() const { return parse_int16(32); }

    /**
     * Transformer Temperature [C]
     */
    inline int16_t transformerTemperature() const { return parse_int16(33); }

    /**
     * Other Temperature [C]
     */
    inline int16_t otherTemperature() const { return parse_int16(34); }

    inline int16_t tmp_SF() const { return parse_sunssf(35); }

    /**
     * Enumerated value.  Operating state
     */
    inline uint16_t operatingState() const { return parse_enum16(36); }

    /**
     * Vendor specific operating state code
     */
    inline uint16_t vendorOperatingState() const { return parse_enum16(37); }

    /**
     * Bitmask value. Event fields
     */
    inline uint32_t event1() const { return parse_bitfield32(38); }

    /**
     * Reserved for future use
     */
    inline uint32_t eventBitfield2() const { return parse_bitfield32(40); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield1() const { return parse_bitfield32(42); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield2() const { return parse_bitfield32(44); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield3() const { return parse_bitfield32(46); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield4() const { return parse_bitfield32(48); }

};

/**
 * Include this model for split phase inverter monitoring
 */
class InverterSplitPhase : public SunSpecModel<102, 50> {
  public:
    /**
     * AC Current [A]
     */
    inline uint16_t amps() const { return parse_uint16(0); }

    /**
     * Phase A Current [A]
     */
    inline uint16_t ampsPhaseA() const { return parse_uint16(1); }

    /**
     * Phase B Current [A]
     */
    inline uint16_t ampsPhaseB() const { return parse_uint16(2); }

    /**
     * Phase C Current [A]
     */
    inline uint16_t ampsPhaseC() const { return parse_uint16(3); }

    inline int16_t A_SF() const { return parse_sunssf(4); }

    /**
     * Phase Voltage AB [V]
     */
    inline uint16_t phaseVoltageAB() const { return parse_uint16(5); }

    /**
     * Phase Voltage BC [V]
     */
    inline uint16_t phaseVoltageBC() const { return parse_uint16(6); }

    /**
     * Phase Voltage CA [V]
     */
    inline uint16_t phaseVoltageCA() const { return parse_uint16(7); }

    /**
     * Phase Voltage AN [V]
     */
    inline uint16_t phaseVoltageAN() const { return parse_uint16(8); }

    /**
     * Phase Voltage BN [V]
     */
    inline uint16_t phaseVoltageBN() const { return parse_uint16(9); }

    /**
     * Phase Voltage CN [V]
     */
    inline uint16_t phaseVoltageCN() const { return parse_uint16(10); }

    inline int16_t V_SF() const { return parse_sunssf(11); }

    /**
     * AC Power [W]
     */
    inline int16_t watts() const { return parse_int16(12); }

    inline int16_t W_SF() const { return parse_sunssf(13); }

    /**
     * Line Frequency [Hz]
     */
    inline uint16_t hz() const { return parse_uint16(14); }

    inline int16_t hz_SF() const { return parse_sunssf(15); }

    /**
     * AC Apparent Power [VA]
     */
    inline int16_t VA() const { return parse_int16(16); }

    inline int16_t VA_SF() const { return parse_sunssf(17); }

    /**
     * AC Reactive Power [var]
     */
    inline int16_t vAr() const { return parse_int16(18); }

    inline int16_t vAr_SF() const { return parse_sunssf(19); }

    /**
     * AC Power Factor [Pct]
     */
    inline int16_t PF() const { return parse_int16(20); }

    inline int16_t PF_SF() const { return parse_sunssf(21); }

    /**
     * AC Energy [Wh]
     */
    inline uint32_t wattHours() const { return parse_acc32(22); }

    inline int16_t WH_SF() const { return parse_sunssf(24); }

    /**
     * DC Current [A]
     */
    inline uint16_t DCAmps() const { return parse_uint16(25); }

    inline int16_t DCA_SF() const { return parse_sunssf(26); }

    /**
     * DC Voltage [V]
     */
    inline uint16_t DCVoltage() const { return parse_uint16(27); }

    inline int16_t DCV_SF() const { return parse_sunssf(28); }

    /**
     * DC Power [W]
     */
    inline int16_t DCWatts() const { return parse_int16(29); }

    inline int16_t DCW_SF() const { return parse_sunssf(30); }

    /**
     * Cabinet Temperature [C]
     */
    inline int16_t cabinetTemperature() const { return parse_int16(31); }

    /**
     * Heat Sink Temperature [C]
     */
    inline int16_t heatSinkTemperature() const { return parse_int16(32); }

    /**
     * Transformer Temperature [C]
     */
    inline int16_t transformerTemperature() const { return parse_int16(33); }

    /**
     * Other Temperature [C]
     */
    inline int16_t otherTemperature() const { return parse_int16(34); }

    inline int16_t tmp_SF() const { return parse_sunssf(35); }

    /**
     * Enumerated value.  Operating state
     */
    inline uint16_t operatingState() const { return parse_enum16(36); }

    /**
     * Vendor specific operating state code
     */
    inline uint16_t vendorOperatingState() const { return parse_enum16(37); }

    /**
     * Bitmask value. Event fields
     */
    inline uint32_t event1() const { return parse_bitfield32(38); }

    /**
     * Reserved for future use
     */
    inline uint32_t eventBitfield2() const { return parse_bitfield32(40); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield1() const { return parse_bitfield32(42); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield2() const { return parse_bitfield32(44); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield3() const { return parse_bitfield32(46); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield4() const { return parse_bitfield32(48); }

};

/**
 * Include this model for three phase inverter monitoring
 */
class InverterThreePhase : public SunSpecModel<103, 50> {
  public:
    /**
     * AC Current [A]
     */
    inline uint16_t amps() const { return parse_uint16(0); }

    /**
     * Phase A Current [A]
     */
    inline uint16_t ampsPhaseA() const { return parse_uint16(1); }

    /**
     * Phase B Current [A]
     */
    inline uint16_t ampsPhaseB() const { return parse_uint16(2); }

    /**
     * Phase C Current [A]
     */
    inline uint16_t ampsPhaseC() const { return parse_uint16(3); }

    inline int16_t A_SF() const { return parse_sunssf(4); }

    /**
     * Phase Voltage AB [V]
     */
    inline uint16_t phaseVoltageAB() const { return parse_uint16(5); }

    /**
     * Phase Voltage BC [V]
     */
    inline uint16_t phaseVoltageBC() const { return parse_uint16(6); }

    /**
     * Phase Voltage CA [V]
     */
    inline uint16_t phaseVoltageCA() const { return parse_uint16(7); }

    /**
     * Phase Voltage AN [V]
     */
    inline uint16_t phaseVoltageAN() const { return parse_uint16(8); }

    /**
     * Phase Voltage BN [V]
     */
    inline uint16_t phaseVoltageBN() const { return parse_uint16(9); }

    /**
     * Phase Voltage CN [V]
     */
    inline uint16_t phaseVoltageCN() const { return parse_uint16(10); }

    inline int16_t V_SF() const { return parse_sunssf(11); }

    /**
     * AC Power [W]
     */
    inline int16_t watts() const { return parse_int16(12); }

    inline int16_t W_SF() const { return parse_sunssf(13); }

    /**
     * Line Frequency [Hz]
     */
    inline uint16_t hz() const { return parse_uint16(14); }

    inline int16_t hz_SF() const { return parse_sunssf(15); }

    /**
     * AC Apparent Power [VA]
     */
    inline int16_t VA() const { return parse_int16(16); }

    inline int16_t VA_SF() const { return parse_sunssf(17); }

    /**
     * AC Reactive Power [var]
     */
    inline int16_t vAr() const { return parse_int16(18); }

    inline int16_t vAr_SF() const { return parse_sunssf(19); }

    /**
     * AC Power Factor [Pct]
     */
    inline int16_t PF() const { return parse_int16(20); }

    inline int16_t PF_SF() const { return parse_sunssf(21); }

    /**
     * AC Energy [Wh]
     */
    inline uint32_t wattHours() const { return parse_acc32(22); }

    inline int16_t WH_SF() const { return parse_sunssf(24); }

    /**
     * DC Current [A]
     */
    inline uint16_t DCAmps() const { return parse_uint16(25); }

    inline int16_t DCA_SF() const { return parse_sunssf(26); }

    /**
     * DC Voltage [V]
     */
    inline uint16_t DCVoltage() const { return parse_uint16(27); }

    inline int16_t DCV_SF() const { return parse_sunssf(28); }

    /**
     * DC Power [W]
     */
    inline int16_t DCWatts() const { return parse_int16(29); }

    inline int16_t DCW_SF() const { return parse_sunssf(30); }

    /**
     * Cabinet Temperature [C]
     */
    inline int16_t cabinetTemperature() const { return parse_int16(31); }

    /**
     * Heat Sink Temperature [C]
     */
    inline int16_t heatSinkTemperature() const { return parse_int16(32); }

    /**
     * Transformer Temperature [C]
     */
    inline int16_t transformerTemperature() const { return parse_int16(33); }

    /**
     * Other Temperature [C]
     */
    inline int16_t otherTemperature() const { return parse_int16(34); }

    inline int16_t tmp_SF() const { return parse_sunssf(35); }

    /**
     * Enumerated value.  Operating state
     */
    inline uint16_t operatingState() const { return parse_enum16(36); }

    /**
     * Vendor specific operating state code
     */
    inline uint16_t vendorOperatingState() const { return parse_enum16(37); }

    /**
     * Bitmask value. Event fields
     */
    inline uint32_t event1() const { return parse_bitfield32(38); }

    /**
     * Reserved for future use
     */
    inline uint32_t eventBitfield2() const { return parse_bitfield32(40); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield1() const { return parse_bitfield32(42); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield2() const { return parse_bitfield32(44); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield3() const { return parse_bitfield32(46); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield4() const { return parse_bitfield32(48); }

};

/**
 * Include this model for single phase inverter monitoring using float values
 */
class InverterSinglePhaseFLOAT : public SunSpecModel<111, 60> {
  public:
    /**
     * AC Current [A]
     */
    inline float amps() const { return parse_float32(0); }

    /**
     * Phase A Current [A]
     */
    inline float ampsPhaseA() const { return parse_float32(2); }

    /**
     * Phase B Current [A]
     */
    inline float ampsPhaseB() const { return parse_float32(4); }

    /**
     * Phase C Current [A]
     */
    inline float ampsPhaseC() const { return parse_float32(6); }

    /**
     * Phase Voltage AB [V]
     */
    inline float phaseVoltageAB() const { return parse_float32(8); }

    /**
     * Phase Voltage BC [V]
     */
    inline float phaseVoltageBC() const { return parse_float32(10); }

    /**
     * Phase Voltage CA [V]
     */
    inline float phaseVoltageCA() const { return parse_float32(12); }

    /**
     * Phase Voltage AN [V]
     */
    inline float phaseVoltageAN() const { return parse_float32(14); }

    /**
     * Phase Voltage BN [V]
     */
    inline float phaseVoltageBN() const { return parse_float32(16); }

    /**
     * Phase Voltage CN [V]
     */
    inline float phaseVoltageCN() const { return parse_float32(18); }

    /**
     * AC Power [W]
     */
    inline float watts() const { return parse_float32(20); }

    /**
     * Line Frequency [Hz]
     */
    inline float hz() const { return parse_float32(22); }

    /**
     * AC Apparent Power [VA]
     */
    inline float VA() const { return parse_float32(24); }

    /**
     * AC Reactive Power [var]
     */
    inline float vAr() const { return parse_float32(26); }

    /**
     * AC Power Factor [Pct]
     */
    inline float PF() const { return parse_float32(28); }

    /**
     * AC Energy [Wh]
     */
    inline float wattHours() const { return parse_float32(30); }

    /**
     * DC Current [A]
     */
    inline float DCAmps() const { return parse_float32(32); }

    /**
     * DC Voltage [V]
     */
    inline float DCVoltage() const { return parse_float32(34); }

    /**
     * DC Power [W]
     */
    inline float DCWatts() const { return parse_float32(36); }

    /**
     * Cabinet Temperature [C]
     */
    inline float cabinetTemperature() const { return parse_float32(38); }

    /**
     * Heat Sink Temperature [C]
     */
    inline float heatSinkTemperature() const { return parse_float32(40); }

    /**
     * Transformer Temperature [C]
     */
    inline float transformerTemperature() const { return parse_float32(42); }

    /**
     * Other Temperature [C]
     */
    inline float otherTemperature() const { return parse_float32(44); }

    /**
     * Enumerated value.  Operating state
     */
    inline uint16_t operatingState() const { return parse_enum16(46); }

    /**
     * Vendor specific operating state code
     */
    inline uint16_t vendorOperatingState() const { return parse_enum16(47); }

    /**
     * Bitmask value. Event fields
     */
    inline uint32_t event1() const { return parse_bitfield32(48); }

    /**
     * Reserved for future use
     */
    inline uint32_t eventBitfield2() const { return parse_bitfield32(50); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield1() const { return parse_bitfield32(52); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield2() const { return parse_bitfield32(54); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield3() const { return parse_bitfield32(56); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield4() const { return parse_bitfield32(58); }

};

/**
 * Include this model for split phase inverter monitoring using float values
 */
class InverterSplitPhaseFLOAT : public SunSpecModel<112, 60> {
  public:
    /**
     * AC Current [A]
     */
    inline float amps() const { return parse_float32(0); }

    /**
     * Phase A Current [A]
     */
    inline float ampsPhaseA() const { return parse_float32(2); }

    /**
     * Phase B Current [A]
     */
    inline float ampsPhaseB() const { return parse_float32(4); }

    /**
     * Phase C Current [A]
     */
    inline float ampsPhaseC() const { return parse_float32(6); }

    /**
     * Phase Voltage AB [V]
     */
    inline float phaseVoltageAB() const { return parse_float32(8); }

    /**
     * Phase Voltage BC [V]
     */
    inline float phaseVoltageBC() const { return parse_float32(10); }

    /**
     * Phase Voltage CA [V]
     */
    inline float phaseVoltageCA() const { return parse_float32(12); }

    /**
     * Phase Voltage AN [V]
     */
    inline float phaseVoltageAN() const { return parse_float32(14); }

    /**
     * Phase Voltage BN [V]
     */
    inline float phaseVoltageBN() const { return parse_float32(16); }

    /**
     * Phase Voltage CN [V]
     */
    inline float phaseVoltageCN() const { return parse_float32(18); }

    /**
     * AC Power [W]
     */
    inline float watts() const { return parse_float32(20); }

    /**
     * Line Frequency [Hz]
     */
    inline float hz() const { return parse_float32(22); }

    /**
     * AC Apparent Power [VA]
     */
    inline float VA() const { return parse_float32(24); }

    /**
     * AC Reactive Power [var]
     */
    inline float vAr() const { return parse_float32(26); }

    /**
     * AC Power Factor [Pct]
     */
    inline float PF() const { return parse_float32(28); }

    /**
     * AC Energy [Wh]
     */
    inline float wattHours() const { return parse_float32(30); }

    /**
     * DC Current [A]
     */
    inline float DCAmps() const { return parse_float32(32); }

    /**
     * DC Voltage [V]
     */
    inline float DCVoltage() const { return parse_float32(34); }

    /**
     * DC Power [W]
     */
    inline float DCWatts() const { return parse_float32(36); }

    /**
     * Cabinet Temperature [C]
     */
    inline float cabinetTemperature() const { return parse_float32(38); }

    /**
     * Heat Sink Temperature [C]
     */
    inline float heatSinkTemperature() const { return parse_float32(40); }

    /**
     * Transformer Temperature [C]
     */
    inline float transformerTemperature() const { return parse_float32(42); }

    /**
     * Other Temperature [C]
     */
    inline float otherTemperature() const { return parse_float32(44); }

    /**
     * Enumerated value.  Operating state
     */
    inline uint16_t operatingState() const { return parse_enum16(46); }

    /**
     * Vendor specific operating state code
     */
    inline uint16_t vendorOperatingState() const { return parse_enum16(47); }

    /**
     * Bitmask value. Event fields
     */
    inline uint32_t event1() const { return parse_bitfield32(48); }

    /**
     * Reserved for future use
     */
    inline uint32_t eventBitfield2() const { return parse_bitfield32(50); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield1() const { return parse_bitfield32(52); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield2() const { return parse_bitfield32(54); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield3() const { return parse_bitfield32(56); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield4() const { return parse_bitfield32(58); }

};

/**
 * Include this model for three phase inverter monitoring using float values
 */
class InverterThreePhaseFLOAT : public SunSpecModel<113, 60> {
  public:
    /**
     * AC Current [A]
     */
    inline float amps() const { return parse_float32(0); }

    /**
     * Phase A Current [A]
     */
    inline float ampsPhaseA() const { return parse_float32(2); }

    /**
     * Phase B Current [A]
     */
    inline float ampsPhaseB() const { return parse_float32(4); }

    /**
     * Phase C Current [A]
     */
    inline float ampsPhaseC() const { return parse_float32(6); }

    /**
     * Phase Voltage AB [V]
     */
    inline float phaseVoltageAB() const { return parse_float32(8); }

    /**
     * Phase Voltage BC [V]
     */
    inline float phaseVoltageBC() const { return parse_float32(10); }

    /**
     * Phase Voltage CA [V]
     */
    inline float phaseVoltageCA() const { return parse_float32(12); }

    /**
     * Phase Voltage AN [V]
     */
    inline float phaseVoltageAN() const { return parse_float32(14); }

    /**
     * Phase Voltage BN [V]
     */
    inline float phaseVoltageBN() const { return parse_float32(16); }

    /**
     * Phase Voltage CN [V]
     */
    inline float phaseVoltageCN() const { return parse_float32(18); }

    /**
     * AC Power [W]
     */
    inline float watts() const { return parse_float32(20); }

    /**
     * Line Frequency [Hz]
     */
    inline float hz() const { return parse_float32(22); }

    /**
     * AC Apparent Power [VA]
     */
    inline float VA() const { return parse_float32(24); }

    /**
     * AC Reactive Power [var]
     */
    inline float vAr() const { return parse_float32(26); }

    /**
     * AC Power Factor [Pct]
     */
    inline float PF() const { return parse_float32(28); }

    /**
     * AC Energy [Wh]
     */
    inline float wattHours() const { return parse_float32(30); }

    /**
     * DC Current [A]
     */
    inline float DCAmps() const { return parse_float32(32); }

    /**
     * DC Voltage [V]
     */
    inline float DCVoltage() const { return parse_float32(34); }

    /**
     * DC Power [W]
     */
    inline float DCWatts() const { return parse_float32(36); }

    /**
     * Cabinet Temperature [C]
     */
    inline float cabinetTemperature() const { return parse_float32(38); }

    /**
     * Heat Sink Temperature [C]
     */
    inline float heatSinkTemperature() const { return parse_float32(40); }

    /**
     * Transformer Temperature [C]
     */
    inline float transformerTemperature() const { return parse_float32(42); }

    /**
     * Other Temperature [C]
     */
    inline float otherTemperature() const { return parse_float32(44); }

    /**
     * Enumerated value.  Operating state
     */
    inline uint16_t operatingState() const { return parse_enum16(46); }

    /**
     * Vendor specific operating state code
     */
    inline uint16_t vendorOperatingState() const { return parse_enum16(47); }

    /**
     * Bitmask value. Event fields
     */
    inline uint32_t event1() const { return parse_bitfield32(48); }

    /**
     * Reserved for future use
     */
    inline uint32_t eventBitfield2() const { return parse_bitfield32(50); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield1() const { return parse_bitfield32(52); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield2() const { return parse_bitfield32(54); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield3() const { return parse_bitfield32(56); }

    /**
     * Vendor defined events
     */
    inline uint32_t vendorEventBitfield4() const { return parse_bitfield32(58); }

};

/**
 * Inverter Controls Nameplate Ratings 
 */
class Nameplate : public SunSpecModel<120, 26> {
  public:
    /**
     * Type of DER device. Default value is 4 to indicate PV device.
     */
    inline uint16_t dERTyp() const { return parse_enum16(0); }

    /**
     * Continuous power output capability of the inverter. [W]
     */
    inline uint16_t wRtg() const { return parse_uint16(1); }

    /**
     * Scale factor
     */
    inline int16_t wRtg_SF() const { return parse_sunssf(2); }

    /**
     * Continuous Volt-Ampere capability of the inverter. [VA]
     */
    inline uint16_t vARtg() const { return parse_uint16(3); }

    /**
     * Scale factor
     */
    inline int16_t vARtg_SF() const { return parse_sunssf(4); }

    /**
     * Continuous VAR capability of the inverter in quadrant 1. [var]
     */
    inline int16_t vArRtgQ1() const { return parse_int16(5); }

    /**
     * Continuous VAR capability of the inverter in quadrant 2. [var]
     */
    inline int16_t vArRtgQ2() const { return parse_int16(6); }

    /**
     * Continuous VAR capability of the inverter in quadrant 3. [var]
     */
    inline int16_t vArRtgQ3() const { return parse_int16(7); }

    /**
     * Continuous VAR capability of the inverter in quadrant 4. [var]
     */
    inline int16_t vArRtgQ4() const { return parse_int16(8); }

    /**
     * Scale factor
     */
    inline int16_t vArRtg_SF() const { return parse_sunssf(9); }

    /**
     * Maximum RMS AC current level capability of the inverter. [A]
     */
    inline uint16_t aRtg() const { return parse_uint16(10); }

    /**
     * Scale factor
     */
    inline int16_t aRtg_SF() const { return parse_sunssf(11); }

    /**
     * Minimum power factor capability of the inverter in quadrant 1. [cos()]
     */
    inline int16_t pFRtgQ1() const { return parse_int16(12); }

    /**
     * Minimum power factor capability of the inverter in quadrant 2. [cos()]
     */
    inline int16_t pFRtgQ2() const { return parse_int16(13); }

    /**
     * Minimum power factor capability of the inverter in quadrant 3. [cos()]
     */
    inline int16_t pFRtgQ3() const { return parse_int16(14); }

    /**
     * Minimum power factor capability of the inverter in quadrant 4. [cos()]
     */
    inline int16_t pFRtgQ4() const { return parse_int16(15); }

    /**
     * Scale factor
     */
    inline int16_t pFRtg_SF() const { return parse_sunssf(16); }

    /**
     * Nominal energy rating of storage device. [Wh]
     */
    inline uint16_t wHRtg() const { return parse_uint16(17); }

    /**
     * Scale factor
     */
    inline int16_t wHRtg_SF() const { return parse_sunssf(18); }

    /**
     * The usable capacity of the battery.  Maximum charge minus minimum charge from a technology capability perspective (Amp-hour capacity rating). [AH]
     */
    inline uint16_t ahrRtg() const { return parse_uint16(19); }

    /**
     * Scale factor for amp-hour rating.
     */
    inline int16_t ahrRtg_SF() const { return parse_sunssf(20); }

    /**
     * Maximum rate of energy transfer into the storage device. [W]
     */
    inline uint16_t maxChaRte() const { return parse_uint16(21); }

    /**
     * Scale factor
     */
    inline int16_t maxChaRte_SF() const { return parse_sunssf(22); }

    /**
     * Maximum rate of energy transfer out of the storage device. [W]
     */
    inline uint16_t maxDisChaRte() const { return parse_uint16(23); }

    /**
     * Scale factor
     */
    inline int16_t maxDisChaRte_SF() const { return parse_sunssf(24); }

};

/**
 * Inverter Controls Basic Settings 
 */
class BasicSettings : public SunSpecModel<121, 30> {
  public:
    /**
     * Setting for maximum power output. Default to WRtg. [W]
     */
    inline uint16_t wMax() const { return parse_uint16(0); }

    /**
     * Voltage at the PCC. [V]
     */
    inline uint16_t vRef() const { return parse_uint16(1); }

    /**
     * Offset  from PCC to inverter. [V]
     */
    inline int16_t vRefOfs() const { return parse_int16(2); }

    /**
     * Setpoint for maximum voltage. [V]
     */
    inline uint16_t vMax() const { return parse_uint16(3); }

    /**
     * Setpoint for minimum voltage. [V]
     */
    inline uint16_t vMin() const { return parse_uint16(4); }

    /**
     * Setpoint for maximum apparent power. Default to VARtg. [VA]
     */
    inline uint16_t vAMax() const { return parse_uint16(5); }

    /**
     * Setting for maximum reactive power in quadrant 1. Default to VArRtgQ1. [var]
     */
    inline int16_t vArMaxQ1() const { return parse_int16(6); }

    /**
     * Setting for maximum reactive power in quadrant 2. Default to VArRtgQ2. [var]
     */
    inline int16_t vArMaxQ2() const { return parse_int16(7); }

    /**
     * Setting for maximum reactive power in quadrant 3. Default to VArRtgQ3. [var]
     */
    inline int16_t vArMaxQ3() const { return parse_int16(8); }

    /**
     * Setting for maximum reactive power in quadrant 4. Default to VArRtgQ4. [var]
     */
    inline int16_t vArMaxQ4() const { return parse_int16(9); }

    /**
     * Default ramp rate of change of active power due to command or internal action. [% WMax/sec]
     */
    inline uint16_t wGra() const { return parse_uint16(10); }

    /**
     * Setpoint for minimum power factor value in quadrant 1. Default to PFRtgQ1. [cos()]
     */
    inline int16_t pFMinQ1() const { return parse_int16(11); }

    /**
     * Setpoint for minimum power factor value in quadrant 2. Default to PFRtgQ2. [cos()]
     */
    inline int16_t pFMinQ2() const { return parse_int16(12); }

    /**
     * Setpoint for minimum power factor value in quadrant 3. Default to PFRtgQ3. [cos()]
     */
    inline int16_t pFMinQ3() const { return parse_int16(13); }

    /**
     * Setpoint for minimum power factor value in quadrant 4. Default to PFRtgQ4. [cos()]
     */
    inline int16_t pFMinQ4() const { return parse_int16(14); }

    /**
     * VAR action on change between charging and discharging: 1=switch 2=maintain VAR characterization.
     */
    inline uint16_t vArAct() const { return parse_enum16(15); }

    /**
     * Calculation method for total apparent power. 1=vector 2=arithmetic.
     */
    inline uint16_t clcTotVA() const { return parse_enum16(16); }

    /**
     * Setpoint for maximum ramp rate as percentage of nominal maximum ramp rate. This setting will limit the rate that watts delivery to the grid can increase or decrease in response to intermittent PV generation. [% WGra]
     */
    inline uint16_t maxRmpRte() const { return parse_uint16(17); }

    /**
     * Setpoint for nominal frequency at the ECP. [Hz]
     */
    inline uint16_t eCPNomHz() const { return parse_uint16(18); }

    /**
     * Identity of connected phase for single phase inverters. A=1 B=2 C=3.
     */
    inline uint16_t connPh() const { return parse_enum16(19); }

    /**
     * Scale factor for real power.
     */
    inline int16_t wMax_SF() const { return parse_sunssf(20); }

    /**
     * Scale factor for voltage at the PCC.
     */
    inline int16_t vRef_SF() const { return parse_sunssf(21); }

    /**
     * Scale factor for offset voltage.
     */
    inline int16_t vRefOfs_SF() const { return parse_sunssf(22); }

    /**
     * Scale factor for min/max voltages.
     */
    inline int16_t vMinMax_SF() const { return parse_sunssf(23); }

    /**
     * Scale factor for apparent power.
     */
    inline int16_t vAMax_SF() const { return parse_sunssf(24); }

    /**
     * Scale factor for reactive power.
     */
    inline int16_t vArMax_SF() const { return parse_sunssf(25); }

    /**
     * Scale factor for default ramp rate.
     */
    inline int16_t wGra_SF() const { return parse_sunssf(26); }

    /**
     * Scale factor for minimum power factor.
     */
    inline int16_t pFMin_SF() const { return parse_sunssf(27); }

    /**
     * Scale factor for maximum ramp percentage.
     */
    inline int16_t maxRmpRte_SF() const { return parse_sunssf(28); }

    /**
     * Scale factor for nominal frequency.
     */
    inline int16_t eCPNomHz_SF() const { return parse_sunssf(29); }

};

/**
 * Inverter Controls Extended Measurements and Status 
 */
class Measurements_Status : public SunSpecModel<122, 44> {
  public:
    /**
     * PV inverter present/available status. Enumerated value.
     */
    inline uint16_t pVConn() const { return parse_bitfield16(0); }

    /**
     * Storage inverter present/available status. Enumerated value.
     */
    inline uint16_t storConn() const { return parse_bitfield16(1); }

    /**
     * ECP connection status: disconnected=0  connected=1.
     */
    inline uint16_t eCPConn() const { return parse_bitfield16(2); }

    /**
     * AC lifetime active (real) energy output. [Wh]
     */
    inline uint64_t actWh() const { return parse_acc64(3); }

    /**
     * AC lifetime apparent energy output. [VAh]
     */
    inline uint64_t actVAh() const { return parse_acc64(7); }

    /**
     * AC lifetime reactive energy output in quadrant 1. [varh]
     */
    inline uint64_t actVArhQ1() const { return parse_acc64(11); }

    /**
     * AC lifetime reactive energy output in quadrant 2. [varh]
     */
    inline uint64_t actVArhQ2() const { return parse_acc64(15); }

    /**
     * AC lifetime negative energy output  in quadrant 3. [varh]
     */
    inline uint64_t actVArhQ3() const { return parse_acc64(19); }

    /**
     * AC lifetime reactive energy output  in quadrant 4. [varh]
     */
    inline uint64_t actVArhQ4() const { return parse_acc64(23); }

    /**
     * Amount of VARs available without impacting watts output. [var]
     */
    inline int16_t vArAval() const { return parse_int16(27); }

    /**
     * Scale factor for available VARs.
     */
    inline int16_t vArAval_SF() const { return parse_sunssf(28); }

    /**
     * Amount of Watts available. [var]
     */
    inline uint16_t wAval() const { return parse_uint16(29); }

    /**
     * Scale factor for available Watts.
     */
    inline int16_t wAval_SF() const { return parse_sunssf(30); }

    /**
     * Bit Mask indicating setpoint limit(s) reached.
     */
    inline uint32_t stSetLimMsk() const { return parse_bitfield32(31); }

    /**
     * Bit Mask indicating which inverter controls are currently active.
     */
    inline uint32_t stActCtl() const { return parse_bitfield32(33); }

    /**
     * Source of time synchronization.
     */
    inline String tmSrc() const { return parse_string(35, 4); }

    /**
     * Seconds since 01-01-2000 00:00 UTC [Secs]
     */
    inline uint32_t tms() const { return parse_uint32(39); }

    /**
     * Bit Mask indicating active ride-through status.
     */
    inline uint16_t rtSt() const { return parse_bitfield16(41); }

    /**
     * Isolation resistance. [ohms]
     */
    inline uint16_t ris() const { return parse_uint16(42); }

    /**
     * Scale factor for isolation resistance.
     */
    inline int16_t ris_SF() const { return parse_sunssf(43); }

};

/**
 * Immediate Inverter Controls 
 */
class ImmediateControls : public SunSpecModel<123, 24> {
  public:
    /**
     * Time window for connect/disconnect. [Secs]
     */
    inline uint16_t conn_WinTms() const { return parse_uint16(0); }

    /**
     * Timeout period for connect/disconnect. [Secs]
     */
    inline uint16_t conn_RvrtTms() const { return parse_uint16(1); }

    /**
     * Enumerated valued.  Connection control.
     */
    inline uint16_t conn() const { return parse_enum16(2); }

    /**
     * Set power output to specified level. [% WMax]
     */
    inline uint16_t wMaxLimPct() const { return parse_uint16(3); }

    /**
     * Time window for power limit change. [Secs]
     */
    inline uint16_t wMaxLimPct_WinTms() const { return parse_uint16(4); }

    /**
     * Timeout period for power limit. [Secs]
     */
    inline uint16_t wMaxLimPct_RvrtTms() const { return parse_uint16(5); }

    /**
     * Ramp time for moving from current setpoint to new setpoint. [Secs]
     */
    inline uint16_t wMaxLimPct_RmpTms() const { return parse_uint16(6); }

    /**
     * Enumerated valued.  Throttle enable/disable control.
     */
    inline uint16_t wMaxLim_Ena() const { return parse_enum16(7); }

    /**
     * Set power factor to specific value - cosine of angle. [cos()]
     */
    inline int16_t outPFSet() const { return parse_int16(8); }

    /**
     * Time window for power factor change. [Secs]
     */
    inline uint16_t outPFSet_WinTms() const { return parse_uint16(9); }

    /**
     * Timeout period for power factor. [Secs]
     */
    inline uint16_t outPFSet_RvrtTms() const { return parse_uint16(10); }

    /**
     * Ramp time for moving from current setpoint to new setpoint. [Secs]
     */
    inline uint16_t outPFSet_RmpTms() const { return parse_uint16(11); }

    /**
     * Enumerated valued.  Fixed power factor enable/disable control.
     */
    inline uint16_t outPFSet_Ena() const { return parse_enum16(12); }

    /**
     * Reactive power in percent of WMax. [% WMax]
     */
    inline int16_t vArWMaxPct() const { return parse_int16(13); }

    /**
     * Reactive power in percent of VArMax. [% VArMax]
     */
    inline int16_t vArMaxPct() const { return parse_int16(14); }

    /**
     * Reactive power in percent of VArAval. [% VArAval]
     */
    inline int16_t vArAvalPct() const { return parse_int16(15); }

    /**
     * Time window for VAR limit change. [Secs]
     */
    inline uint16_t vArPct_WinTms() const { return parse_uint16(16); }

    /**
     * Timeout period for VAR limit. [Secs]
     */
    inline uint16_t vArPct_RvrtTms() const { return parse_uint16(17); }

    /**
     * Ramp time for moving from current setpoint to new setpoint. [Secs]
     */
    inline uint16_t vArPct_RmpTms() const { return parse_uint16(18); }

    /**
     * Enumerated value. VAR percent limit mode.
     */
    inline uint16_t vArPct_Mod() const { return parse_enum16(19); }

    /**
     * Enumerated valued.  Percent limit VAr enable/disable control.
     */
    inline uint16_t vArPct_Ena() const { return parse_enum16(20); }

    /**
     * Scale factor for power output percent.
     */
    inline int16_t wMaxLimPct_SF() const { return parse_sunssf(21); }

    /**
     * Scale factor for power factor.
     */
    inline int16_t outPFSet_SF() const { return parse_sunssf(22); }

    /**
     * Scale factor for reactive power percent.
     */
    inline int16_t vArPct_SF() const { return parse_sunssf(23); }

};

/**
 * Basic Storage Controls 
 */
class Storage : public SunSpecModel<124, 24> {
  public:
    /**
     * Setpoint for maximum charge. [W]
     */
    inline uint16_t wChaMax() const { return parse_uint16(0); }

    /**
     * Setpoint for maximum charging rate. Default is MaxChaRte. [% WChaMax/sec]
     */
    inline uint16_t wChaGra() const { return parse_uint16(1); }

    /**
     * Setpoint for maximum discharge rate. Default is MaxDisChaRte. [% WChaMax/sec]
     */
    inline uint16_t wDisChaGra() const { return parse_uint16(2); }

    /**
     * Activate hold/discharge/charge storage control mode. Bitfield value.
     */
    inline uint16_t storCtl_Mod() const { return parse_bitfield16(3); }

    /**
     * Setpoint for maximum charging VA. [VA]
     */
    inline uint16_t vAChaMax() const { return parse_uint16(4); }

    /**
     * Setpoint for minimum reserve for storage as a percentage of the nominal maximum storage. [% WChaMax]
     */
    inline uint16_t minRsvPct() const { return parse_uint16(5); }

    /**
     * Currently available energy as a percent of the capacity rating. [% AhrRtg]
     */
    inline uint16_t chaState() const { return parse_uint16(6); }

    /**
     * State of charge (ChaState) minus storage reserve (MinRsvPct) times capacity rating (AhrRtg). [AH]
     */
    inline uint16_t storAval() const { return parse_uint16(7); }

    /**
     * Internal battery voltage. [V]
     */
    inline uint16_t inBatV() const { return parse_uint16(8); }

    /**
     * Charge status of storage device. Enumerated value.
     */
    inline uint16_t chaSt() const { return parse_enum16(9); }

    /**
     * Percent of max discharge rate. [% WDisChaMax]
     */
    inline int16_t outWRte() const { return parse_int16(10); }

    /**
     * Percent of max charging rate. [ % WChaMax]
     */
    inline int16_t inWRte() const { return parse_int16(11); }

    /**
     * Time window for charge/discharge rate change. [Secs]
     */
    inline uint16_t inOutWRte_WinTms() const { return parse_uint16(12); }

    /**
     * Timeout period for charge/discharge rate. [Secs]
     */
    inline uint16_t inOutWRte_RvrtTms() const { return parse_uint16(13); }

    /**
     * Ramp time for moving from current setpoint to new setpoint. [Secs]
     */
    inline uint16_t inOutWRte_RmpTms() const { return parse_uint16(14); }

    inline uint16_t chaGriSet() const { return parse_enum16(15); }

    /**
     * Scale factor for maximum charge.
     */
    inline int16_t wChaMax_SF() const { return parse_sunssf(16); }

    /**
     * Scale factor for maximum charge and discharge rate.
     */
    inline int16_t wChaDisChaGra_SF() const { return parse_sunssf(17); }

    /**
     * Scale factor for maximum charging VA.
     */
    inline int16_t vAChaMax_SF() const { return parse_sunssf(18); }

    /**
     * Scale factor for minimum reserve percentage.
     */
    inline int16_t minRsvPct_SF() const { return parse_sunssf(19); }

    /**
     * Scale factor for available energy percent.
     */
    inline int16_t chaState_SF() const { return parse_sunssf(20); }

    /**
     * Scale factor for state of charge.
     */
    inline int16_t storAval_SF() const { return parse_sunssf(21); }

    /**
     * Scale factor for battery voltage.
     */
    inline int16_t inBatV_SF() const { return parse_sunssf(22); }

    /**
     * Scale factor for percent charge/discharge rate.
     */
    inline int16_t inOutWRte_SF() const { return parse_sunssf(23); }

};

/**
 * Pricing Signal  
 */
class Pricing : public SunSpecModel<125, 8> {
  public:
    /**
     * Is price-based charge/discharge mode active?
     */
    inline uint16_t modEna() const { return parse_bitfield16(0); }

    /**
     * Meaning of the pricing signal. When a Price schedule is used, type must match the schedule range variable description.
     */
    inline uint16_t sigType() const { return parse_enum16(1); }

    /**
     * Utility/ESP specific pricing signal. Content depends on pricing signal type. When H/M/L type is specified. Low=0; Med=1; High=2.
     */
    inline int16_t sig() const { return parse_int16(2); }

    /**
     * Time window for charge/discharge pricing change. [Secs]
     */
    inline uint16_t winTms() const { return parse_uint16(3); }

    /**
     * Timeout period for charge/discharge pricing change. [Secs]
     */
    inline uint16_t rvtTms() const { return parse_uint16(4); }

    /**
     * Ramp time for moving from current charge or discharge level to new level. [Secs]
     */
    inline uint16_t rmpTms() const { return parse_uint16(5); }

    /**
     * Pricing signal scale factor.
     */
    inline int16_t sig_SF() const { return parse_sunssf(6); }

};

/**
 * Parameterized Frequency-Watt 
 */
class FreqWattParam : public SunSpecModel<127, 10> {
  public:
    /**
     * The slope of the reduction in the maximum allowed watts output as a function of frequency. [% PM/Hz]
     */
    inline uint16_t wGra() const { return parse_uint16(0); }

    /**
     * The frequency deviation from nominal frequency (ECPNomHz) at which a snapshot of the instantaneous power output is taken to act as the CAPPED power level (PM) and above which reduction in power output occurs. [Hz]
     */
    inline int16_t hzStr() const { return parse_int16(1); }

    /**
     * The frequency deviation from nominal frequency (ECPNomHz) at which curtailed power output may return to normal and the cap on the power level value is removed. [Hz]
     */
    inline int16_t hzStop() const { return parse_int16(2); }

    /**
     * Enable hysteresis
     */
    inline uint16_t hysEna() const { return parse_bitfield16(3); }

    /**
     * Is Parameterized Frequency-Watt control active.
     */
    inline uint16_t modEna() const { return parse_bitfield16(4); }

    /**
     * The maximum time-based rate of change at which power output returns to normal after having been capped by an over frequency event. [% WMax/min]
     */
    inline uint16_t hzStopWGra() const { return parse_uint16(5); }

    /**
     * Scale factor for output gradient.
     */
    inline int16_t wGra_SF() const { return parse_sunssf(6); }

    /**
     * Scale factor for frequency deviations.
     */
    inline int16_t hzStrStop_SF() const { return parse_sunssf(7); }

    /**
     * Scale factor for increment and decrement ramps.
     */
    inline int16_t rmpIncDec_SF() const { return parse_sunssf(8); }

};

/**
 * Dynamic Reactive Current 
 */
class DynamicReactiveCurrent : public SunSpecModel<128, 14> {
  public:
    /**
     * Indicates if gradients trend toward zero at the edges of the deadband or trend toward zero at the center of the deadband.
     */
    inline uint16_t arGraMod() const { return parse_enum16(0); }

    /**
     * The gradient used to increase capacitive dynamic current. A value of 0 indicates no additional reactive current support. [%ARtg/%dV]
     */
    inline uint16_t arGraSag() const { return parse_uint16(1); }

    /**
     * The gradient used to increase inductive dynamic current.  A value of 0 indicates no additional reactive current support. [%ARtg/%dV]
     */
    inline uint16_t arGraSwell() const { return parse_uint16(2); }

    /**
     * Activate dynamic reactive current model
     */
    inline uint16_t modEna() const { return parse_bitfield16(3); }

    /**
     * The time window used to calculate the moving average voltage. [Secs]
     */
    inline uint16_t filTms() const { return parse_uint16(4); }

    /**
     * The lower delta voltage limit for which negative voltage deviations less than this value no dynamic vars are produced. [% VRef]
     */
    inline uint16_t dbVMin() const { return parse_uint16(5); }

    /**
     * The upper delta voltage limit for which positive voltage deviations less than this value no dynamic current produced. [% VRef]
     */
    inline uint16_t dbVMax() const { return parse_uint16(6); }

    /**
     * Block zone voltage which defines a lower voltage boundary below which no dynamic current is produced. [% VRef]
     */
    inline uint16_t blkZnV() const { return parse_uint16(7); }

    /**
     * Hysteresis voltage used with BlkZnV. [% VRef]
     */
    inline uint16_t hysBlkZnV() const { return parse_uint16(8); }

    /**
     * Block zone time the time before which reactive current support remains active regardless of how low the voltage drops. [mSecs]
     */
    inline uint16_t blkZnTmms() const { return parse_uint16(9); }

    /**
     * Hold time during which reactive current support continues after the average voltage has entered the dead zone. [mSecs]
     */
    inline uint16_t holdTmms() const { return parse_uint16(10); }

    /**
     * Scale factor for the gradients.
     */
    inline int16_t arGra_SF() const { return parse_sunssf(11); }

    /**
     * Scale factor for the voltage zone and limit settings.
     */
    inline int16_t vRefPct_SF() const { return parse_sunssf(12); }

};

/**
 * Inverter controls extended settings 
 */
class ExtendedSettings : public SunSpecModel<145, 8> {
  public:
    /**
     * Ramp up rate as a percentage of max current. [Pct]
     */
    inline uint16_t rampUpRate() const { return parse_uint16(0); }

    /**
     * Ramp down rate as a percentage of max current. [Pct]
     */
    inline uint16_t nomRmpDnRte() const { return parse_uint16(1); }

    /**
     * Emergency ramp up rate as a percentage of max current. [Pct]
     */
    inline uint16_t emergencyRampUpRate() const { return parse_uint16(2); }

    /**
     * Emergency ramp down rate as a percentage of max current. [Pct]
     */
    inline uint16_t emergencyRampDownRate() const { return parse_uint16(3); }

    /**
     * Connect ramp up rate as a percentage of max current. [Pct]
     */
    inline uint16_t connectRampUpRate() const { return parse_uint16(4); }

    /**
     * Connect ramp down rate as a percentage of max current. [Pct]
     */
    inline uint16_t connectRampDownRate() const { return parse_uint16(5); }

    /**
     * Ramp rate specified in percent of max current. [Pct]
     */
    inline uint16_t defaultRampRate() const { return parse_uint16(6); }

    /**
     * Ramp Rate Scale Factor
     */
    inline int16_t rampRateScaleFactor() const { return parse_sunssf(7); }

};

/**
 * Include this model for single phase (AN or AB) metering
 */
class MeterSinglePhaseSinglePhaseANOrABMeter : public SunSpecModel<201, 105> {
  public:
    /**
     * Total AC Current [A]
     */
    inline int16_t amps() const { return parse_int16(0); }

    /**
     * Phase A Current [A]
     */
    inline int16_t ampsPhaseA() const { return parse_int16(1); }

    /**
     * Phase B Current [A]
     */
    inline int16_t ampsPhaseB() const { return parse_int16(2); }

    /**
     * Phase C Current [A]
     */
    inline int16_t ampsPhaseC() const { return parse_int16(3); }

    /**
     * Current scale factor
     */
    inline int16_t A_SF() const { return parse_sunssf(4); }

    /**
     * Line to Neutral AC Voltage (average of active phases) [V]
     */
    inline int16_t voltageLN() const { return parse_int16(5); }

    /**
     * Phase Voltage AN [V]
     */
    inline int16_t phaseVoltageAN() const { return parse_int16(6); }

    /**
     * Phase Voltage BN [V]
     */
    inline int16_t phaseVoltageBN() const { return parse_int16(7); }

    /**
     * Phase Voltage CN [V]
     */
    inline int16_t phaseVoltageCN() const { return parse_int16(8); }

    /**
     * Line to Line AC Voltage (average of active phases) [V]
     */
    inline int16_t voltageLL() const { return parse_int16(9); }

    /**
     * Phase Voltage AB [V]
     */
    inline int16_t phaseVoltageAB() const { return parse_int16(10); }

    /**
     * Phase Voltage BC [V]
     */
    inline int16_t phaseVoltageBC() const { return parse_int16(11); }

    /**
     * Phase Voltage CA [V]
     */
    inline int16_t phaseVoltageCA() const { return parse_int16(12); }

    /**
     * Voltage scale factor
     */
    inline int16_t V_SF() const { return parse_sunssf(13); }

    /**
     * Frequency [Hz]
     */
    inline int16_t hz() const { return parse_int16(14); }

    /**
     * Frequency scale factor
     */
    inline int16_t hz_SF() const { return parse_sunssf(15); }

    /**
     * Total Real Power [W]
     */
    inline int16_t watts() const { return parse_int16(16); }

    /**
     * [W]
     */
    inline int16_t wattsPhaseA() const { return parse_int16(17); }

    /**
     * [W]
     */
    inline int16_t wattsPhaseB() const { return parse_int16(18); }

    /**
     * [W]
     */
    inline int16_t wattsPhaseC() const { return parse_int16(19); }

    /**
     * Real Power scale factor
     */
    inline int16_t W_SF() const { return parse_sunssf(20); }

    /**
     * AC Apparent Power [VA]
     */
    inline int16_t VA() const { return parse_int16(21); }

    /**
     * [VA]
     */
    inline int16_t VAPhaseA() const { return parse_int16(22); }

    /**
     * [VA]
     */
    inline int16_t VAPhaseB() const { return parse_int16(23); }

    /**
     * [VA]
     */
    inline int16_t VAPhaseC() const { return parse_int16(24); }

    /**
     * Apparent Power scale factor
     */
    inline int16_t VA_SF() const { return parse_sunssf(25); }

    /**
     * Reactive Power [var]
     */
    inline int16_t VAR() const { return parse_int16(26); }

    /**
     * [var]
     */
    inline int16_t VARPhaseA() const { return parse_int16(27); }

    /**
     * [var]
     */
    inline int16_t VARPhaseB() const { return parse_int16(28); }

    /**
     * [var]
     */
    inline int16_t VARPhaseC() const { return parse_int16(29); }

    /**
     * Reactive Power scale factor
     */
    inline int16_t VAR_SF() const { return parse_sunssf(30); }

    /**
     * Power Factor [Pct]
     */
    inline int16_t PF() const { return parse_int16(31); }

    /**
     * [Pct]
     */
    inline int16_t PFPhaseA() const { return parse_int16(32); }

    /**
     * [Pct]
     */
    inline int16_t PFPhaseB() const { return parse_int16(33); }

    /**
     * [Pct]
     */
    inline int16_t PFPhaseC() const { return parse_int16(34); }

    /**
     * Power Factor scale factor
     */
    inline int16_t PF_SF() const { return parse_sunssf(35); }

    /**
     * Total Real Energy Exported [Wh]
     */
    inline uint32_t totalWattHoursExported() const { return parse_acc32(36); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursExportedPhaseA() const { return parse_acc32(38); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursExportedPhaseB() const { return parse_acc32(40); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursExportedPhaseC() const { return parse_acc32(42); }

    /**
     * Total Real Energy Imported [Wh]
     */
    inline uint32_t totalWattHoursImported() const { return parse_acc32(44); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursImportedPhaseA() const { return parse_acc32(46); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursImportedPhaseB() const { return parse_acc32(48); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursImportedPhaseC() const { return parse_acc32(50); }

    /**
     * Real Energy scale factor
     */
    inline int16_t totWh_SF() const { return parse_sunssf(52); }

    /**
     * Total Apparent Energy Exported [VAh]
     */
    inline uint32_t totalVAHoursExported() const { return parse_acc32(53); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursExportedPhaseA() const { return parse_acc32(55); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursExportedPhaseB() const { return parse_acc32(57); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursExportedPhaseC() const { return parse_acc32(59); }

    /**
     * Total Apparent Energy Imported [VAh]
     */
    inline uint32_t totalVAHoursImported() const { return parse_acc32(61); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursImportedPhaseA() const { return parse_acc32(63); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursImportedPhaseB() const { return parse_acc32(65); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursImportedPhaseC() const { return parse_acc32(67); }

    /**
     * Apparent Energy scale factor
     */
    inline int16_t totVAh_SF() const { return parse_sunssf(69); }

    /**
     * Total Reactive Energy Imported Quadrant 1 [varh]
     */
    inline uint32_t totalVARHoursImportedQ1() const { return parse_acc32(70); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ1PhaseA() const { return parse_acc32(72); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ1PhaseB() const { return parse_acc32(74); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ1PhaseC() const { return parse_acc32(76); }

    /**
     * Total Reactive Power Imported Quadrant 2 [varh]
     */
    inline uint32_t totalVArHoursImportedQ2() const { return parse_acc32(78); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ2PhaseA() const { return parse_acc32(80); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ2PhaseB() const { return parse_acc32(82); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ2PhaseC() const { return parse_acc32(84); }

    /**
     * Total Reactive Power Exported Quadrant 3 [varh]
     */
    inline uint32_t totalVArHoursExportedQ3() const { return parse_acc32(86); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ3PhaseA() const { return parse_acc32(88); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ3PhaseB() const { return parse_acc32(90); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ3PhaseC() const { return parse_acc32(92); }

    /**
     * Total Reactive Power Exported Quadrant 4 [varh]
     */
    inline uint32_t totalVArHoursExportedQ4() const { return parse_acc32(94); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ4ImportedPhaseA() const { return parse_acc32(96); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ4ImportedPhaseB() const { return parse_acc32(98); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ4ImportedPhaseC() const { return parse_acc32(100); }

    /**
     * Reactive Energy scale factor
     */
    inline int16_t totVArh_SF() const { return parse_sunssf(102); }

    /**
     * Meter Event Flags
     */
    inline uint32_t events() const { return parse_bitfield32(103); }

};

class SplitSinglePhaseABNMeter_202 : public SunSpecModel<202, 105> {
  public:
    /**
     * Total AC Current [A]
     */
    inline int16_t amps() const { return parse_int16(0); }

    /**
     * Phase A Current [A]
     */
    inline int16_t ampsPhaseA() const { return parse_int16(1); }

    /**
     * Phase B Current [A]
     */
    inline int16_t ampsPhaseB() const { return parse_int16(2); }

    /**
     * Phase C Current [A]
     */
    inline int16_t ampsPhaseC() const { return parse_int16(3); }

    /**
     * Current scale factor
     */
    inline int16_t A_SF() const { return parse_sunssf(4); }

    /**
     * Line to Neutral AC Voltage (average of active phases) [V]
     */
    inline int16_t voltageLN() const { return parse_int16(5); }

    /**
     * Phase Voltage AN [V]
     */
    inline int16_t phaseVoltageAN() const { return parse_int16(6); }

    /**
     * Phase Voltage BN [V]
     */
    inline int16_t phaseVoltageBN() const { return parse_int16(7); }

    /**
     * Phase Voltage CN [V]
     */
    inline int16_t phaseVoltageCN() const { return parse_int16(8); }

    /**
     * Line to Line AC Voltage (average of active phases) [V]
     */
    inline int16_t voltageLL() const { return parse_int16(9); }

    /**
     * Phase Voltage AB [V]
     */
    inline int16_t phaseVoltageAB() const { return parse_int16(10); }

    /**
     * Phase Voltage BC [V]
     */
    inline int16_t phaseVoltageBC() const { return parse_int16(11); }

    /**
     * Phase Voltage CA [V]
     */
    inline int16_t phaseVoltageCA() const { return parse_int16(12); }

    /**
     * Voltage scale factor
     */
    inline int16_t V_SF() const { return parse_sunssf(13); }

    /**
     * Frequency [Hz]
     */
    inline int16_t hz() const { return parse_int16(14); }

    /**
     * Frequency scale factor
     */
    inline int16_t hz_SF() const { return parse_sunssf(15); }

    /**
     * Total Real Power [W]
     */
    inline int16_t watts() const { return parse_int16(16); }

    /**
     * [W]
     */
    inline int16_t wattsPhaseA() const { return parse_int16(17); }

    /**
     * [W]
     */
    inline int16_t wattsPhaseB() const { return parse_int16(18); }

    /**
     * [W]
     */
    inline int16_t wattsPhaseC() const { return parse_int16(19); }

    /**
     * Real Power scale factor
     */
    inline int16_t W_SF() const { return parse_sunssf(20); }

    /**
     * AC Apparent Power [VA]
     */
    inline int16_t VA() const { return parse_int16(21); }

    /**
     * [VA]
     */
    inline int16_t VAPhaseA() const { return parse_int16(22); }

    /**
     * [VA]
     */
    inline int16_t VAPhaseB() const { return parse_int16(23); }

    /**
     * [VA]
     */
    inline int16_t VAPhaseC() const { return parse_int16(24); }

    /**
     * Apparent Power scale factor
     */
    inline int16_t VA_SF() const { return parse_sunssf(25); }

    /**
     * Reactive Power [var]
     */
    inline int16_t VAR() const { return parse_int16(26); }

    /**
     * [var]
     */
    inline int16_t VARPhaseA() const { return parse_int16(27); }

    /**
     * [var]
     */
    inline int16_t VARPhaseB() const { return parse_int16(28); }

    /**
     * [var]
     */
    inline int16_t VARPhaseC() const { return parse_int16(29); }

    /**
     * Reactive Power scale factor
     */
    inline int16_t VAR_SF() const { return parse_sunssf(30); }

    /**
     * Power Factor [Pct]
     */
    inline int16_t PF() const { return parse_int16(31); }

    /**
     * [Pct]
     */
    inline int16_t PFPhaseA() const { return parse_int16(32); }

    /**
     * [Pct]
     */
    inline int16_t PFPhaseB() const { return parse_int16(33); }

    /**
     * [Pct]
     */
    inline int16_t PFPhaseC() const { return parse_int16(34); }

    /**
     * Power Factor scale factor
     */
    inline int16_t PF_SF() const { return parse_sunssf(35); }

    /**
     * Total Real Energy Exported [Wh]
     */
    inline uint32_t totalWattHoursExported() const { return parse_acc32(36); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursExportedPhaseA() const { return parse_acc32(38); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursExportedPhaseB() const { return parse_acc32(40); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursExportedPhaseC() const { return parse_acc32(42); }

    /**
     * Total Real Energy Imported [Wh]
     */
    inline uint32_t totalWattHoursImported() const { return parse_acc32(44); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursImportedPhaseA() const { return parse_acc32(46); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursImportedPhaseB() const { return parse_acc32(48); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursImportedPhaseC() const { return parse_acc32(50); }

    /**
     * Real Energy scale factor
     */
    inline int16_t totWh_SF() const { return parse_sunssf(52); }

    /**
     * Total Apparent Energy Exported [VAh]
     */
    inline uint32_t totalVAHoursExported() const { return parse_acc32(53); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursExportedPhaseA() const { return parse_acc32(55); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursExportedPhaseB() const { return parse_acc32(57); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursExportedPhaseC() const { return parse_acc32(59); }

    /**
     * Total Apparent Energy Imported [VAh]
     */
    inline uint32_t totalVAHoursImported() const { return parse_acc32(61); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursImportedPhaseA() const { return parse_acc32(63); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursImportedPhaseB() const { return parse_acc32(65); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursImportedPhaseC() const { return parse_acc32(67); }

    /**
     * Apparent Energy scale factor
     */
    inline int16_t totVAh_SF() const { return parse_sunssf(69); }

    /**
     * Total Reactive Energy Imported Quadrant 1 [varh]
     */
    inline uint32_t totalVARHoursImportedQ1() const { return parse_acc32(70); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ1PhaseA() const { return parse_acc32(72); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ1PhaseB() const { return parse_acc32(74); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ1PhaseC() const { return parse_acc32(76); }

    /**
     * Total Reactive Power Imported Quadrant 2 [varh]
     */
    inline uint32_t totalVArHoursImportedQ2() const { return parse_acc32(78); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ2PhaseA() const { return parse_acc32(80); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ2PhaseB() const { return parse_acc32(82); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ2PhaseC() const { return parse_acc32(84); }

    /**
     * Total Reactive Power Exported Quadrant 3 [varh]
     */
    inline uint32_t totalVArHoursExportedQ3() const { return parse_acc32(86); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ3PhaseA() const { return parse_acc32(88); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ3PhaseB() const { return parse_acc32(90); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ3PhaseC() const { return parse_acc32(92); }

    /**
     * Total Reactive Power Exported Quadrant 4 [varh]
     */
    inline uint32_t totalVArHoursExportedQ4() const { return parse_acc32(94); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ4ImportedPhaseA() const { return parse_acc32(96); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ4ImportedPhaseB() const { return parse_acc32(98); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ4ImportedPhaseC() const { return parse_acc32(100); }

    /**
     * Reactive Energy scale factor
     */
    inline int16_t totVArh_SF() const { return parse_sunssf(102); }

    /**
     * Meter Event Flags
     */
    inline uint32_t events() const { return parse_bitfield32(103); }

};

class WyeConnectThreePhaseAbcnMeter_203 : public SunSpecModel<203, 105> {
  public:
    /**
     * Total AC Current [A]
     */
    inline int16_t amps() const { return parse_int16(0); }

    /**
     * Phase A Current [A]
     */
    inline int16_t ampsPhaseA() const { return parse_int16(1); }

    /**
     * Phase B Current [A]
     */
    inline int16_t ampsPhaseB() const { return parse_int16(2); }

    /**
     * Phase C Current [A]
     */
    inline int16_t ampsPhaseC() const { return parse_int16(3); }

    /**
     * Current scale factor
     */
    inline int16_t A_SF() const { return parse_sunssf(4); }

    /**
     * Line to Neutral AC Voltage (average of active phases) [V]
     */
    inline int16_t voltageLN() const { return parse_int16(5); }

    /**
     * Phase Voltage AN [V]
     */
    inline int16_t phaseVoltageAN() const { return parse_int16(6); }

    /**
     * Phase Voltage BN [V]
     */
    inline int16_t phaseVoltageBN() const { return parse_int16(7); }

    /**
     * Phase Voltage CN [V]
     */
    inline int16_t phaseVoltageCN() const { return parse_int16(8); }

    /**
     * Line to Line AC Voltage (average of active phases) [V]
     */
    inline int16_t voltageLL() const { return parse_int16(9); }

    /**
     * Phase Voltage AB [V]
     */
    inline int16_t phaseVoltageAB() const { return parse_int16(10); }

    /**
     * Phase Voltage BC [V]
     */
    inline int16_t phaseVoltageBC() const { return parse_int16(11); }

    /**
     * Phase Voltage CA [V]
     */
    inline int16_t phaseVoltageCA() const { return parse_int16(12); }

    /**
     * Voltage scale factor
     */
    inline int16_t V_SF() const { return parse_sunssf(13); }

    /**
     * Frequency [Hz]
     */
    inline int16_t hz() const { return parse_int16(14); }

    /**
     * Frequency scale factor
     */
    inline int16_t hz_SF() const { return parse_sunssf(15); }

    /**
     * Total Real Power [W]
     */
    inline int16_t watts() const { return parse_int16(16); }

    /**
     * [W]
     */
    inline int16_t wattsPhaseA() const { return parse_int16(17); }

    /**
     * [W]
     */
    inline int16_t wattsPhaseB() const { return parse_int16(18); }

    /**
     * [W]
     */
    inline int16_t wattsPhaseC() const { return parse_int16(19); }

    /**
     * Real Power scale factor
     */
    inline int16_t W_SF() const { return parse_sunssf(20); }

    /**
     * AC Apparent Power [VA]
     */
    inline int16_t VA() const { return parse_int16(21); }

    /**
     * [VA]
     */
    inline int16_t VAPhaseA() const { return parse_int16(22); }

    /**
     * [VA]
     */
    inline int16_t VAPhaseB() const { return parse_int16(23); }

    /**
     * [VA]
     */
    inline int16_t VAPhaseC() const { return parse_int16(24); }

    /**
     * Apparent Power scale factor
     */
    inline int16_t VA_SF() const { return parse_sunssf(25); }

    /**
     * Reactive Power [var]
     */
    inline int16_t VAR() const { return parse_int16(26); }

    /**
     * [var]
     */
    inline int16_t VARPhaseA() const { return parse_int16(27); }

    /**
     * [var]
     */
    inline int16_t VARPhaseB() const { return parse_int16(28); }

    /**
     * [var]
     */
    inline int16_t VARPhaseC() const { return parse_int16(29); }

    /**
     * Reactive Power scale factor
     */
    inline int16_t VAR_SF() const { return parse_sunssf(30); }

    /**
     * Power Factor [Pct]
     */
    inline int16_t PF() const { return parse_int16(31); }

    /**
     * [Pct]
     */
    inline int16_t PFPhaseA() const { return parse_int16(32); }

    /**
     * [Pct]
     */
    inline int16_t PFPhaseB() const { return parse_int16(33); }

    /**
     * [Pct]
     */
    inline int16_t PFPhaseC() const { return parse_int16(34); }

    /**
     * Power Factor scale factor
     */
    inline int16_t PF_SF() const { return parse_sunssf(35); }

    /**
     * Total Real Energy Exported [Wh]
     */
    inline uint32_t totalWattHoursExported() const { return parse_acc32(36); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursExportedPhaseA() const { return parse_acc32(38); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursExportedPhaseB() const { return parse_acc32(40); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursExportedPhaseC() const { return parse_acc32(42); }

    /**
     * Total Real Energy Imported [Wh]
     */
    inline uint32_t totalWattHoursImported() const { return parse_acc32(44); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursImportedPhaseA() const { return parse_acc32(46); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursImportedPhaseB() const { return parse_acc32(48); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursImportedPhaseC() const { return parse_acc32(50); }

    /**
     * Real Energy scale factor
     */
    inline int16_t totWh_SF() const { return parse_sunssf(52); }

    /**
     * Total Apparent Energy Exported [VAh]
     */
    inline uint32_t totalVAHoursExported() const { return parse_acc32(53); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursExportedPhaseA() const { return parse_acc32(55); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursExportedPhaseB() const { return parse_acc32(57); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursExportedPhaseC() const { return parse_acc32(59); }

    /**
     * Total Apparent Energy Imported [VAh]
     */
    inline uint32_t totalVAHoursImported() const { return parse_acc32(61); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursImportedPhaseA() const { return parse_acc32(63); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursImportedPhaseB() const { return parse_acc32(65); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursImportedPhaseC() const { return parse_acc32(67); }

    /**
     * Apparent Energy scale factor
     */
    inline int16_t totVAh_SF() const { return parse_sunssf(69); }

    /**
     * Total Reactive Energy Imported Quadrant 1 [varh]
     */
    inline uint32_t totalVARHoursImportedQ1() const { return parse_acc32(70); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ1PhaseA() const { return parse_acc32(72); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ1PhaseB() const { return parse_acc32(74); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ1PhaseC() const { return parse_acc32(76); }

    /**
     * Total Reactive Power Imported Quadrant 2 [varh]
     */
    inline uint32_t totalVArHoursImportedQ2() const { return parse_acc32(78); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ2PhaseA() const { return parse_acc32(80); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ2PhaseB() const { return parse_acc32(82); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ2PhaseC() const { return parse_acc32(84); }

    /**
     * Total Reactive Power Exported Quadrant 3 [varh]
     */
    inline uint32_t totalVArHoursExportedQ3() const { return parse_acc32(86); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ3PhaseA() const { return parse_acc32(88); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ3PhaseB() const { return parse_acc32(90); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ3PhaseC() const { return parse_acc32(92); }

    /**
     * Total Reactive Power Exported Quadrant 4 [varh]
     */
    inline uint32_t totalVArHoursExportedQ4() const { return parse_acc32(94); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ4ImportedPhaseA() const { return parse_acc32(96); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ4ImportedPhaseB() const { return parse_acc32(98); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ4ImportedPhaseC() const { return parse_acc32(100); }

    /**
     * Reactive Energy scale factor
     */
    inline int16_t totVArh_SF() const { return parse_sunssf(102); }

    /**
     * Meter Event Flags
     */
    inline uint32_t events() const { return parse_bitfield32(103); }

};

class DeltaConnectThreePhaseAbcMeter_204 : public SunSpecModel<204, 105> {
  public:
    /**
     * Total AC Current [A]
     */
    inline int16_t amps() const { return parse_int16(0); }

    /**
     * Phase A Current [A]
     */
    inline int16_t ampsPhaseA() const { return parse_int16(1); }

    /**
     * Phase B Current [A]
     */
    inline int16_t ampsPhaseB() const { return parse_int16(2); }

    /**
     * Phase C Current [A]
     */
    inline int16_t ampsPhaseC() const { return parse_int16(3); }

    /**
     * Current scale factor
     */
    inline int16_t A_SF() const { return parse_sunssf(4); }

    /**
     * Line to Neutral AC Voltage (average of active phases) [V]
     */
    inline int16_t voltageLN() const { return parse_int16(5); }

    /**
     * Phase Voltage AN [V]
     */
    inline int16_t phaseVoltageAN() const { return parse_int16(6); }

    /**
     * Phase Voltage BN [V]
     */
    inline int16_t phaseVoltageBN() const { return parse_int16(7); }

    /**
     * Phase Voltage CN [V]
     */
    inline int16_t phaseVoltageCN() const { return parse_int16(8); }

    /**
     * Line to Line AC Voltage (average of active phases) [V]
     */
    inline int16_t voltageLL() const { return parse_int16(9); }

    /**
     * Phase Voltage AB [V]
     */
    inline int16_t phaseVoltageAB() const { return parse_int16(10); }

    /**
     * Phase Voltage BC [V]
     */
    inline int16_t phaseVoltageBC() const { return parse_int16(11); }

    /**
     * Phase Voltage CA [V]
     */
    inline int16_t phaseVoltageCA() const { return parse_int16(12); }

    /**
     * Voltage scale factor
     */
    inline int16_t V_SF() const { return parse_sunssf(13); }

    /**
     * Frequency [Hz]
     */
    inline int16_t hz() const { return parse_int16(14); }

    /**
     * Frequency scale factor
     */
    inline int16_t hz_SF() const { return parse_sunssf(15); }

    /**
     * Total Real Power [W]
     */
    inline int16_t watts() const { return parse_int16(16); }

    /**
     * [W]
     */
    inline int16_t wattsPhaseA() const { return parse_int16(17); }

    /**
     * [W]
     */
    inline int16_t wattsPhaseB() const { return parse_int16(18); }

    /**
     * [W]
     */
    inline int16_t wattsPhaseC() const { return parse_int16(19); }

    /**
     * Real Power scale factor
     */
    inline int16_t W_SF() const { return parse_sunssf(20); }

    /**
     * AC Apparent Power [VA]
     */
    inline int16_t VA() const { return parse_int16(21); }

    /**
     * [VA]
     */
    inline int16_t VAPhaseA() const { return parse_int16(22); }

    /**
     * [VA]
     */
    inline int16_t VAPhaseB() const { return parse_int16(23); }

    /**
     * [VA]
     */
    inline int16_t VAPhaseC() const { return parse_int16(24); }

    /**
     * Apparent Power scale factor
     */
    inline int16_t VA_SF() const { return parse_sunssf(25); }

    /**
     * Reactive Power [var]
     */
    inline int16_t VAR() const { return parse_int16(26); }

    /**
     * [var]
     */
    inline int16_t VARPhaseA() const { return parse_int16(27); }

    /**
     * [var]
     */
    inline int16_t VARPhaseB() const { return parse_int16(28); }

    /**
     * [var]
     */
    inline int16_t VARPhaseC() const { return parse_int16(29); }

    /**
     * Reactive Power scale factor
     */
    inline int16_t VAR_SF() const { return parse_sunssf(30); }

    /**
     * Power Factor [Pct]
     */
    inline int16_t PF() const { return parse_int16(31); }

    /**
     * [Pct]
     */
    inline int16_t PFPhaseA() const { return parse_int16(32); }

    /**
     * [Pct]
     */
    inline int16_t PFPhaseB() const { return parse_int16(33); }

    /**
     * [Pct]
     */
    inline int16_t PFPhaseC() const { return parse_int16(34); }

    /**
     * Power Factor scale factor
     */
    inline int16_t PF_SF() const { return parse_sunssf(35); }

    /**
     * Total Real Energy Exported [Wh]
     */
    inline uint32_t totalWattHoursExported() const { return parse_acc32(36); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursExportedPhaseA() const { return parse_acc32(38); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursExportedPhaseB() const { return parse_acc32(40); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursExportedPhaseC() const { return parse_acc32(42); }

    /**
     * Total Real Energy Imported [Wh]
     */
    inline uint32_t totalWattHoursImported() const { return parse_acc32(44); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursImportedPhaseA() const { return parse_acc32(46); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursImportedPhaseB() const { return parse_acc32(48); }

    /**
     * [Wh]
     */
    inline uint32_t totalWattHoursImportedPhaseC() const { return parse_acc32(50); }

    /**
     * Real Energy scale factor
     */
    inline int16_t totWh_SF() const { return parse_sunssf(52); }

    /**
     * Total Apparent Energy Exported [VAh]
     */
    inline uint32_t totalVAHoursExported() const { return parse_acc32(53); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursExportedPhaseA() const { return parse_acc32(55); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursExportedPhaseB() const { return parse_acc32(57); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursExportedPhaseC() const { return parse_acc32(59); }

    /**
     * Total Apparent Energy Imported [VAh]
     */
    inline uint32_t totalVAHoursImported() const { return parse_acc32(61); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursImportedPhaseA() const { return parse_acc32(63); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursImportedPhaseB() const { return parse_acc32(65); }

    /**
     * [VAh]
     */
    inline uint32_t totalVAHoursImportedPhaseC() const { return parse_acc32(67); }

    /**
     * Apparent Energy scale factor
     */
    inline int16_t totVAh_SF() const { return parse_sunssf(69); }

    /**
     * Total Reactive Energy Imported Quadrant 1 [varh]
     */
    inline uint32_t totalVARHoursImportedQ1() const { return parse_acc32(70); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ1PhaseA() const { return parse_acc32(72); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ1PhaseB() const { return parse_acc32(74); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ1PhaseC() const { return parse_acc32(76); }

    /**
     * Total Reactive Power Imported Quadrant 2 [varh]
     */
    inline uint32_t totalVArHoursImportedQ2() const { return parse_acc32(78); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ2PhaseA() const { return parse_acc32(80); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ2PhaseB() const { return parse_acc32(82); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursImportedQ2PhaseC() const { return parse_acc32(84); }

    /**
     * Total Reactive Power Exported Quadrant 3 [varh]
     */
    inline uint32_t totalVArHoursExportedQ3() const { return parse_acc32(86); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ3PhaseA() const { return parse_acc32(88); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ3PhaseB() const { return parse_acc32(90); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ3PhaseC() const { return parse_acc32(92); }

    /**
     * Total Reactive Power Exported Quadrant 4 [varh]
     */
    inline uint32_t totalVArHoursExportedQ4() const { return parse_acc32(94); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ4ImportedPhaseA() const { return parse_acc32(96); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ4ImportedPhaseB() const { return parse_acc32(98); }

    /**
     * [varh]
     */
    inline uint32_t totalVArHoursExportedQ4ImportedPhaseC() const { return parse_acc32(100); }

    /**
     * Reactive Energy scale factor
     */
    inline int16_t totVArh_SF() const { return parse_sunssf(102); }

    /**
     * Meter Event Flags
     */
    inline uint32_t events() const { return parse_bitfield32(103); }

};

class SinglePhaseANOrABMeter : public SunSpecModel<211, 124> {
  public:
    /**
     * Total AC Current [A]
     */
    inline float amps() const { return parse_float32(0); }

    /**
     * Phase A Current [A]
     */
    inline float ampsPhaseA() const { return parse_float32(2); }

    /**
     * Phase B Current [A]
     */
    inline float ampsPhaseB() const { return parse_float32(4); }

    /**
     * Phase C Current [A]
     */
    inline float ampsPhaseC() const { return parse_float32(6); }

    /**
     * Line to Neutral AC Voltage (average of active phases) [V]
     */
    inline float voltageLN() const { return parse_float32(8); }

    /**
     * Phase Voltage AN [V]
     */
    inline float phaseVoltageAN() const { return parse_float32(10); }

    /**
     * Phase Voltage BN [V]
     */
    inline float phaseVoltageBN() const { return parse_float32(12); }

    /**
     * Phase Voltage CN [V]
     */
    inline float phaseVoltageCN() const { return parse_float32(14); }

    /**
     * Line to Line AC Voltage (average of active phases) [V]
     */
    inline float voltageLL() const { return parse_float32(16); }

    /**
     * Phase Voltage AB [V]
     */
    inline float phaseVoltageAB() const { return parse_float32(18); }

    /**
     * Phase Voltage BC [V]
     */
    inline float phaseVoltageBC() const { return parse_float32(20); }

    /**
     * Phase Voltage CA [V]
     */
    inline float phaseVoltageCA() const { return parse_float32(22); }

    /**
     * Frequency [Hz]
     */
    inline float hz() const { return parse_float32(24); }

    /**
     * Total Real Power [W]
     */
    inline float watts() const { return parse_float32(26); }

    /**
     * [W]
     */
    inline float wattsPhaseA() const { return parse_float32(28); }

    /**
     * [W]
     */
    inline float wattsPhaseB() const { return parse_float32(30); }

    /**
     * [W]
     */
    inline float wattsPhaseC() const { return parse_float32(32); }

    /**
     * AC Apparent Power [VA]
     */
    inline float VA() const { return parse_float32(34); }

    /**
     * [VA]
     */
    inline float VAPhaseA() const { return parse_float32(36); }

    /**
     * [VA]
     */
    inline float VAPhaseB() const { return parse_float32(38); }

    /**
     * [VA]
     */
    inline float VAPhaseC() const { return parse_float32(40); }

    /**
     * Reactive Power [var]
     */
    inline float VAR() const { return parse_float32(42); }

    /**
     * [var]
     */
    inline float VARPhaseA() const { return parse_float32(44); }

    /**
     * [var]
     */
    inline float VARPhaseB() const { return parse_float32(46); }

    /**
     * [var]
     */
    inline float VARPhaseC() const { return parse_float32(48); }

    /**
     * Power Factor [PF]
     */
    inline float PF() const { return parse_float32(50); }

    /**
     * [PF]
     */
    inline float PFPhaseA() const { return parse_float32(52); }

    /**
     * [PF]
     */
    inline float PFPhaseB() const { return parse_float32(54); }

    /**
     * [PF]
     */
    inline float PFPhaseC() const { return parse_float32(56); }

    /**
     * Total Real Energy Exported [Wh]
     */
    inline float totalWattHoursExported() const { return parse_float32(58); }

    /**
     * [Wh]
     */
    inline float totalWattHoursExportedPhaseA() const { return parse_float32(60); }

    /**
     * [Wh]
     */
    inline float totalWattHoursExportedPhaseB() const { return parse_float32(62); }

    /**
     * [Wh]
     */
    inline float totalWattHoursExportedPhaseC() const { return parse_float32(64); }

    /**
     * Total Real Energy Imported [Wh]
     */
    inline float totalWattHoursImported() const { return parse_float32(66); }

    /**
     * [Wh]
     */
    inline float totalWattHoursImportedPhaseA() const { return parse_float32(68); }

    /**
     * [Wh]
     */
    inline float totalWattHoursImportedPhaseB() const { return parse_float32(70); }

    /**
     * [Wh]
     */
    inline float totalWattHoursImportedPhaseC() const { return parse_float32(72); }

    /**
     * Total Apparent Energy Exported [VAh]
     */
    inline float totalVAHoursExported() const { return parse_float32(74); }

    /**
     * [VAh]
     */
    inline float totalVAHoursExportedPhaseA() const { return parse_float32(76); }

    /**
     * [VAh]
     */
    inline float totalVAHoursExportedPhaseB() const { return parse_float32(78); }

    /**
     * [VAh]
     */
    inline float totalVAHoursExportedPhaseC() const { return parse_float32(80); }

    /**
     * Total Apparent Energy Imported [VAh]
     */
    inline float totalVAHoursImported() const { return parse_float32(82); }

    /**
     * [VAh]
     */
    inline float totalVAHoursImportedPhaseA() const { return parse_float32(84); }

    /**
     * [VAh]
     */
    inline float totalVAHoursImportedPhaseB() const { return parse_float32(86); }

    /**
     * [VAh]
     */
    inline float totalVAHoursImportedPhaseC() const { return parse_float32(88); }

    /**
     * Total Reactive Energy Imported Quadrant 1 [varh]
     */
    inline float totalVARHoursImportedQ1() const { return parse_float32(90); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ1PhaseA() const { return parse_float32(92); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ1PhaseB() const { return parse_float32(94); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ1PhaseC() const { return parse_float32(96); }

    /**
     * Total Reactive Power Imported Quadrant 2 [varh]
     */
    inline float totalVArHoursImportedQ2() const { return parse_float32(98); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ2PhaseA() const { return parse_float32(100); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ2PhaseB() const { return parse_float32(102); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ2PhaseC() const { return parse_float32(104); }

    /**
     * Total Reactive Power Exported Quadrant 3 [varh]
     */
    inline float totalVArHoursExportedQ3() const { return parse_float32(106); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ3PhaseA() const { return parse_float32(108); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ3PhaseB() const { return parse_float32(110); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ3PhaseC() const { return parse_float32(112); }

    /**
     * Total Reactive Power Exported Quadrant 4 [varh]
     */
    inline float totalVArHoursExportedQ4() const { return parse_float32(114); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ4ImportedPhaseA() const { return parse_float32(116); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ4ImportedPhaseB() const { return parse_float32(118); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ4ImportedPhaseC() const { return parse_float32(120); }

    /**
     * Meter Event Flags
     */
    inline uint32_t events() const { return parse_bitfield32(122); }

};

class SplitSinglePhaseABNMeter_212 : public SunSpecModel<212, 124> {
  public:
    /**
     * Total AC Current [A]
     */
    inline float amps() const { return parse_float32(0); }

    /**
     * Phase A Current [A]
     */
    inline float ampsPhaseA() const { return parse_float32(2); }

    /**
     * Phase B Current [A]
     */
    inline float ampsPhaseB() const { return parse_float32(4); }

    /**
     * Phase C Current [A]
     */
    inline float ampsPhaseC() const { return parse_float32(6); }

    /**
     * Line to Neutral AC Voltage (average of active phases) [V]
     */
    inline float voltageLN() const { return parse_float32(8); }

    /**
     * Phase Voltage AN [V]
     */
    inline float phaseVoltageAN() const { return parse_float32(10); }

    /**
     * Phase Voltage BN [V]
     */
    inline float phaseVoltageBN() const { return parse_float32(12); }

    /**
     * Phase Voltage CN [V]
     */
    inline float phaseVoltageCN() const { return parse_float32(14); }

    /**
     * Line to Line AC Voltage (average of active phases) [V]
     */
    inline float voltageLL() const { return parse_float32(16); }

    /**
     * Phase Voltage AB [V]
     */
    inline float phaseVoltageAB() const { return parse_float32(18); }

    /**
     * Phase Voltage BC [V]
     */
    inline float phaseVoltageBC() const { return parse_float32(20); }

    /**
     * Phase Voltage CA [V]
     */
    inline float phaseVoltageCA() const { return parse_float32(22); }

    /**
     * Frequency [Hz]
     */
    inline float hz() const { return parse_float32(24); }

    /**
     * Total Real Power [W]
     */
    inline float watts() const { return parse_float32(26); }

    /**
     * [W]
     */
    inline float wattsPhaseA() const { return parse_float32(28); }

    /**
     * [W]
     */
    inline float wattsPhaseB() const { return parse_float32(30); }

    /**
     * [W]
     */
    inline float wattsPhaseC() const { return parse_float32(32); }

    /**
     * AC Apparent Power [VA]
     */
    inline float VA() const { return parse_float32(34); }

    /**
     * [VA]
     */
    inline float VAPhaseA() const { return parse_float32(36); }

    /**
     * [VA]
     */
    inline float VAPhaseB() const { return parse_float32(38); }

    /**
     * [VA]
     */
    inline float VAPhaseC() const { return parse_float32(40); }

    /**
     * Reactive Power [var]
     */
    inline float VAR() const { return parse_float32(42); }

    /**
     * [var]
     */
    inline float VARPhaseA() const { return parse_float32(44); }

    /**
     * [var]
     */
    inline float VARPhaseB() const { return parse_float32(46); }

    /**
     * [var]
     */
    inline float VARPhaseC() const { return parse_float32(48); }

    /**
     * Power Factor [PF]
     */
    inline float PF() const { return parse_float32(50); }

    /**
     * [PF]
     */
    inline float PFPhaseA() const { return parse_float32(52); }

    /**
     * [PF]
     */
    inline float PFPhaseB() const { return parse_float32(54); }

    /**
     * [PF]
     */
    inline float PFPhaseC() const { return parse_float32(56); }

    /**
     * Total Real Energy Exported [Wh]
     */
    inline float totalWattHoursExported() const { return parse_float32(58); }

    /**
     * [Wh]
     */
    inline float totalWattHoursExportedPhaseA() const { return parse_float32(60); }

    /**
     * [Wh]
     */
    inline float totalWattHoursExportedPhaseB() const { return parse_float32(62); }

    /**
     * [Wh]
     */
    inline float totalWattHoursExportedPhaseC() const { return parse_float32(64); }

    /**
     * Total Real Energy Imported [Wh]
     */
    inline float totalWattHoursImported() const { return parse_float32(66); }

    /**
     * [Wh]
     */
    inline float totalWattHoursImportedPhaseA() const { return parse_float32(68); }

    /**
     * [Wh]
     */
    inline float totalWattHoursImportedPhaseB() const { return parse_float32(70); }

    /**
     * [Wh]
     */
    inline float totalWattHoursImportedPhaseC() const { return parse_float32(72); }

    /**
     * Total Apparent Energy Exported [VAh]
     */
    inline float totalVAHoursExported() const { return parse_float32(74); }

    /**
     * [VAh]
     */
    inline float totalVAHoursExportedPhaseA() const { return parse_float32(76); }

    /**
     * [VAh]
     */
    inline float totalVAHoursExportedPhaseB() const { return parse_float32(78); }

    /**
     * [VAh]
     */
    inline float totalVAHoursExportedPhaseC() const { return parse_float32(80); }

    /**
     * Total Apparent Energy Imported [VAh]
     */
    inline float totalVAHoursImported() const { return parse_float32(82); }

    /**
     * [VAh]
     */
    inline float totalVAHoursImportedPhaseA() const { return parse_float32(84); }

    /**
     * [VAh]
     */
    inline float totalVAHoursImportedPhaseB() const { return parse_float32(86); }

    /**
     * [VAh]
     */
    inline float totalVAHoursImportedPhaseC() const { return parse_float32(88); }

    /**
     * Total Reactive Energy Imported Quadrant 1 [varh]
     */
    inline float totalVARHoursImportedQ1() const { return parse_float32(90); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ1PhaseA() const { return parse_float32(92); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ1PhaseB() const { return parse_float32(94); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ1PhaseC() const { return parse_float32(96); }

    /**
     * Total Reactive Power Imported Quadrant 2 [varh]
     */
    inline float totalVArHoursImportedQ2() const { return parse_float32(98); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ2PhaseA() const { return parse_float32(100); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ2PhaseB() const { return parse_float32(102); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ2PhaseC() const { return parse_float32(104); }

    /**
     * Total Reactive Power Exported Quadrant 3 [varh]
     */
    inline float totalVArHoursExportedQ3() const { return parse_float32(106); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ3PhaseA() const { return parse_float32(108); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ3PhaseB() const { return parse_float32(110); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ3PhaseC() const { return parse_float32(112); }

    /**
     * Total Reactive Power Exported Quadrant 4 [varh]
     */
    inline float totalVArHoursExportedQ4() const { return parse_float32(114); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ4ImportedPhaseA() const { return parse_float32(116); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ4ImportedPhaseB() const { return parse_float32(118); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ4ImportedPhaseC() const { return parse_float32(120); }

    /**
     * Meter Event Flags
     */
    inline uint32_t events() const { return parse_bitfield32(122); }

};

class WyeConnectThreePhaseAbcnMeter_213 : public SunSpecModel<213, 124> {
  public:
    /**
     * Total AC Current [A]
     */
    inline float amps() const { return parse_float32(0); }

    /**
     * Phase A Current [A]
     */
    inline float ampsPhaseA() const { return parse_float32(2); }

    /**
     * Phase B Current [A]
     */
    inline float ampsPhaseB() const { return parse_float32(4); }

    /**
     * Phase C Current [A]
     */
    inline float ampsPhaseC() const { return parse_float32(6); }

    /**
     * Line to Neutral AC Voltage (average of active phases) [V]
     */
    inline float voltageLN() const { return parse_float32(8); }

    /**
     * Phase Voltage AN [V]
     */
    inline float phaseVoltageAN() const { return parse_float32(10); }

    /**
     * Phase Voltage BN [V]
     */
    inline float phaseVoltageBN() const { return parse_float32(12); }

    /**
     * Phase Voltage CN [V]
     */
    inline float phaseVoltageCN() const { return parse_float32(14); }

    /**
     * Line to Line AC Voltage (average of active phases) [V]
     */
    inline float voltageLL() const { return parse_float32(16); }

    /**
     * Phase Voltage AB [V]
     */
    inline float phaseVoltageAB() const { return parse_float32(18); }

    /**
     * Phase Voltage BC [V]
     */
    inline float phaseVoltageBC() const { return parse_float32(20); }

    /**
     * Phase Voltage CA [V]
     */
    inline float phaseVoltageCA() const { return parse_float32(22); }

    /**
     * Frequency [Hz]
     */
    inline float hz() const { return parse_float32(24); }

    /**
     * Total Real Power [W]
     */
    inline float watts() const { return parse_float32(26); }

    /**
     * [W]
     */
    inline float wattsPhaseA() const { return parse_float32(28); }

    /**
     * [W]
     */
    inline float wattsPhaseB() const { return parse_float32(30); }

    /**
     * [W]
     */
    inline float wattsPhaseC() const { return parse_float32(32); }

    /**
     * AC Apparent Power [VA]
     */
    inline float VA() const { return parse_float32(34); }

    /**
     * [VA]
     */
    inline float VAPhaseA() const { return parse_float32(36); }

    /**
     * [VA]
     */
    inline float VAPhaseB() const { return parse_float32(38); }

    /**
     * [VA]
     */
    inline float VAPhaseC() const { return parse_float32(40); }

    /**
     * Reactive Power [var]
     */
    inline float VAR() const { return parse_float32(42); }

    /**
     * [var]
     */
    inline float VARPhaseA() const { return parse_float32(44); }

    /**
     * [var]
     */
    inline float VARPhaseB() const { return parse_float32(46); }

    /**
     * [var]
     */
    inline float VARPhaseC() const { return parse_float32(48); }

    /**
     * Power Factor [PF]
     */
    inline float PF() const { return parse_float32(50); }

    /**
     * [PF]
     */
    inline float PFPhaseA() const { return parse_float32(52); }

    /**
     * [PF]
     */
    inline float PFPhaseB() const { return parse_float32(54); }

    /**
     * [PF]
     */
    inline float PFPhaseC() const { return parse_float32(56); }

    /**
     * Total Real Energy Exported [Wh]
     */
    inline float totalWattHoursExported() const { return parse_float32(58); }

    /**
     * [Wh]
     */
    inline float totalWattHoursExportedPhaseA() const { return parse_float32(60); }

    /**
     * [Wh]
     */
    inline float totalWattHoursExportedPhaseB() const { return parse_float32(62); }

    /**
     * [Wh]
     */
    inline float totalWattHoursExportedPhaseC() const { return parse_float32(64); }

    /**
     * Total Real Energy Imported [Wh]
     */
    inline float totalWattHoursImported() const { return parse_float32(66); }

    /**
     * [Wh]
     */
    inline float totalWattHoursImportedPhaseA() const { return parse_float32(68); }

    /**
     * [Wh]
     */
    inline float totalWattHoursImportedPhaseB() const { return parse_float32(70); }

    /**
     * [Wh]
     */
    inline float totalWattHoursImportedPhaseC() const { return parse_float32(72); }

    /**
     * Total Apparent Energy Exported [VAh]
     */
    inline float totalVAHoursExported() const { return parse_float32(74); }

    /**
     * [VAh]
     */
    inline float totalVAHoursExportedPhaseA() const { return parse_float32(76); }

    /**
     * [VAh]
     */
    inline float totalVAHoursExportedPhaseB() const { return parse_float32(78); }

    /**
     * [VAh]
     */
    inline float totalVAHoursExportedPhaseC() const { return parse_float32(80); }

    /**
     * Total Apparent Energy Imported [VAh]
     */
    inline float totalVAHoursImported() const { return parse_float32(82); }

    /**
     * [VAh]
     */
    inline float totalVAHoursImportedPhaseA() const { return parse_float32(84); }

    /**
     * [VAh]
     */
    inline float totalVAHoursImportedPhaseB() const { return parse_float32(86); }

    /**
     * [VAh]
     */
    inline float totalVAHoursImportedPhaseC() const { return parse_float32(88); }

    /**
     * Total Reactive Energy Imported Quadrant 1 [varh]
     */
    inline float totalVARHoursImportedQ1() const { return parse_float32(90); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ1PhaseA() const { return parse_float32(92); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ1PhaseB() const { return parse_float32(94); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ1PhaseC() const { return parse_float32(96); }

    /**
     * Total Reactive Power Imported Quadrant 2 [varh]
     */
    inline float totalVArHoursImportedQ2() const { return parse_float32(98); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ2PhaseA() const { return parse_float32(100); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ2PhaseB() const { return parse_float32(102); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ2PhaseC() const { return parse_float32(104); }

    /**
     * Total Reactive Power Exported Quadrant 3 [varh]
     */
    inline float totalVArHoursExportedQ3() const { return parse_float32(106); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ3PhaseA() const { return parse_float32(108); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ3PhaseB() const { return parse_float32(110); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ3PhaseC() const { return parse_float32(112); }

    /**
     * Total Reactive Power Exported Quadrant 4 [varh]
     */
    inline float totalVArHoursExportedQ4() const { return parse_float32(114); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ4ImportedPhaseA() const { return parse_float32(116); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ4ImportedPhaseB() const { return parse_float32(118); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ4ImportedPhaseC() const { return parse_float32(120); }

    /**
     * Meter Event Flags
     */
    inline uint32_t events() const { return parse_bitfield32(122); }

};

class DeltaConnectThreePhaseAbcMeter_214 : public SunSpecModel<214, 124> {
  public:
    /**
     * Total AC Current [A]
     */
    inline float amps() const { return parse_float32(0); }

    /**
     * Phase A Current [A]
     */
    inline float ampsPhaseA() const { return parse_float32(2); }

    /**
     * Phase B Current [A]
     */
    inline float ampsPhaseB() const { return parse_float32(4); }

    /**
     * Phase C Current [A]
     */
    inline float ampsPhaseC() const { return parse_float32(6); }

    /**
     * Line to Neutral AC Voltage (average of active phases) [V]
     */
    inline float voltageLN() const { return parse_float32(8); }

    /**
     * Phase Voltage AN [V]
     */
    inline float phaseVoltageAN() const { return parse_float32(10); }

    /**
     * Phase Voltage BN [V]
     */
    inline float phaseVoltageBN() const { return parse_float32(12); }

    /**
     * Phase Voltage CN [V]
     */
    inline float phaseVoltageCN() const { return parse_float32(14); }

    /**
     * Line to Line AC Voltage (average of active phases) [V]
     */
    inline float voltageLL() const { return parse_float32(16); }

    /**
     * Phase Voltage AB [V]
     */
    inline float phaseVoltageAB() const { return parse_float32(18); }

    /**
     * Phase Voltage BC [V]
     */
    inline float phaseVoltageBC() const { return parse_float32(20); }

    /**
     * Phase Voltage CA [V]
     */
    inline float phaseVoltageCA() const { return parse_float32(22); }

    /**
     * Frequency [Hz]
     */
    inline float hz() const { return parse_float32(24); }

    /**
     * Total Real Power [W]
     */
    inline float watts() const { return parse_float32(26); }

    /**
     * [W]
     */
    inline float wattsPhaseA() const { return parse_float32(28); }

    /**
     * [W]
     */
    inline float wattsPhaseB() const { return parse_float32(30); }

    /**
     * [W]
     */
    inline float wattsPhaseC() const { return parse_float32(32); }

    /**
     * AC Apparent Power [VA]
     */
    inline float VA() const { return parse_float32(34); }

    /**
     * [VA]
     */
    inline float VAPhaseA() const { return parse_float32(36); }

    /**
     * [VA]
     */
    inline float VAPhaseB() const { return parse_float32(38); }

    /**
     * [VA]
     */
    inline float VAPhaseC() const { return parse_float32(40); }

    /**
     * Reactive Power [var]
     */
    inline float VAR() const { return parse_float32(42); }

    /**
     * [var]
     */
    inline float VARPhaseA() const { return parse_float32(44); }

    /**
     * [var]
     */
    inline float VARPhaseB() const { return parse_float32(46); }

    /**
     * [var]
     */
    inline float VARPhaseC() const { return parse_float32(48); }

    /**
     * Power Factor [PF]
     */
    inline float PF() const { return parse_float32(50); }

    /**
     * [PF]
     */
    inline float PFPhaseA() const { return parse_float32(52); }

    /**
     * [PF]
     */
    inline float PFPhaseB() const { return parse_float32(54); }

    /**
     * [PF]
     */
    inline float PFPhaseC() const { return parse_float32(56); }

    /**
     * Total Real Energy Exported [Wh]
     */
    inline float totalWattHoursExported() const { return parse_float32(58); }

    /**
     * [Wh]
     */
    inline float totalWattHoursExportedPhaseA() const { return parse_float32(60); }

    /**
     * [Wh]
     */
    inline float totalWattHoursExportedPhaseB() const { return parse_float32(62); }

    /**
     * [Wh]
     */
    inline float totalWattHoursExportedPhaseC() const { return parse_float32(64); }

    /**
     * Total Real Energy Imported [Wh]
     */
    inline float totalWattHoursImported() const { return parse_float32(66); }

    /**
     * [Wh]
     */
    inline float totalWattHoursImportedPhaseA() const { return parse_float32(68); }

    /**
     * [Wh]
     */
    inline float totalWattHoursImportedPhaseB() const { return parse_float32(70); }

    /**
     * [Wh]
     */
    inline float totalWattHoursImportedPhaseC() const { return parse_float32(72); }

    /**
     * Total Apparent Energy Exported [VAh]
     */
    inline float totalVAHoursExported() const { return parse_float32(74); }

    /**
     * [VAh]
     */
    inline float totalVAHoursExportedPhaseA() const { return parse_float32(76); }

    /**
     * [VAh]
     */
    inline float totalVAHoursExportedPhaseB() const { return parse_float32(78); }

    /**
     * [VAh]
     */
    inline float totalVAHoursExportedPhaseC() const { return parse_float32(80); }

    /**
     * Total Apparent Energy Imported [VAh]
     */
    inline float totalVAHoursImported() const { return parse_float32(82); }

    /**
     * [VAh]
     */
    inline float totalVAHoursImportedPhaseA() const { return parse_float32(84); }

    /**
     * [VAh]
     */
    inline float totalVAHoursImportedPhaseB() const { return parse_float32(86); }

    /**
     * [VAh]
     */
    inline float totalVAHoursImportedPhaseC() const { return parse_float32(88); }

    /**
     * Total Reactive Energy Imported Quadrant 1 [varh]
     */
    inline float totalVARHoursImportedQ1() const { return parse_float32(90); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ1PhaseA() const { return parse_float32(92); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ1PhaseB() const { return parse_float32(94); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ1PhaseC() const { return parse_float32(96); }

    /**
     * Total Reactive Power Imported Quadrant 2 [varh]
     */
    inline float totalVArHoursImportedQ2() const { return parse_float32(98); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ2PhaseA() const { return parse_float32(100); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ2PhaseB() const { return parse_float32(102); }

    /**
     * [varh]
     */
    inline float totalVArHoursImportedQ2PhaseC() const { return parse_float32(104); }

    /**
     * Total Reactive Power Exported Quadrant 3 [varh]
     */
    inline float totalVArHoursExportedQ3() const { return parse_float32(106); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ3PhaseA() const { return parse_float32(108); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ3PhaseB() const { return parse_float32(110); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ3PhaseC() const { return parse_float32(112); }

    /**
     * Total Reactive Power Exported Quadrant 4 [varh]
     */
    inline float totalVArHoursExportedQ4() const { return parse_float32(114); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ4ImportedPhaseA() const { return parse_float32(116); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ4ImportedPhaseB() const { return parse_float32(118); }

    /**
     * [varh]
     */
    inline float totalVArHoursExportedQ4ImportedPhaseC() const { return parse_float32(120); }

    /**
     * Meter Event Flags
     */
    inline uint32_t events() const { return parse_bitfield32(122); }

};

/**
 * Include to support location measurements
 */
class GPS : public SunSpecModel<305, 36> {
  public:
    /**
     * UTC 24 hour time stamp to millisecond hhmmss.sssZ format [hhmmss.sssZ]
     */
    inline String tm() const { return parse_string(0, 6); }

    /**
     * UTC Date string YYYYMMDD format [YYYYMMDD]
     */
    inline String date() const { return parse_string(6, 4); }

    /**
     * Location string (40 chars max) [text]
     */
    inline String location() const { return parse_string(10, 20); }

    /**
     * Latitude with seven degrees of precision [Degrees]
     */
    inline int32_t lat() const { return parse_int32(30); }

    /**
     * Longitude with seven degrees of precision [Degrees]
     */
    inline int32_t long_() const { return parse_int32(32); }

    /**
     * Altitude measurement in meters [meters]
     */
    inline int32_t altitude() const { return parse_int32(34); }

};

/**
 * Include to support a standard reference point
 */
class ReferencePointModel : public SunSpecModel<306, 4> {
  public:
    /**
     * Global Horizontal Irradiance [W/m2]
     */
    inline uint16_t GHI() const { return parse_uint16(0); }

    /**
     * Current measurement at reference point [W/m2]
     */
    inline uint16_t amps() const { return parse_uint16(1); }

    /**
     * Voltage  measurement at reference point [W/m2]
     */
    inline uint16_t voltage() const { return parse_uint16(2); }

    /**
     * Temperature measurement at reference point [W/m2]
     */
    inline uint16_t temperature() const { return parse_uint16(3); }

};

/**
 * Base Meteorological Model
 */
class BaseMet : public SunSpecModel<307, 11> {
  public:
    /**
     * [C]
     */
    inline int16_t ambientTemperature() const { return parse_int16(0); }

    /**
     * [Pct]
     */
    inline int16_t relativeHumidity() const { return parse_int16(1); }

    /**
     * [HPa]
     */
    inline int16_t barometricPressure() const { return parse_int16(2); }

    /**
     * [mps]
     */
    inline int16_t windSpeed() const { return parse_int16(3); }

    /**
     * [deg]
     */
    inline int16_t windDirection() const { return parse_int16(4); }

    /**
     * [mm]
     */
    inline int16_t rainfall() const { return parse_int16(5); }

    /**
     * [mm]
     */
    inline int16_t snowDepth() const { return parse_int16(6); }

    /**
     * Precipitation Type (WMO 4680 SYNOP code reference)
     */
    inline int16_t precipitationType() const { return parse_int16(7); }

    /**
     * [Vm]
     */
    inline int16_t electricField() const { return parse_int16(8); }

    /**
     * [kO]
     */
    inline int16_t surfaceWetness() const { return parse_int16(9); }

    /**
     * [Pct]
     */
    inline int16_t soilWetness() const { return parse_int16(10); }

};

/**
 * Include to support a few basic measurements
 */
class MiniMetModel : public SunSpecModel<308, 4> {
  public:
    /**
     * Global Horizontal Irradiance [W/m2]
     */
    inline uint16_t GHI() const { return parse_uint16(0); }

    /**
     * Back of module temperature measurement [C]
     */
    inline int16_t temp() const { return parse_int16(1); }

    /**
     * [C]
     */
    inline int16_t ambientTemperature() const { return parse_int16(2); }

    /**
     * [m/s]
     */
    inline uint16_t windSpeed() const { return parse_uint16(3); }

};

/**
 * A solar module model supporting DC-DC converter
 */
class SolarModule_501 : public SunSpecModel<501, 31> {
  public:
    /**
     * Enumerated value.  Module Status Code
     */
    inline uint16_t status() const { return parse_enum16(0); }

    /**
     * Module Vendor Status Code
     */
    inline uint16_t vendorStatus() const { return parse_enum16(1); }

    /**
     * Bitmask value.  Module Event Flags
     */
    inline uint32_t events() const { return parse_bitfield32(2); }

    /**
     * Vendor specific flags
     */
    inline uint32_t vendorModuleEventFlags() const { return parse_bitfield32(4); }

    /**
     * Module Control
     */
    inline uint16_t control() const { return parse_enum16(6); }

    /**
     * Vendor Module Control
     */
    inline uint32_t vendorControl() const { return parse_enum32(7); }

    /**
     * Module Control Value
     */
    inline int32_t controlValue() const { return parse_int32(9); }

    /**
     * Time in seconds since 2000 epoch [Secs]
     */
    inline uint32_t timestamp() const { return parse_uint32(11); }

    /**
     * Output Current [A]
     */
    inline float outputCurrent() const { return parse_float32(13); }

    /**
     * Output Voltage [V]
     */
    inline float outputVoltage() const { return parse_float32(15); }

    /**
     * Output Energy [Wh]
     */
    inline float outputEnergy() const { return parse_float32(17); }

    /**
     * Output Power [W]
     */
    inline float outputPower() const { return parse_float32(19); }

    /**
     * Module Temperature [C]
     */
    inline float temp() const { return parse_float32(21); }

    /**
     * Input Current [A]
     */
    inline float inputCurrent() const { return parse_float32(23); }

    /**
     * Input Voltage [V]
     */
    inline float inputVoltage() const { return parse_float32(25); }

    /**
     * Input Energy [Wh]
     */
    inline float inputEnergy() const { return parse_float32(27); }

    /**
     * Input Power [W]
     */
    inline float inputPower() const { return parse_float32(29); }

};

/**
 * A solar module model supporting DC-DC converter
 */
class SolarModule_502 : public SunSpecModel<502, 28> {
  public:
    /**
     * Current scale factor
     */
    inline int16_t A_SF() const { return parse_sunssf(0); }

    /**
     * Voltage scale factor
     */
    inline int16_t V_SF() const { return parse_sunssf(1); }

    /**
     * Power scale factor
     */
    inline int16_t W_SF() const { return parse_sunssf(2); }

    /**
     * Energy scale factor
     */
    inline int16_t wh_SF() const { return parse_sunssf(3); }

    /**
     * Enumerated value.  Module Status Code
     */
    inline uint16_t status() const { return parse_enum16(4); }

    /**
     * Module Vendor Status Code
     */
    inline uint16_t vendorStatus() const { return parse_enum16(5); }

    /**
     * Bitmask value.  Module Event Flags
     */
    inline uint32_t events() const { return parse_bitfield32(6); }

    /**
     * Vendor specific flags
     */
    inline uint32_t vendorModuleEventFlags() const { return parse_bitfield32(8); }

    /**
     * Module Control
     */
    inline uint16_t control() const { return parse_enum16(10); }

    /**
     * Vendor Module Control
     */
    inline uint32_t vendorControl() const { return parse_enum32(11); }

    /**
     * Module Control Value
     */
    inline int32_t controlValue() const { return parse_int32(13); }

    /**
     * Time in seconds since 2000 epoch [Secs]
     */
    inline uint32_t timestamp() const { return parse_uint32(15); }

    /**
     * Output Current [A]
     */
    inline int16_t outputCurrent() const { return parse_int16(17); }

    /**
     * Output Voltage [V]
     */
    inline int16_t outputVoltage() const { return parse_int16(18); }

    /**
     * Output Energy [Wh]
     */
    inline uint32_t outputEnergy() const { return parse_acc32(19); }

    /**
     * Output Power [W]
     */
    inline int16_t outputPower() const { return parse_int16(21); }

    /**
     * Module Temperature [C]
     */
    inline int16_t temp() const { return parse_int16(22); }

    /**
     * Input Current [A]
     */
    inline int16_t inputCurrent() const { return parse_int16(23); }

    /**
     * Input Voltage [V]
     */
    inline int16_t inputVoltage() const { return parse_int16(24); }

    /**
     * Input Energy [Wh]
     */
    inline uint32_t inputEnergy() const { return parse_acc32(25); }

    /**
     * Input Power [W]
     */
    inline int16_t inputPower() const { return parse_int16(27); }

};

/**
 * DER AC measurement model.
 */
class DERACMeasurement : public SunSpecModel<701, 153> {
  public:
    /**
     * AC wiring type.
     */
    inline uint16_t ACWiringType() const { return parse_enum16(0); }

    /**
     * Operating state of the DER.
     */
    inline uint16_t operatingState() const { return parse_enum16(1); }

    /**
     * Enumerated value.  Inverter state.
     */
    inline uint16_t inverterState() const { return parse_enum16(2); }

    /**
     * Grid connection state of the DER.
     */
    inline uint16_t gridConnectionState() const { return parse_enum16(3); }

    /**
     * Active alarms for the DER.
     */
    inline uint32_t alarmBitfield() const { return parse_bitfield32(4); }

    /**
     * Current operational characteristics of the DER.
     */
    inline uint32_t DEROperationalCharacteristics() const { return parse_bitfield32(6); }

    /**
     * Total active power. Active power is positive for DER generation and negative for absorption. [W]
     */
    inline int16_t activePower() const { return parse_int16(8); }

    /**
     * Total apparent power. [VA]
     */
    inline int16_t apparentPower() const { return parse_int16(9); }

    /**
     * Total reactive power. [Var]
     */
    inline int16_t reactivePower() const { return parse_int16(10); }

    /**
     * Power factor. The sign of power factor should be the sign of active power.
     */
    inline int16_t powerFactor() const { return parse_int16(11); }

    /**
     * Total AC current. [A]
     */
    inline int16_t totalACCurrent() const { return parse_int16(12); }

    /**
     * Line to line AC voltage as an average of active phases. [V]
     */
    inline uint16_t voltageLL() const { return parse_uint16(13); }

    /**
     * Line to neutral AC voltage as an average of active phases. [V]
     */
    inline uint16_t voltageLN() const { return parse_uint16(14); }

    /**
     * AC frequency. [Hz]
     */
    inline uint32_t frequency() const { return parse_uint32(15); }

    /**
     * Total active energy injected (Quadrants 1 & 4). [Wh]
     */
    inline uint64_t totalEnergyInjected() const { return parse_uint64(17); }

    /**
     * Total active energy absorbed (Quadrants 2 & 3). [Wh]
     */
    inline uint64_t totalEnergyAbsorbed() const { return parse_uint64(21); }

    /**
     * Total reactive energy injected (Quadrants 1 & 2). [Varh]
     */
    inline uint64_t totalReactiveEnergyInj() const { return parse_uint64(25); }

    /**
     * Total reactive energy absorbed (Quadrants 3 & 4). [Varh]
     */
    inline uint64_t totalReactiveEnergyAbs() const { return parse_uint64(29); }

    /**
     * Ambient temperature. [C]
     */
    inline int16_t ambientTemperature() const { return parse_int16(33); }

    /**
     * Cabinet temperature. [C]
     */
    inline int16_t cabinetTemperature() const { return parse_int16(34); }

    /**
     * Heat sink temperature. [C]
     */
    inline int16_t heatSinkTemperature() const { return parse_int16(35); }

    /**
     * Transformer temperature. [C]
     */
    inline int16_t transformerTemperature() const { return parse_int16(36); }

    /**
     * IGBT/MOSFET temperature. [C]
     */
    inline int16_t IGBTMOSFETTemperature() const { return parse_int16(37); }

    /**
     * Other temperature. [C]
     */
    inline int16_t otherTemperature() const { return parse_int16(38); }

    /**
     * Active power L1. [W]
     */
    inline int16_t wattsL1() const { return parse_int16(39); }

    /**
     * Apparent power L1. [VA]
     */
    inline int16_t VAL1() const { return parse_int16(40); }

    /**
     * Reactive power L1. [Var]
     */
    inline int16_t varL1() const { return parse_int16(41); }

    /**
     * Power factor phase L1.
     */
    inline int16_t PFL1() const { return parse_int16(42); }

    /**
     * Current phase L1. [A]
     */
    inline int16_t ampsL1() const { return parse_int16(43); }

    /**
     * Phase voltage L1-L2. [V]
     */
    inline uint16_t phaseVoltageL1L2() const { return parse_uint16(44); }

    /**
     * Phase voltage L1-N. [V]
     */
    inline uint16_t phaseVoltageL1N() const { return parse_uint16(45); }

    /**
     * Total active energy injected L1. [Wh]
     */
    inline uint64_t totalWattHoursInjL1() const { return parse_uint64(46); }

    /**
     * Total active energy absorbed L1. [Wh]
     */
    inline uint64_t totalWattHoursAbsL1() const { return parse_uint64(50); }

    /**
     * Total reactive energy injected L1. [Varh]
     */
    inline uint64_t totalVarHoursInjL1() const { return parse_uint64(54); }

    /**
     * Total reactive energy absorbed L1. [Varh]
     */
    inline uint64_t totalVarHoursAbsL1() const { return parse_uint64(58); }

    /**
     * Active power L2. [W]
     */
    inline int16_t wattsL2() const { return parse_int16(62); }

    /**
     * Apparent power L2. [VA]
     */
    inline int16_t VAL2() const { return parse_int16(63); }

    /**
     * Reactive power L2. [Var]
     */
    inline int16_t varL2() const { return parse_int16(64); }

    /**
     * Power factor L2.
     */
    inline int16_t PFL2() const { return parse_int16(65); }

    /**
     * Current L2. [A]
     */
    inline int16_t ampsL2() const { return parse_int16(66); }

    /**
     * Phase voltage L2-L3. [V]
     */
    inline uint16_t phaseVoltageL2L3() const { return parse_uint16(67); }

    /**
     * Phase voltage L2-N. [V]
     */
    inline uint16_t phaseVoltageL2N() const { return parse_uint16(68); }

    /**
     * Total active energy injected L2. [Wh]
     */
    inline uint64_t totalWattHoursInjL2() const { return parse_uint64(69); }

    /**
     * Total active energy absorbed L2. [Wh]
     */
    inline uint64_t totalWattHoursAbsL2() const { return parse_uint64(73); }

    /**
     * Total reactive energy injected L2. [Varh]
     */
    inline uint64_t totalVarHoursInjL2() const { return parse_uint64(77); }

    /**
     * Total reactive energy absorbed L2. [Varh]
     */
    inline uint64_t totalVarHoursAbsL2() const { return parse_uint64(81); }

    /**
     * Active power L3. [W]
     */
    inline int16_t wattsL3() const { return parse_int16(85); }

    /**
     * Apparent power L3. [VA]
     */
    inline int16_t VAL3() const { return parse_int16(86); }

    /**
     * Reactive power L3. [Var]
     */
    inline int16_t varL3() const { return parse_int16(87); }

    /**
     * Power factor L3.
     */
    inline int16_t PFL3() const { return parse_int16(88); }

    /**
     * Current L3. [A]
     */
    inline int16_t ampsL3() const { return parse_int16(89); }

    /**
     * Phase voltage L3-L1. [V]
     */
    inline uint16_t phaseVoltageL3L1() const { return parse_uint16(90); }

    /**
     * Phase voltage L3-N. [V]
     */
    inline uint16_t phaseVoltageL3N() const { return parse_uint16(91); }

    /**
     * Total active energy injected L3. [Wh]
     */
    inline uint64_t totalWattHoursInjL3() const { return parse_uint64(92); }

    /**
     * Total active energy absorbed L3. [Wh]
     */
    inline uint64_t totalWattHoursAbsL3() const { return parse_uint64(96); }

    /**
     * Total reactive energy injected L3. [Varh]
     */
    inline uint64_t totalVarHoursInjL3() const { return parse_uint64(100); }

    /**
     * Total reactive energy absorbed L3. [Varh]
     */
    inline uint64_t totalVarHoursAbsL3() const { return parse_uint64(104); }

    /**
     * Throttling in pct of maximum active power. [Pct]
     */
    inline uint16_t throttlingInPct() const { return parse_uint16(108); }

    /**
     * Active throttling source.
     */
    inline uint32_t throttleSourceInformation() const { return parse_bitfield32(109); }

    /**
     * Current scale factor.
     */
    inline int16_t currentScaleFactor() const { return parse_sunssf(111); }

    /**
     * Voltage scale factor.
     */
    inline int16_t voltageScaleFactor() const { return parse_sunssf(112); }

    /**
     * Frequency scale factor.
     */
    inline int16_t frequencyScaleFactor() const { return parse_sunssf(113); }

    /**
     * Active power scale factor.
     */
    inline int16_t activePowerScaleFactor() const { return parse_sunssf(114); }

    /**
     * Power factor scale factor.
     */
    inline int16_t powerFactorScaleFactor() const { return parse_sunssf(115); }

    /**
     * Apparent power scale factor.
     */
    inline int16_t apparentPowerScaleFactor() const { return parse_sunssf(116); }

    /**
     * Reactive power scale factor.
     */
    inline int16_t reactivePowerScaleFactor() const { return parse_sunssf(117); }

    /**
     * Active energy scale factor.
     */
    inline int16_t activeEnergyScaleFactor() const { return parse_sunssf(118); }

    /**
     * Reactive energy scale factor.
     */
    inline int16_t reactiveEnergyScaleFactor() const { return parse_sunssf(119); }

    /**
     * Temperature scale factor.
     */
    inline int16_t temperatureScaleFactor() const { return parse_sunssf(120); }

    /**
     * Manufacturer alarm information. Valid if MANUFACTURER_ALRM indication is active.
     */
    inline String manufacturerAlarmInfo() const { return parse_string(121, 32); }

};

/**
 * DER capacity model.
 */
class DERCapacity : public SunSpecModel<702, 50> {
  public:
    /**
     * Maximum active power rating at unity power factor in watts. [W]
     */
    inline uint16_t activePowerMaxRating() const { return parse_uint16(0); }

    /**
     * Active power rating at specified over-excited power factor in watts. [W]
     */
    inline uint16_t activePowerOverExcitedRating() const { return parse_uint16(1); }

    /**
     * Specified over-excited power factor.
     */
    inline uint16_t specifiedOverExcitedPF_2() const { return parse_uint16(2); }

    /**
     * Active power rating at specified under-excited power factor in watts. [W]
     */
    inline uint16_t activePowerUnderExcitedRating() const { return parse_uint16(3); }

    /**
     * Specified under-excited power factor.
     */
    inline uint16_t specifiedUnderExcitedPF_4() const { return parse_uint16(4); }

    /**
     * Maximum apparent power rating in voltamperes. [VA]
     */
    inline uint16_t apparentPowerMaxRating() const { return parse_uint16(5); }

    /**
     * Maximum injected reactive power rating in vars. [Var]
     */
    inline uint16_t reactivePowerInjectedRating() const { return parse_uint16(6); }

    /**
     * Maximum absorbed reactive power rating in vars. [Var]
     */
    inline uint16_t reactivePowerAbsorbedRating() const { return parse_uint16(7); }

    /**
     * Maximum active power charge rate in watts. [W]
     */
    inline uint16_t chargeRateMaxRating() const { return parse_uint16(8); }

    /**
     * Maximum active power discharge rate in watts. [W]
     */
    inline uint16_t dischargeRateMaxRating() const { return parse_uint16(9); }

    /**
     * Maximum apparent power charge rate in voltamperes. [VA]
     */
    inline uint16_t chargeRateMaxVARating() const { return parse_uint16(10); }

    /**
     * Maximum apparent power discharge rate in voltamperes. [VA]
     */
    inline uint16_t dischargeRateMaxVARating() const { return parse_uint16(11); }

    /**
     * AC voltage nominal rating. [V]
     */
    inline uint16_t ACVoltageNominalRating() const { return parse_uint16(12); }

    /**
     * AC voltage maximum rating. [V]
     */
    inline uint16_t ACVoltageMaxRating() const { return parse_uint16(13); }

    /**
     * AC voltage minimum rating. [V]
     */
    inline uint16_t ACVoltageMinRating() const { return parse_uint16(14); }

    /**
     * AC current maximum rating in amps. [A]
     */
    inline uint16_t ACCurrentMaxRating() const { return parse_uint16(15); }

    /**
     * Power factor over-excited rating.
     */
    inline uint16_t PFOverExcitedRating() const { return parse_uint16(16); }

    /**
     * Power factor under-excited rating.
     */
    inline uint16_t PFUnderExcitedRating() const { return parse_uint16(17); }

    /**
     * Reactive susceptance that remains connected to the Area EPS in the cease to energize and trip state. [S]
     */
    inline uint16_t reactiveSusceptance() const { return parse_uint16(18); }

    /**
     * Normal operating performance category as specified in IEEE 1547-2018.
     */
    inline uint16_t normalOperatingCategory() const { return parse_enum16(19); }

    /**
     * Abnormal operating performance category as specified in IEEE 1547-2018.
     */
    inline uint16_t abnormalOperatingCategory() const { return parse_enum16(20); }

    /**
     * Supported control mode functions.
     */
    inline uint32_t supportedControlModes() const { return parse_bitfield32(21); }

    /**
     * Intentional island categories.
     */
    inline uint16_t intentionalIslandCategories_23() const { return parse_bitfield16(23); }

    /**
     * Maximum active power setting used to adjust maximum active power setting. [W]
     */
    inline uint16_t activePowerMaxSetting() const { return parse_uint16(24); }

    /**
     * Active power setting at specified over-excited power factor in watts. [W]
     */
    inline uint16_t activePowerOverExcitedSetting() const { return parse_uint16(25); }

    /**
     * Specified over-excited power factor.
     */
    inline uint16_t specifiedOverExcitedPF_26() const { return parse_uint16(26); }

    /**
     * Active power setting at specified under-excited power factor in watts. [W]
     */
    inline uint16_t activePowerUnderExcitedSetting() const { return parse_uint16(27); }

    /**
     * Specified under-excited power factor.
     */
    inline uint16_t specifiedUnderExcitedPF_28() const { return parse_uint16(28); }

    /**
     * Maximum apparent power setting used to adjust maximum apparent power rating. [VA]
     */
    inline uint16_t apparentPowerMaxSetting() const { return parse_uint16(29); }

    /**
     * Maximum injected reactive power setting used to adjust maximum injected reactive power rating. [Var]
     */
    inline uint16_t reactivePowerInjectedSetting() const { return parse_uint16(30); }

    /**
     * Maximum absorbed reactive power setting used to adjust maximum absorbed reactive power rating. [Var]
     */
    inline uint16_t reactivePowerAbsorbedSetting() const { return parse_uint16(31); }

    /**
     * Maximum active power charge rate setting used to adjust maximum active power charge rate rating. [W]
     */
    inline uint16_t chargeRateMaxSetting() const { return parse_uint16(32); }

    /**
     * Maximum active power discharge rate setting used to adjust maximum active power discharge rate rating. [W]
     */
    inline uint16_t dischargeRateMaxSetting() const { return parse_uint16(33); }

    /**
     * Maximum apparent power charge rate setting used to adjust maximum apparent power charge rate rating. [VA]
     */
    inline uint16_t chargeRateMaxVASetting() const { return parse_uint16(34); }

    /**
     * Maximum apparent power discharge rate setting used to adjust maximum apparent power discharge rate rating. [VA]
     */
    inline uint16_t dischargeRateMaxVASetting() const { return parse_uint16(35); }

    /**
     * Nominal AC voltage setting. [V]
     */
    inline uint16_t nominalACVoltageSetting() const { return parse_uint16(36); }

    /**
     * AC voltage maximum setting used to adjust AC voltage maximum rating. [V]
     */
    inline uint16_t ACVoltageMaxSetting() const { return parse_uint16(37); }

    /**
     * AC voltage minimum setting used to adjust AC voltage minimum rating. [V]
     */
    inline uint16_t ACVoltageMinSetting() const { return parse_uint16(38); }

    /**
     * Maximum AC current setting used to adjust maximum AC current rating. [A]
     */
    inline uint16_t ACCurrentMaxSetting() const { return parse_uint16(39); }

    /**
     * Power factor over-excited setting.
     */
    inline uint16_t PFOverExcitedSetting() const { return parse_uint16(40); }

    /**
     * Power factor under-excited setting.
     */
    inline uint16_t PFUnderExcitedSetting() const { return parse_uint16(41); }

    /**
     * Intentional island categories.
     */
    inline uint16_t intentionalIslandCategories_42() const { return parse_bitfield16(42); }

    /**
     * Active power scale factor.
     */
    inline int16_t activePowerScaleFactor() const { return parse_sunssf(43); }

    /**
     * Power factor scale factor.
     */
    inline int16_t powerFactorScaleFactor() const { return parse_sunssf(44); }

    /**
     * Apparent power scale factor.
     */
    inline int16_t apparentPowerScaleFactor() const { return parse_sunssf(45); }

    /**
     * Reactive power scale factor.
     */
    inline int16_t reactivePowerScaleFactor() const { return parse_sunssf(46); }

    /**
     * Voltage scale factor.
     */
    inline int16_t voltageScaleFactor() const { return parse_sunssf(47); }

    /**
     * Current scale factor.
     */
    inline int16_t currentScaleFactor() const { return parse_sunssf(48); }

    /**
     * Susceptance scale factor.
     */
    inline int16_t susceptanceScaleFactor() const { return parse_sunssf(49); }

};

/**
 * Enter service model.
 */
class EnterService : public SunSpecModel<703, 17> {
  public:
    /**
     * Permit enter service.
     */
    inline uint16_t permitEnterService() const { return parse_enum16(0); }

    /**
     * Enter service voltage high threshold as percent of normal voltage. [Pct]
     */
    inline uint16_t enterServiceVoltageHigh() const { return parse_uint16(1); }

    /**
     * Enter service voltage low threshold as percent of normal voltage. [Pct]
     */
    inline uint16_t enterServiceVoltageLow() const { return parse_uint16(2); }

    /**
     * Enter service frequency high threshold. [Hz]
     */
    inline uint32_t enterServiceFrequencyHigh() const { return parse_uint32(3); }

    /**
     * Enter service frequency low threshold. [Hz]
     */
    inline uint32_t enterServiceFrequencyLow() const { return parse_uint32(5); }

    /**
     * Enter service delay time in seconds. [Secs]
     */
    inline uint32_t enterServiceDelayTime() const { return parse_uint32(7); }

    /**
     * Enter service random delay in seconds. [Secs]
     */
    inline uint32_t enterServiceRandomDelay() const { return parse_uint32(9); }

    /**
     * Enter service ramp time in seconds. [Secs]
     */
    inline uint32_t enterServiceRampTime() const { return parse_uint32(11); }

    /**
     * Enter service delay time remaining in seconds. [Secs]
     */
    inline uint32_t enterServiceDelayRemaining() const { return parse_uint32(13); }

    /**
     * Voltage percentage scale factor.
     */
    inline int16_t voltageScaleFactor() const { return parse_sunssf(15); }

    /**
     * Frequency scale factor.
     */
    inline int16_t frequencyScaleFactor() const { return parse_sunssf(16); }

};

/**
 * DER storage capacity.
 */
class DERStorageCapacity : public SunSpecModel<713, 7> {
  public:
    /**
     * Energy rating of the DER storage. [WH]
     */
    inline uint16_t energyRating() const { return parse_uint16(0); }

    /**
     * Energy available of the DER storage (WHAvail = WHRtg * SoC * SoH) [WH]
     */
    inline uint16_t energyAvailable() const { return parse_uint16(1); }

    /**
     * State of charge of the DER storage. [Pct]
     */
    inline uint16_t stateOfCharge() const { return parse_uint16(2); }

    /**
     * State of health of the DER storage. [Pct]
     */
    inline uint16_t stateOfHealth() const { return parse_uint16(3); }

    /**
     * Storage status.
     */
    inline uint16_t status() const { return parse_enum16(4); }

    /**
     * Scale factor for energy capacity.
     */
    inline int16_t energyScaleFactor() const { return parse_sunssf(5); }

    /**
     * Scale factor for percentage.
     */
    inline int16_t percentScaleFactor() const { return parse_sunssf(6); }

};

/**
 * DER Control
 */
class DERCtl : public SunSpecModel<715, 7> {
  public:
    /**
     * DER control mode. Enumeration.
     */
    inline uint16_t controlMode() const { return parse_enum16(0); }

    /**
     * Value is incremented every second by the DER with periodic resets to zero.
     */
    inline uint32_t DERHeartbeat() const { return parse_uint32(1); }

    /**
     * Value is incremented every second by the controller with periodic resets to zero.
     */
    inline uint32_t controllerHeartbeat() const { return parse_uint32(3); }

    /**
     * Used to reset any latched alarms. 1 = Reset.
     */
    inline uint16_t alarmReset() const { return parse_uint16(5); }

    /**
     * Commands to PCS. Enumerated value.
     */
    inline uint16_t setOperation() const { return parse_enum16(6); }

};

/**
 * This model has been deprecated.
 */
class EnergyStorageBaseModelDEPRECATED : public SunSpecModel<801, 1> {
  public:
    /**
     * This model has been deprecated.
     */
    inline uint16_t deprecatedModel() const { return parse_enum16(0); }

};

class BatteryBaseModel : public SunSpecModel<802, 62> {
  public:
    /**
     * Nameplate charge capacity in amp-hours. [Ah]
     */
    inline uint16_t nameplateChargeCapacity() const { return parse_uint16(0); }

    /**
     * Nameplate energy capacity in DC watt-hours. [Wh]
     */
    inline uint16_t nameplateEnergyCapacity() const { return parse_uint16(1); }

    /**
     * Maximum rate of energy transfer into the storage device in DC watts. [W]
     */
    inline uint16_t nameplateMaxChargeRate() const { return parse_uint16(2); }

    /**
     * Maximum rate of energy transfer out of the storage device in DC watts. [W]
     */
    inline uint16_t nameplateMaxDischargeRate() const { return parse_uint16(3); }

    /**
     * Self discharge rate.  Percentage of capacity (WHRtg) discharged per day. [%WHRtg]
     */
    inline uint16_t selfDischargeRate() const { return parse_uint16(4); }

    /**
     * Manufacturer maximum state of charge, expressed as a percentage. [%WHRtg]
     */
    inline uint16_t nameplateMaxSoC() const { return parse_uint16(5); }

    /**
     * Manufacturer minimum state of charge, expressed as a percentage. [%WHRtg]
     */
    inline uint16_t nameplateMinSoC() const { return parse_uint16(6); }

    /**
     * Setpoint for maximum reserve for storage as a percentage of the nominal maximum storage. [%WHRtg]
     */
    inline uint16_t maxReservePercent() const { return parse_uint16(7); }

    /**
     * Setpoint for minimum reserve for storage as a percentage of the nominal maximum storage. [%WHRtg]
     */
    inline uint16_t minReservePercent() const { return parse_uint16(8); }

    /**
     * State of charge, expressed as a percentage. [%WHRtg]
     */
    inline uint16_t stateOfCharge() const { return parse_uint16(9); }

    /**
     * Depth of discharge, expressed as a percentage. [%]
     */
    inline uint16_t depthOfDischarge() const { return parse_uint16(10); }

    /**
     * Percentage of battery life remaining. [%]
     */
    inline uint16_t stateOfHealth() const { return parse_uint16(11); }

    /**
     * Number of cycles executed in the battery.
     */
    inline uint32_t cycleCount() const { return parse_uint32(12); }

    /**
     * Charge status of storage device. Enumeration.
     */
    inline uint16_t chargeStatus() const { return parse_enum16(14); }

    /**
     * Battery control mode. Enumeration.
     */
    inline uint16_t controlMode() const { return parse_enum16(15); }

    /**
     * Value is incremented every second with periodic resets to zero.
     */
    inline uint16_t batteryHeartbeat() const { return parse_uint16(16); }

    /**
     * Value is incremented every second with periodic resets to zero.
     */
    inline uint16_t controllerHeartbeat() const { return parse_uint16(17); }

    /**
     * Used to reset any latched alarms.  1 = Reset.
     */
    inline uint16_t alarmReset() const { return parse_uint16(18); }

    /**
     * Type of battery. Enumeration.
     */
    inline uint16_t batteryType() const { return parse_enum16(19); }

    /**
     * State of the battery bank.  Enumeration.
     */
    inline uint16_t stateOfTheBatteryBank() const { return parse_enum16(20); }

    /**
     * Vendor specific battery bank state.  Enumeration.
     */
    inline uint16_t vendorBatteryBankState() const { return parse_enum16(21); }

    /**
     * Date the device warranty expires.
     */
    inline uint32_t warrantyDate() const { return parse_uint32(22); }

    /**
     * Alarms and warnings.  Bit flags.
     */
    inline uint32_t batteryEvent1Bitfield() const { return parse_bitfield32(24); }

    /**
     * Alarms and warnings.  Bit flags.
     */
    inline uint32_t batteryEvent2Bitfield() const { return parse_bitfield32(26); }

    /**
     * Vendor defined events.
     */
    inline uint32_t vendorEventBitfield1() const { return parse_bitfield32(28); }

    /**
     * Vendor defined events.
     */
    inline uint32_t vendorEventBitfield2() const { return parse_bitfield32(30); }

    /**
     * DC Bus Voltage. [V]
     */
    inline uint16_t externalBatteryVoltage() const { return parse_uint16(32); }

    /**
     * Instantaneous maximum battery voltage. [V]
     */
    inline uint16_t maxBatteryVoltage() const { return parse_uint16(33); }

    /**
     * Instantaneous minimum battery voltage. [V]
     */
    inline uint16_t minBatteryVoltage() const { return parse_uint16(34); }

    /**
     * Maximum voltage for all cells in the bank. [V]
     */
    inline uint16_t maxCellVoltage() const { return parse_uint16(35); }

    /**
     * String containing the cell with maximum voltage.
     */
    inline uint16_t maxCellVoltageString() const { return parse_uint16(36); }

    /**
     * Module containing the cell with maximum voltage.
     */
    inline uint16_t maxCellVoltageModule() const { return parse_uint16(37); }

    /**
     * Minimum voltage for all cells in the bank. [V]
     */
    inline uint16_t minCellVoltage() const { return parse_uint16(38); }

    /**
     * String containing the cell with minimum voltage.
     */
    inline uint16_t minCellVoltageString() const { return parse_uint16(39); }

    /**
     * Module containing the cell with minimum voltage.
     */
    inline uint16_t minCellVoltageModule() const { return parse_uint16(40); }

    /**
     * Average cell voltage for all cells in the bank. [V]
     */
    inline uint16_t averageCellVoltage() const { return parse_uint16(41); }

    /**
     * Total DC current flowing to/from the battery bank. [A]
     */
    inline int16_t totalDCCurrent() const { return parse_int16(42); }

    /**
     * Instantaneous maximum DC charge current. [A]
     */
    inline uint16_t maxChargeCurrent() const { return parse_uint16(43); }

    /**
     * Instantaneous maximum DC discharge current. [A]
     */
    inline uint16_t maxDischargeCurrent() const { return parse_uint16(44); }

    /**
     * Total power flowing to/from the battery bank. [W]
     */
    inline int16_t totalPower() const { return parse_int16(45); }

    /**
     * Request from battery to start or stop the inverter.  Enumeration.
     */
    inline uint16_t inverterStateRequest() const { return parse_enum16(46); }

    /**
     * AC Power requested by battery. [W]
     */
    inline int16_t batteryPowerRequest() const { return parse_int16(47); }

    /**
     * Instruct the battery bank to perform an operation such as connecting.  Enumeration.
     */
    inline uint16_t setOperation() const { return parse_enum16(48); }

    /**
     * Set the current state of the inverter.
     */
    inline uint16_t setInverterState() const { return parse_enum16(49); }

    /**
     * Scale factor for charge capacity.
     */
    inline int16_t aHRtg_SF() const { return parse_sunssf(50); }

    /**
     * Scale factor for energy capacity.
     */
    inline int16_t wHRtg_SF() const { return parse_sunssf(51); }

    /**
     * Scale factor for maximum charge and discharge rate.
     */
    inline int16_t wChaDisChaMax_SF() const { return parse_sunssf(52); }

    /**
     * Scale factor for self discharge rate.
     */
    inline int16_t disChaRte_SF() const { return parse_sunssf(53); }

    /**
     * Scale factor for state of charge values.
     */
    inline int16_t soC_SF() const { return parse_sunssf(54); }

    /**
     * Scale factor for depth of discharge.
     */
    inline int16_t doD_SF() const { return parse_sunssf(55); }

    /**
     * Scale factor for state of health.
     */
    inline int16_t soH_SF() const { return parse_sunssf(56); }

    /**
     * Scale factor for DC bus voltage.
     */
    inline int16_t V_SF() const { return parse_sunssf(57); }

    /**
     * Scale factor for cell voltage.
     */
    inline int16_t cellV_SF() const { return parse_sunssf(58); }

    /**
     * Scale factor for DC current.
     */
    inline int16_t A_SF() const { return parse_sunssf(59); }

    /**
     * Scale factor for instantaneous DC charge/discharge current.
     */
    inline int16_t aMax_SF() const { return parse_sunssf(60); }

    /**
     * Scale factor for AC power request.
     */
    inline int16_t W_SF() const { return parse_sunssf(61); }

};

class VerisStatusAndConfiguration : public SunSpecModel<64001, 71> {
  public:
    inline uint16_t commandCode() const { return parse_enum16(0); }

    inline uint16_t hardwareRevision() const { return parse_uint16(1); }

    inline uint16_t RSFWRevision() const { return parse_uint16(2); }

    inline uint16_t OSFWRevision() const { return parse_uint16(3); }

    inline String productRevision() const { return parse_string(4, 2); }

    inline uint16_t bootCount() const { return parse_uint16(6); }

    inline uint16_t DIPSwitches() const { return parse_bitfield16(7); }

    inline uint16_t numDetectedSensors() const { return parse_uint16(8); }

    inline uint16_t numCommunicatingSensors() const { return parse_uint16(9); }

    inline uint16_t systemStatus() const { return parse_bitfield16(10); }

    inline uint16_t systemConfiguration() const { return parse_bitfield16(11); }

    /**
     * [Pct]
     */
    inline uint16_t LEDBlinkThreshold() const { return parse_uint16(12); }

    /**
     * [Pct]
     */
    inline uint16_t LEDOnThreshold() const { return parse_uint16(13); }

    inline uint16_t reserved() const { return parse_uint16(14); }

    inline String locationString() const { return parse_string(15, 16); }

    inline uint16_t sensor1UnitID() const { return parse_enum16(31); }

    inline uint16_t sensor1Address() const { return parse_uint16(32); }

    inline uint16_t sensor1OSVersion() const { return parse_uint16(33); }

    inline String sensor1ProductVersion() const { return parse_string(34, 2); }

    inline String sensor1SerialNum() const { return parse_string(36, 5); }

    inline uint16_t sensor2UnitID() const { return parse_enum16(41); }

    inline uint16_t sensor2Address() const { return parse_uint16(42); }

    inline uint16_t sensor2OSVersion() const { return parse_uint16(43); }

    inline String sensor2ProductVersion() const { return parse_string(44, 2); }

    inline String sensor2SerialNum() const { return parse_string(46, 5); }

    inline uint16_t sensor3UnitID() const { return parse_enum16(51); }

    inline uint16_t sensor3Address() const { return parse_uint16(52); }

    inline uint16_t sensor3OSVersion() const { return parse_uint16(53); }

    inline String sensor3ProductVersion() const { return parse_string(54, 2); }

    inline String sensor3SerialNum() const { return parse_string(56, 5); }

    inline uint16_t sensor4UnitID() const { return parse_enum16(61); }

    inline uint16_t sensor4Address() const { return parse_uint16(62); }

    inline uint16_t sensor4OSVersion() const { return parse_uint16(63); }

    inline String sensor4ProductVersion() const { return parse_string(64, 2); }

    inline String sensor4SerialNum() const { return parse_string(66, 5); }

};

class EltekInverterExtension : public SunSpecModel<64101, 7> {
  public:
    inline uint16_t eltek_Country_Code() const { return parse_uint16(0); }

    inline uint16_t eltek_Feeding_Phase() const { return parse_uint16(1); }

    inline uint16_t eltek_APD_Method() const { return parse_uint16(2); }

    inline uint16_t eltek_APD_Power_Ref() const { return parse_uint16(3); }

    inline uint16_t eltek_RPS_Method() const { return parse_uint16(4); }

    inline uint16_t eltek_RPS_Q_Ref() const { return parse_uint16(5); }

    inline int16_t eltek_RPS_CosPhi_Ref() const { return parse_int16(6); }

};

class OutBackAXSDevice : public SunSpecModel<64110, 282> {
  public:
    inline uint16_t AXSMajorFirmwareNumber() const { return parse_uint16(0); }

    inline uint16_t AXSMidFirmwareNumber() const { return parse_uint16(1); }

    inline uint16_t AXSMinorFirmwareNumber() const { return parse_uint16(2); }

    inline uint16_t encryptionKey() const { return parse_uint16(3); }

    inline String MACAddress() const { return parse_string(4, 7); }

    inline String writePassword() const { return parse_string(11, 8); }

    inline uint16_t enableDHCP() const { return parse_enum16(19); }

    inline uint32_t TCPIPAddress() const { return parse_ipaddr(20); }

    inline uint32_t TCPIPGateway() const { return parse_ipaddr(22); }

    inline uint32_t TCPIPNetmask() const { return parse_ipaddr(24); }

    inline uint32_t TCPIPDNS1() const { return parse_ipaddr(26); }

    inline uint32_t TCPIPDNS2() const { return parse_ipaddr(28); }

    inline uint16_t modBusPort() const { return parse_uint16(30); }

    inline String SMTPServerName() const { return parse_string(31, 20); }

    inline String SMTPAccountName() const { return parse_string(51, 16); }

    inline uint16_t enableSMTPSSL() const { return parse_enum16(67); }

    inline String SMTPPassword() const { return parse_string(68, 8); }

    inline String SMTPUserName() const { return parse_string(76, 20); }

    inline uint16_t statusEmailInterval() const { return parse_uint16(96); }

    inline uint16_t statusEmailStartHour() const { return parse_uint16(97); }

    inline String statusEmailSubject() const { return parse_string(98, 25); }

    inline String statusEmailToAddress1() const { return parse_string(123, 20); }

    inline String statusEmailToAddress2() const { return parse_string(143, 20); }

    inline uint16_t enableAlarmEmail() const { return parse_enum16(163); }

    inline String alarmEmailSubject() const { return parse_string(164, 25); }

    inline String alarmEmailToAddress1() const { return parse_string(189, 20); }

    inline String alarmEmailToAddress2() const { return parse_string(209, 20); }

    inline String FTPPassword() const { return parse_string(229, 8); }

    inline String telnetPassword() const { return parse_string(237, 8); }

    /**
     * [Tms]
     */
    inline uint16_t SDCardDatalogWriteInterval() const { return parse_uint16(245); }

    /**
     * [Tmd]
     */
    inline uint16_t SDCardDatalogRetain() const { return parse_uint16(246); }

    inline uint16_t SDCardDatalogMode() const { return parse_enum16(247); }

    inline String NTPTimerServerName() const { return parse_string(248, 20); }

    inline uint16_t enableNetworkTime() const { return parse_enum16(268); }

    /**
     * [Tmh]
     */
    inline int16_t timeZone() const { return parse_int16(269); }

    inline uint16_t year() const { return parse_uint16(270); }

    inline uint16_t month() const { return parse_uint16(271); }

    inline uint16_t day() const { return parse_uint16(272); }

    inline uint16_t hour() const { return parse_uint16(273); }

    inline uint16_t minute() const { return parse_uint16(274); }

    inline uint16_t second() const { return parse_uint16(275); }

    /**
     * [C]
     */
    inline int16_t batteryTemperature() const { return parse_int16(276); }

    /**
     * [C]
     */
    inline int16_t ambientTemperature() const { return parse_int16(277); }

    inline int16_t temp_SF() const { return parse_sunssf(278); }

    inline uint16_t AXSError() const { return parse_bitfield16(279); }

    inline uint16_t AXSStatus() const { return parse_bitfield16(280); }

    inline uint16_t spare() const { return parse_uint16(281); }

};

class BasicChargeController : public SunSpecModel<64111, 23> {
  public:
    inline uint16_t portNumber() const { return parse_uint16(0); }

    inline int16_t V_SF() const { return parse_sunssf(1); }

    inline int16_t A_SF() const { return parse_sunssf(2); }

    inline int16_t P_SF() const { return parse_sunssf(3); }

    inline int16_t AH_SF() const { return parse_sunssf(4); }

    inline int16_t KWH_SF() const { return parse_sunssf(5); }

    /**
     * [V]
     */
    inline uint16_t batteryVoltage() const { return parse_uint16(6); }

    /**
     * [V]
     */
    inline uint16_t arrayVoltage() const { return parse_uint16(7); }

    /**
     * [A]
     */
    inline uint16_t outputCurrent() const { return parse_uint16(8); }

    /**
     * [A]
     */
    inline uint16_t arrayCurrent() const { return parse_uint16(9); }

    inline uint16_t operatingState() const { return parse_enum16(10); }

    /**
     * [W]
     */
    inline uint16_t outputWattage() const { return parse_uint16(11); }

    /**
     * [V]
     */
    inline uint16_t todaySMinimumBatteryVoltage() const { return parse_uint16(12); }

    /**
     * [V]
     */
    inline uint16_t todaySMaximumBatteryVoltage() const { return parse_uint16(13); }

    /**
     * [V]
     */
    inline uint16_t VOC() const { return parse_uint16(14); }

    /**
     * [V]
     */
    inline uint16_t todaySMaximumVOC() const { return parse_uint16(15); }

    /**
     * [kWh]
     */
    inline uint16_t todaySKWh() const { return parse_uint16(16); }

    /**
     * [AH]
     */
    inline uint16_t todaySAH() const { return parse_uint16(17); }

    /**
     * [kWh]
     */
    inline uint16_t lifetimeKWh() const { return parse_uint16(18); }

    /**
     * [kAH]
     */
    inline uint16_t lifetimeKAH() const { return parse_uint16(19); }

    /**
     * [W]
     */
    inline uint16_t lifetimeMaximumOutputWattage() const { return parse_uint16(20); }

    /**
     * [V]
     */
    inline uint16_t lifetimeMaximumBatteryVoltage() const { return parse_uint16(21); }

    /**
     * [V]
     */
    inline uint16_t lifetimeMaximumVOCVoltage() const { return parse_uint16(22); }

};

class OutBackFMChargeController : public SunSpecModel<64112, 64> {
  public:
    inline uint16_t portNumber() const { return parse_uint16(0); }

    inline int16_t V_SF() const { return parse_sunssf(1); }

    inline int16_t C_SF() const { return parse_sunssf(2); }

    inline int16_t H_SF() const { return parse_sunssf(3); }

    inline int16_t P_SF() const { return parse_sunssf(4); }

    inline int16_t AH_SF() const { return parse_sunssf(5); }

    inline int16_t KWH_SF() const { return parse_sunssf(6); }

    inline uint16_t faults() const { return parse_bitfield16(7); }

    /**
     * [V]
     */
    inline uint16_t absorb() const { return parse_uint16(8); }

    /**
     * [Tmh]
     */
    inline uint16_t absorbTime() const { return parse_uint16(9); }

    /**
     * [A]
     */
    inline uint16_t absorbEnd() const { return parse_uint16(10); }

    /**
     * [V]
     */
    inline uint16_t rebulk() const { return parse_uint16(11); }

    /**
     * [V]
     */
    inline uint16_t float_() const { return parse_uint16(12); }

    /**
     * [A]
     */
    inline uint16_t maximumCharge() const { return parse_uint16(13); }

    /**
     * [V]
     */
    inline uint16_t equalize() const { return parse_uint16(14); }

    /**
     * [Tmh]
     */
    inline uint16_t equalizeTime() const { return parse_uint16(15); }

    /**
     * [Tmd]
     */
    inline uint16_t autoEqualizeInterval() const { return parse_uint16(16); }

    inline uint16_t MPPTMode() const { return parse_enum16(17); }

    inline uint16_t sweepWidth() const { return parse_enum16(18); }

    inline uint16_t sweepMaximum() const { return parse_enum16(19); }

    /**
     * [Pct]
     */
    inline uint16_t UPickPWMDutyCycle() const { return parse_uint16(20); }

    inline uint16_t gridTieMode() const { return parse_enum16(21); }

    inline uint16_t tempCompMode() const { return parse_enum16(22); }

    /**
     * [V]
     */
    inline uint16_t tempCompLowerLimit() const { return parse_uint16(23); }

    /**
     * [V]
     */
    inline uint16_t tempCompUpperLimit() const { return parse_uint16(24); }

    inline uint16_t autoRestartMode() const { return parse_enum16(25); }

    /**
     * [V]
     */
    inline uint16_t wakeupVOCChange() const { return parse_uint16(26); }

    /**
     * [A]
     */
    inline uint16_t snoozeMode() const { return parse_uint16(27); }

    /**
     * [Tms]
     */
    inline uint16_t wakeupInterval() const { return parse_uint16(28); }

    inline uint16_t AUXOutputMode() const { return parse_enum16(29); }

    inline uint16_t AUXOutputControl() const { return parse_enum16(30); }

    inline uint16_t AUXOutputState() const { return parse_enum16(31); }

    inline uint16_t AUXOutputPolarity() const { return parse_enum16(32); }

    /**
     * [V]
     */
    inline uint16_t AUXLowBatteryDisconnect() const { return parse_uint16(33); }

    /**
     * [V]
     */
    inline uint16_t AUXLowBatteryReconnect() const { return parse_uint16(34); }

    /**
     * [Tms]
     */
    inline uint16_t AUXLowBatteryDisconnectDelay() const { return parse_uint16(35); }

    /**
     * [V]
     */
    inline uint16_t AUXVentFan() const { return parse_uint16(36); }

    /**
     * [V]
     */
    inline uint16_t AUXPVTrigger() const { return parse_uint16(37); }

    /**
     * [Tms]
     */
    inline uint16_t AUXPVTriggerHoldTime() const { return parse_uint16(38); }

    /**
     * [V]
     */
    inline uint16_t AUXNightLightThreshold() const { return parse_uint16(39); }

    /**
     * [Tmh]
     */
    inline uint16_t AUXNightLightOnTime() const { return parse_uint16(40); }

    /**
     * [Tms]
     */
    inline uint16_t AUXNightLightOnHysteresis() const { return parse_uint16(41); }

    /**
     * [Tms]
     */
    inline uint16_t AUXNightLightOffHysteresis() const { return parse_uint16(42); }

    /**
     * [V]
     */
    inline uint16_t AUXErrorOutputLowBattery() const { return parse_uint16(43); }

    /**
     * [Tms]
     */
    inline uint16_t AUXDivertHoldTime() const { return parse_uint16(44); }

    /**
     * [Tms]
     */
    inline uint16_t AUXDivertDelayTime() const { return parse_uint16(45); }

    /**
     * [V]
     */
    inline uint16_t AUXDivertRelative() const { return parse_uint16(46); }

    /**
     * [V]
     */
    inline uint16_t AUXDivertHysteresis() const { return parse_uint16(47); }

    inline uint16_t FMCCMajorFirmwareNumber() const { return parse_uint16(48); }

    inline uint16_t FMCCMidFirmwareNumber() const { return parse_uint16(49); }

    inline uint16_t FMCCMinorFirmwareNumber() const { return parse_uint16(50); }

    /**
     * [Tmd]
     */
    inline uint16_t setDataLogDayOffset() const { return parse_uint16(51); }

    /**
     * [Tmd]
     */
    inline uint16_t currentDataLogDayOffset() const { return parse_uint16(52); }

    /**
     * [Ah]
     */
    inline uint16_t dataLogDailyAh() const { return parse_uint16(53); }

    /**
     * [kWh]
     */
    inline uint16_t dataLogDailyKWh() const { return parse_uint16(54); }

    /**
     * [A]
     */
    inline uint16_t dataLogDailyMaximumOutputA() const { return parse_uint16(55); }

    /**
     * [W]
     */
    inline uint16_t dataLogDailyMaximumOutputW() const { return parse_uint16(56); }

    /**
     * [Tms]
     */
    inline uint16_t dataLogDailyAbsorbTime() const { return parse_uint16(57); }

    /**
     * [Tms]
     */
    inline uint16_t dataLogDailyFloatTime() const { return parse_uint16(58); }

    /**
     * [V]
     */
    inline uint16_t dataLogDailyMinimumBattery() const { return parse_uint16(59); }

    /**
     * [V]
     */
    inline uint16_t dataLogDailyMaximumBattery() const { return parse_uint16(60); }

    /**
     * [V]
     */
    inline uint16_t dataLogDailyMaximumInput() const { return parse_uint16(61); }

    inline uint16_t dataLogClear() const { return parse_uint16(62); }

    inline uint16_t dataLogClearComplement() const { return parse_uint16(63); }

};

}
