const moment = require('moment')

const db = require('../core/db')
const log = require('../core/log')

module.exports = {
  create: async function ({ ownerUserId, telegram, uploadTimestamp }) {
    await db('telegrams').insert({ ownerUserId, telegram, uploadTimestamp })
  },

  getForUser: async function (user) {
    const telegrams = await db.from('telegrams')
        .where({ ownerUserId: user.id })
        .select('ownerUserId', 'telegram', 'uploadTimestamp')
        .orderBy('uploadTimestamp')
    return telegrams
  },

  cleanUp: async function (now = undefined) {
    if (!now) {
      now = moment()
    }
    const start = Date.now()
    const deletions = await db.from('telegrams')
        .where('uploadTimestamp', '<', now.clone().subtract(14, 'days').toDate())
        .del()
    const durationSeconds = (Date.now() - start) / 1000
    log.info(`Cleaned up ${deletions} old telegrams in ${durationSeconds} seconds`)
  }
}
