const bcryptPromise = require('bcrypt-promise')

const db = require('../storage/db')

module.exports = {
  get: async function ({ email }) {
    const user = await db.from('users').where({ email }).first('id', 'email', 'passwordHash')
    return user
  },

  create: async function ({ email, password }) {
    const passwordHash = await bcryptPromise.hash(password, 12)
    const userId = await db('users').insert({ email, passwordHash }).returning('id')
    return userId
  },

  isPasswordCorrect: async function (user, password) {
    if (!user) {
      return false
    }
    const passwordCorrect = await bcryptPromise.compare(password, user.passwordHash)
    return passwordCorrect
  }
}