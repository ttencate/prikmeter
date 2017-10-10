const db = require('../storage/db')

module.exports = {
  create: async function ({ meterId, telegram }) {
    await db('telegrams').insert({ meterId, telegram })
  }
}
