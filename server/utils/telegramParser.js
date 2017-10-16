const crc = require('crc')
const moment = require('moment')

const log = require('../core/log')

function throwParseError (message) {
  throw new Error(`${message}`)
}

function stripParentheses (value) {
  if (value.length < 2 || value[0] !== '(' || value[value.length - 1] !== ')') {
    throwParseError(`Value is not enclosed in parentheses: ${value}`)
  }
  return value.substring(1, value.length - 1)
}

function string (value) {
  value = stripParentheses(value)
  if (value.length % 2 !== 0) {
    throwParseError(`Hex string has odd length: ${value}`)
  }
  const chars = []
  for (let i = 0; i < value.length; i += 2) {
    const substr = value.substr(i, 2)
    const charCode = parseInt(substr, 16)
    if (isNaN(charCode)) {
      throwParseError(`Invalid hex sequence: ${substr}`)
    }
    chars.push(String.fromCharCode(charCode))
  }
  return chars.join('')
}

function timestamp (value) {
  value = stripParentheses(value)
  if (value.length < 1) {
    throwParseError(`Timestamp is too short: ${value}`)
  }

  const dateTimeStr = value.substring(0, value.length - 1)
  const dstFlag = value[value.length - 1]
  // Assuming Dutch timezone. The standard makes no mention of time zones.
  const timezoneOffset = {
    'S': '+02:00',
    'W': '+01:00'
  }[dstFlag]
  if (!timezoneOffset) {
    throwParseError(`Invalid DST flag in timestamp: ${value}`)
  }

  const dateTime = moment(dateTimeStr + timezoneOffset, 'YYMMDDHHmmssZZ')
  if (!dateTime.isValid()) {
    throwParseError(`Could not parse timestamp: ${value}`)
  }
  return dateTime.unix()
}

function floatWithUnit (expectedUnit) {
  return function parseFloatWithUnit (value) {
    value = stripParentheses(value)

    const parts = value.split('*')
    if (parts.length !== 2) {
      throwParseError(`Value must contain exactly one *: ${value}`)
    }

    const unit = parts[1]
    if (unit !== expectedUnit) {
      throwParseError(`Expected unit ${expectedUnit}, but got ${parts[1]}`)
    }

    const float = parseFloat(parts[0])
    if (isNaN(float)) {
      throwParseError(`Could not parse value as float: ${parts[0]}`)
    }

    return float
  }
}

function deviceType (value) {
  value = stripParentheses(value)
  const number = parseInt(value, 10)
  if (isNaN(number)) {
    throwParseError(`Could not parse device type number: ${value}`)
  }
  switch (number) {
    case 3: return 'gas'
    default: return undefined
  }
}

function fixedValue (value) {
  return function (_) {
    return value
  }
}

function singleField (field, parser) {
  return function handle (output, value) {
    output[field] = parser(value)
  }
}

function multipleFields (handlers) {
  return function handle (output, value) {
    const fieldRe = /\([^)]*\)/g
    let match
    let expectedIndex = 0
    let handlerIndex = 0
    while ((match = fieldRe.exec(value)) !== null) {
      if (handlerIndex > handlers.length) {
        throwParseError(`Found more than the expected ${handlers.length} fields in ${value}`)
      }
      if (match.index !== expectedIndex) {
        throwParseError(`Garbage between fields in multi-valued field ${value}`)
      }
      expectedIndex = match.index + match[0].length
      handlers[handlerIndex](output, match[0])
      handlerIndex++
    }
    if (handlerIndex !== handlers.length) {
      throwParseError(`Found fewer than the expected ${handlers.length} fields in ${value}`)
    }
  }
}

function ObisCode ({ a, b, c, d, e, f }) {
  this.a = parseInt(a)
  this.b = parseInt(b)
  this.c = parseInt(c)
  this.d = parseInt(d)
  this.e = parseInt(e)
  this.f = parseInt(f)
}

ObisCode.prototype.matchesPattern = function (pattern) {
  return (
    (pattern.a === undefined || this.a === pattern.a) &&
    (pattern.b === undefined || this.b === pattern.b) &&
    (pattern.c === undefined || this.c === pattern.c) &&
    (pattern.d === undefined || this.d === pattern.d) &&
    (pattern.e === undefined || this.e === pattern.e) &&
    (pattern.f === undefined || this.f === pattern.f)
  )
}

