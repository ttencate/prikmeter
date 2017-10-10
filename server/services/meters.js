const crypto = require('crypto')
const db = require('../storage/db')

module.exports = {
  get: async function ({ id }) {
    const meter = await db.from('meters').where({ id }).first('id', 'name')
    return meter
  },

  getForAuthToken: async function (authToken) {
    authToken = authToken || ''
    const meter = await db.from('authTokens')
        .where({ 'authTokens.token': authToken })
        .leftJoin('meters', 'authTokens.meterId', 'meters.id')
        .first('meters.id', 'meters.name')
    return meter
  },

  getForUser: async function (user) {
    const queryResult = await db.from('meters').where({ ownerUserId: user.id })
        .leftJoin('authTokens', 'meters.id', 'authTokens.meterId')
        .select('meters.id', 'meters.name', 'authTokens.token')
    const metersById = {}
    for (let row of queryResult) {
      metersById[row.id] = metersById[row.id] || { id: row.id, name: row.name, authTokens: [] }
      metersById[row.id].authTokens.push(row.token)
    }
    return Object.values(metersById)
  },

  create: async function ({ name, ownerUserId }) {
    const meterId = await db('meters').insert({ name, ownerUserId }).returning('id')
    return meterId
  },

  createAuthToken: async function (meter) {
    const token = crypto.randomBytes(32).toString('hex')
    await db('authTokens').insert({ token: token, meterId: meter.id })
    return token
  }
}
