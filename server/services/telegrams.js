const db = require('../storage/db')

module.exports = {
  create: async function ({ ownerUserId, telegram }) {
    await db('telegrams').insert({ ownerUserId, telegram })
  }
}