// Mapping from OBIS numbers to parsers. OBIS notation:
// A- B: C. D .E *F
// The parts "C." and "D" are required, the rest are optional. The character
// before F can vary.
//
// Part ("value group") B is the channel number. This distinguishes the
// electricity meter (channel 0) from any gas meters (channels 1-...).
//
// Part F is unused in DSMR and defaults to 255 in that case.
const KNOWN_COSEM_OBJECTS = [
  {obisCode: {a: 0, b: 0, c: 96, d: 1, e: 1}, parse: singleField('type', fixedValue('electricity'))},
  {obisCode: {a: 0, c: 96, d: 1}, parse: singleField('meterId', string)},
  {obisCode: {a: 0, c: 1, d: 0, e: 0}, parse: singleField('timestamp', timestamp)},
  {obisCode: {a: 1, c: 1, d: 8, e: 1}, parse: singleField('totalConsumptionKwhLow', floatWithUnit('kWh'))},
  {obisCode: {a: 1, c: 1, d: 8, e: 2}, parse: singleField('totalConsumptionKwhHigh', floatWithUnit('kWh'))},
  {obisCode: {a: 1, c: 2, d: 8, e: 1}, parse: singleField('totalProductionKwhLow', floatWithUnit('kWh'))},
  {obisCode: {a: 1, c: 2, d: 8, e: 2}, parse: singleField('totalProductionKwhHigh', floatWithUnit('kWh'))},
  {obisCode: {a: 1, c: 1, d: 7, e: 0}, parse: singleField('currentConsumptionKw', floatWithUnit('kW'))},
  {obisCode: {a: 1, c: 2, d: 7, e: 0}, parse: singleField('currentProductionKw', floatWithUnit('kW'))},
  {obisCode: {a: 0, c: 24, d: 1, e: 0}, parse: singleField('type', deviceType)},
  {obisCode: {a: 0, c: 24, d: 2, e: 1}, parse: multipleFields([singleField('timestamp', timestamp), singleField('totalConsumptionM3', floatWithUnit('m3'))])}
  // eslint-disable: object-curly-spacing
]

module.exports = {

  isCrcValid: function checkCrc (data) {
    // [^], unlike ., matches any character _including_ \r and \n.
    const match = /^([^]*!)([a-fA-F0-9]{4})\s*$/.exec(data)
    if (!match) {
      const snippet = (data.length > 12 ? '...' : '') + data.substring(data.length - 12).replace(/\r/g, '\\r').replace(/\n/g, '\\n')
      log.warn(`No CRC found at end of telegram: "${snippet}"`)
      return false
    }

    const expectedCrc = parseInt(match[2], 16)
    if (isNaN(expectedCrc)) {
      log.warn(`Could not parse expected CRC ${match[2]}`)
      return false
    }

    const actualCrc = crc.crc16(match[1])

    if (actualCrc !== expectedCrc) {
      log.warn(`CRC mismatch: expected ${expectedCrc.toString(16)}, got ${actualCrc.toString(16)}`)
      return false
    }

    return true
  },

  parse: function parse (data) {
    const lines = data.split('\r\n').map(line => line.trim()).filter((line) => line.length > 0)
    if (lines.length === 0) {
      throwParseError('No non-blank lines found in telegram')
    }

    const meters = []

    const dataItemRe = /^(?:(\d+)-|)(?:(\d+):|)(\d+)\.(\d+)(?:\.(\d+)|)(?:[^(](\d+)|)(.*)\r\n/mg
    let match
    while ((match = dataItemRe.exec(data)) !== null) {
      let [, a, b, c, d, e, f, value] = match
      if (!a || !b || !e) {
        throwParseError(`Current implementation cannot handle missing value groups A, B or E: ${match[0]}`)
      }
      const obisCode = new ObisCode({ a, b, c, d, e, f })
      const channel = obisCode.b
      const meter = meters[channel] = meters[channel] || {}

      for (const obj of KNOWN_COSEM_OBJECTS) {
        if (obisCode.matchesPattern(obj.obisCode)) {
          obj.parse(meter, value)
          // Do continue
        }
      }
    }

    return meters.filter(({ meterId, type }) => meterId && type)
  }
}
