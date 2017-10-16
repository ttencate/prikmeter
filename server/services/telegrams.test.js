const chai = { expect } = require('chai')

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
      expect(telegrams.getForUser({ id: testDb.data.user.id })).to.eventually.deep.equal([testDb.data.telegram])
    })
  })
})
