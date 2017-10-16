const db = require('../core/db')

module.exports = {
  get: async function ({ id }) {
    const meter = await db.from('meters').where({ id }).first('id', 'name')
    return meter
  },

  getForUser: async function (user) {
    const meters = await db.from('meters')
        .where({ ownerUserId: user.id })
        .select('id', 'name')
    return meters
  },

  create: async function ({ name, ownerUserId }) {
    const meterId = await db('meters').insert({ name, ownerUserId }).returning('id')
    return meterId
  }
}
