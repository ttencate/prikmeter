const db = require('./db')
const testdata = require('../seeds/testdata')

module.exports = {
  data: testdata.data,
  reset: async function () {
    await db.migrate.latest()
    await db.seed.run()
  }
}
