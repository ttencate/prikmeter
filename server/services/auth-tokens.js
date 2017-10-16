const crypto = require('crypto')
const db = require('../storage/db')

module.exports = {
  get: async function ({ token }) {
    const authToken = await db.from('authTokens').where({ token }).first('token', 'ownerUserId')
    return authToken
  },

  getForUser: async function (user) {
    const authTokens = await db.from('authTokens')
        .where({ ownerUserId: user.id })
        .select('token')
    return authTokens
  },

  getOwnerUser: async function ({ token }) {
    const user = await db.from('authTokens')
        .join('users', 'authTokens.ownerUserId', 'users.id')
        .first('users.id', 'users.email')
    return user
  },

  create: async function (ownerUser) {
    const token = crypto.randomBytes(32).toString('hex')
    const ownerUserId = ownerUser.id
    await db('authTokens').insert({ token, ownerUserId })
    return token
  }
}

