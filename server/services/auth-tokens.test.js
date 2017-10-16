const chai = { expect } = require('chai')

const authTokens = require('./auth-tokens')
const users = require('./users')
const testDb = require('../storage/testDb')

describe('settings', () => {
  beforeEach(testDb.reset)

  describe('getForUser', () => {
    it('returns the user\'s tokens', async () => {
      await expect(authTokens.getForUser(testDb.data.user)).to.eventually.deep.equal([ testDb.data.authToken ])
    })
  })

  describe('getOwnerUser', () => {
    it('returns the user owning the token', async () => {
      const ownerUser = await authTokens.getOwnerUser({ token: testDb.data.authToken.token })
      expect(ownerUser.id).to.equal(testDb.data.user.id)
    })

    it('returns undefined if the token does not exist', async () => {
      await expect(authTokens.getOwnerUser({ token: testDb.data.nonexistentAuthToken.token })).to.eventually.equal(undefined)
    })
  })
})

