const crc = require('crc')

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
  if (value.length < 2 || value[0] != '(' || value[value.length - 1] != ')') {
    throwParseError(`Value is not enclosed in parentheses: ${value}`)
  }
  return value.substring(1, value.length - 1)
}

function parseTimestamp (value) {
  // TODO
}

function parserFloatWithUnit (expectedUnit) {
  return function parseFloatWithUnit (value) {
    value = stripParentheses(value)

    const parts = value.split('*')
    if (parts.length != 2) {
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

const KNOWN_COSEM_OBJECTS = {
  '0-0:1.0.0.255': { field: 'timestamp', parser: parseTimestamp },
  '1-0:1.8.1.255': { field: 'totalConsumptionKwhLow', parser: parserFloatWithUnit('kWh') },
}

module.exports = {

  isCrcValid: function checkCrc(data) {
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

    if (actualCrc != expectedCrc) {
      console.warn(`CRC mismatch: expected ${expectedCrc.toString(16)}, got ${actualCrc.toString(16)}`)
      return false
    }

    return true
  },

  parse: function parse (data) {
    const lines = data.split('\r\n').map(line => line.trim()).filter((line) => line.length > 0)
    if (lines.length == 0) {
      throwParseError('No non-blank lines found in telegram')
    }

    const fields = {}

    const dataItemRe = /^(\d+-|)(\d+:|)(\d+\.)(\d+)(\.\d+|)([^(]\d+|)(.*)\r\n/mg
    let match
    while (match = dataItemRe.exec(data)) {
      const obisCode = canonicalObisCode(match[1], match[2], match[3], match[4], match[5], match[6])
      const desc = KNOWN_COSEM_OBJECTS[obisCode]
      if (!desc) {
        continue
      }
      const value = desc.parser(match[7])
      fields[desc.field] = value
    }

    return fields
  },
}
