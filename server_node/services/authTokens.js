const crypto = require('crypto')
const db = require('../core/db')

module.exports = {
  getForUser: async function (user) {
    const authTokens = await db.from('authTokens')
        .where({ ownerUserId: user.id })
        .select('token', 'ownerUserId')
    return authTokens
  },

  getOwnerUser: async function ({ token }) {
    const user = await db.from('authTokens')
        .join('users', 'authTokens.ownerUserId', 'users.id')
        .where({ token })
        .first('users.id', 'users.email')
    return user
  },

  create: async function (ownerUser) {
    const token = crypto.randomBytes(32).toString('hex')
    const ownerUserId = ownerUser.id
    const authToken = { token, ownerUserId }
    await db('authTokens').insert(authToken)
    return authToken
  }
}
