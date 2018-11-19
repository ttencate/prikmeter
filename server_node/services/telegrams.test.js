/* eslint-env mocha, chai */

const { expect } = require('../core/chai')

const telegrams = require('./telegrams')
const testDb = require('../core/testDb')

describe('services/telegrams', () => {
  beforeEach(testDb.reset)

  describe('create', () => {
    it('creates the telegram', async () => {
      await telegrams.create({ ownerUserId: testDb.data.user.id, telegram: 'testTelegram' })
    })
  })

  describe('getForUser', () => {
    it('returns all the user\'s telegrams', async () => {
      const userTelegrams = await telegrams.getForUser({ id: testDb.data.user.id })

      expect(userTelegrams).to.have.length(1)

      const telegram = userTelegrams[0]
      expect(telegram.uploadTimestamp).to.exist
      delete telegram.uploadTimestamp
      expect(telegram).to.deep.equal(testDb.data.telegram)
    })
  })
})
