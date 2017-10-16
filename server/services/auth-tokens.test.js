const chai = { expect } = require('chai')

const authTokens = require('./auth-tokens')
const users = require('./users')
const testDb = require('../storage/testDb')

describe('settings', () => {
  let user
  let authToken

  beforeEach(testDb.reset)

  beforeEach(async () => {
    const userId = await users.create({ email: 'example@example.com', password: 'supersecret', bcryptRounds: 5 })
    user = await users.get({ id: userId })
    authToken = await authTokens.create(user)
  })

  describe('getForUser', () => {
    it('returns the user\'s tokens', async () => {
      await expect(authTokens.getForUser(user)).to.eventually.deep.equal([ authToken ])
    })
  })

  describe('getOwnerUser', () => {
    it('returns the user owning the token', async () => {
      const ownerUser = await authTokens.getOwnerUser({ token: authToken.token })
      expect(ownerUser.id).to.equal(user.id)
    })

    it('returns undefined if the token does not exist', async () => {
      await expect(authTokens.getOwnerUser({ token: authToken.token + 'x' })).to.eventually.equal(undefined)
    })
  })
})

