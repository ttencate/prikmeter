/* eslint-env mocha, chai */

const { expect } = require('chai')

const meters = require('./meters')
const testDb = require('../core/testDb')

describe('services/meters', () => {
  beforeEach(testDb.reset)

  describe('get', () => {
    it('returns undefined if the meter does not exist', async () => {
      await expect(meters.get({ id: testDb.data.nonexistentMeter.id })).to.eventually.deep.equal(undefined)
    })

    it('returns the meter', async () => {
      await expect(meters.get({ id: testDb.data.meter.id })).to.eventually.deep.equal(testDb.data.meter)
    })
  })

  describe('getForUser', () => {
    it('returns all meters belonging to the user', async () => {
      await expect(meters.getForUser({ id: testDb.data.user.id })).to.eventually.deep.equal([testDb.data.meter])
    })
  })

  describe('createOrUpdate', () => {
    it('does not modify existing meters if data is the same', async () => {
      await meters.createOrUpdate(testDb.data.meter)
      await expect(meters.get({ id: testDb.data.meter.id })).to.eventually.deep.equal(testDb.data.meter)
    })

    it('updates the meter if the data is different', async () => {
      await meters.createOrUpdate({ id: testDb.data.meter.id, type: 'gas', ownerUserId: testDb.data.nonexistentUser.id })
      await expect(meters.get({ id: testDb.data.meter.id })).to.eventually.deep.equal(
        { id: testDb.data.meter.id, type: 'gas', ownerUserId: testDb.data.nonexistentUser.id })
    })

    it('creates the meter if it does not exist', async () => {
      await meters.createOrUpdate(testDb.data.nonexistentMeter)
      await expect(meters.get({ id: testDb.data.nonexistentMeter.id })).to.eventually.deep.equal(testDb.data.nonexistentMeter)
    })
  })
})
