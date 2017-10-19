const db = require('../core/db')

module.exports = {
  get: async function ({ id }) {
    const meter = await db.from('meters').where({ id }).first('id', 'type', 'ownerUserId')
    return meter
  },

  getForUser: async function (user) {
    const meters = await db.from('meters')
        .where({ ownerUserId: user.id })
        .select('id', 'type', 'ownerUserId')
    return meters
  },

  createOrUpdate: async function ({ id, type, ownerUserId }) {
    const meter = await db.from('meters').where({ id }).first('id', 'type', 'ownerUserId')
    if (meter) {
      if (meter.type !== type || meter.ownerUserId !== ownerUserId) {
        await db('meters').where({ id }).update({ type, ownerUserId })
      }
    } else {
      await db('meters').insert({ id, type, ownerUserId })
    }
  }
}
