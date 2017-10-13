const crc = require('crc')
const moment = require('moment')

function throwParseError (message) {
  throw new Error(`${message}`)
}

function canonicalObisCode (a, b, c, d, e, f) {
  if (f) {
    f = '.' + f.substring(1)
  } else {
    f = '.255'
  }
  const joined = a + b + c + d + e + f
  if (!a || !b || !e) {
    throwParseError(`Current implementation cannot handle missing value groups A, B or E: ${joined}`)
  }
  return joined
}

function stripParentheses (value) {
  if (value.length < 2 || value[0] !== '(' || value[value.length - 1] !== ')') {
    throwParseError(`Value is not enclosed in parentheses: ${value}`)
  }
  return value.substring(1, value.length - 1)
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
  return dateTime.toDate()
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

function singleField (field, parser) {
  return function handle (output, canonicalObisCode, value) {
    output[field] = parser(value)
  }
}

function multipleFields (handlers) {
  return function handle (output, canonicalObisCode, value) {
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
      handlers[handlerIndex](output, canonicalObisCode, match[0])
      handlerIndex++
    }
    if (handlerIndex !== handlers.length) {
      throwParseError(`Found fewer than the expected ${handlers.length} fields in ${value}`)
    }
  }
}

const KNOWN_COSEM_OBJECTS = {
  '0-0:1.0.0.255': singleField('electricityDateTime', timestamp),
  '1-0:1.8.1.255': singleField('totalElectricityConsumptionKwhLow', floatWithUnit('kWh')),
  '1-0:1.8.2.255': singleField('totalElectricityConsumptionKwhHigh', floatWithUnit('kWh')),
  '1-0:2.8.1.255': singleField('totalElectricityProductionKwhLow', floatWithUnit('kWh')),
  '1-0:2.8.2.255': singleField('totalElectricityProductionKwhHigh', floatWithUnit('kWh')),
  '1-0:1.7.0.255': singleField('currentElectricityConsumptionKw', floatWithUnit('kW')),
  '1-0:2.7.0.255': singleField('currentElectricityProductionKw', floatWithUnit('kW')),
  // TODO actually this is 0-n, because the gas meter could be on a different channel
  '0-1:24.2.1.255': multipleFields([singleField('gasDateTime', timestamp), singleField('totalGasConsumptionM3', floatWithUnit('m3'))])
}

module.exports = {

  isCrcValid: function checkCrc (data) {
    // [^], unlike ., matches any character _including_ \r and \n.
    const match = /^([^]*!)([a-fA-F0-9]{4})\s*$/.exec(data)
    if (!match) {
      const snippet = (data.length > 12 ? '...' : '') + data.substring(data.length - 12).replace(/\r/g, '\\r').replace(/\n/g, '\\n')
      console.warn(`No CRC found at end of telegram: "${snippet}"`)
      return false
    }

    const expectedCrc = parseInt(match[2], 16)
    if (isNaN(expectedCrc)) {
      console.warn(`Could not parse expected CRC ${match[2]}`)
      return false
    }

    const actualCrc = crc.crc16(match[1])

    if (actualCrc !== expectedCrc) {
      console.warn(`CRC mismatch: expected ${expectedCrc.toString(16)}, got ${actualCrc.toString(16)}`)
      return false
    }

    return true
  },

  parse: function parse (data) {
    const lines = data.split('\r\n').map(line => line.trim()).filter((line) => line.length > 0)
    if (lines.length === 0) {
      throwParseError('No non-blank lines found in telegram')
    }

    const output = {}

    const dataItemRe = /^(\d+-|)(\d+:|)(\d+\.)(\d+)(\.\d+|)([^(]\d+|)(.*)\r\n/mg
    let match
    while ((match = dataItemRe.exec(data)) !== null) {
      const obisCode = canonicalObisCode(match[1], match[2], match[3], match[4], match[5], match[6])
      const value = match[7]

      const parse = KNOWN_COSEM_OBJECTS[obisCode]
      if (!parse) {
        continue
      }
      parse(output, obisCode, value)
    }

    return output
  }
}
