const crc = require('crc')

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
    function parseError() {
      throw new Error(`${message}:\n${data}`)
    }

    const lines = data.split('\r\n').map(line => line.trim()).filter((line) => line.length > 0)
    if (lines.length == 0) {
      parseError('No non-blank lines found in telegram')
    }

  },
}
