const chai = { expect } = require('chai')

const telegrams = require('./telegrams')
const testDb = require('../core/testDb')

describe('settings', () => {
  beforeEach(testDb.reset)

  describe('create', () => {
    it('creates the telegram', async () => {
      await telegrams.create({ ownerUserId: testDb.data.user.id, telegram: 'testTelegram' })
    })
  })
})
