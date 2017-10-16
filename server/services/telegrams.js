const db = require('../core/db')

module.exports = {
  create: async function ({ ownerUserId, telegram }) {
    await db('telegrams').insert({ ownerUserId, telegram })
  }
}
