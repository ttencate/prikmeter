const db = require('./db')
const testdata = require('../seeds/testdata')

/**
 * SQLite returns timestamps (DATETIME type) as milliseconds since epoch,
 * PostgreSQL returns them as Date objects. This normalizes everything to
 * milliseconds.
 */
function datesToTimestamps (input) {
  if (input instanceof Date) {
    return input.getTime()
  } else if (typeof input === 'object') {
    const clone = {}
    for (const key in input) {
      clone[key] = datesToTimestamps(input[key])
    }
    return clone
  } else {
    return input
  }
}

module.exports = {
  data: testdata.data,
  datesToTimestamps: datesToTimestamps,
  reset: async function () {
    await db.migrate.latest()
    await db.seed.run()
  }
}
