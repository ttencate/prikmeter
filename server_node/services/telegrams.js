const db = require('../core/db')

module.exports = {
  create: async function ({ ownerUserId, telegram }) {
    await db('telegrams').insert({ ownerUserId, telegram })
  },

  getForUser: async function (user) {
    const telegrams = await db.from('telegrams')
        .where({ ownerUserId: user.id })
        .select('ownerUserId', 'telegram', 'uploadTimestamp')
        .orderBy('uploadTimestamp')
    return telegrams
  }
}
