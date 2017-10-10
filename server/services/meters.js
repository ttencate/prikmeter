const crypto = require('crypto')
const db = require('../storage/db')

module.exports = {
  get: async function ({ id }) {
    return await db.from('meters').where({ id }).first('id', 'name')
  },

  getForUser: async function (user) {
    const queryResult = await db.from('meters').where({ ownerUserId: user.id })
        .leftJoin('authTokens', 'meters.id', 'authTokens.meterId')
        .select('meters.id', 'meters.name', 'authTokens.token')
    const metersById = {}
    for (row of queryResult) {
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
