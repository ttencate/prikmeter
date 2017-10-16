const bcryptPromise = require('bcrypt-promise')

const db = require('../core/db')

module.exports = {
  get: async function ({ id, email }) {
    const query = {}
    if (id) query.id = id
    if (email) query.email = email
    const user = await db.from('users').where(query).first('id', 'email', 'passwordHash')
    return user
  },

  create: async function ({ email, password, bcryptRounds = 12 }) {
    const passwordHash = await bcryptPromise.hash(password, bcryptRounds)
    const user = { email, passwordHash };
    [ user.id ] = await db('users').insert(user).returning('id')
    return user
  },

  isPasswordCorrect: async function (user, password) {
    if (!user) {
      return false
    }
    const passwordCorrect = await bcryptPromise.compare(password, user.passwordHash)
    return passwordCorrect
  }
}
