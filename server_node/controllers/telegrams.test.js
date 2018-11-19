/* eslint-env mocha, chai */

const { expect } = require('chai')

const { simulateRequest } = require('./testing')
const meters = require('../services/meters')
const readings = require('../services/readings')
const telegrams = require('./telegrams')
const telegramsService = require('../services/telegrams')
const testDb = require('../core/testDb')

describe('controllers/telegrams', () => {
  beforeEach(testDb.reset)

  describe('createFromBody', () => {
    it('rejects requests without a token', async () => {
      const res = await simulateRequest(telegrams.createFromBody, {
        body: testDb.data.telegram.telegram
      })

      expect(res.statusCode).to.equal(403)
      await expect(await telegramsService.getForUser({ id: testDb.data.user.id })).to.have.length(1)
    })

    it('rejects requests with an invalid token', async () => {
      const res = await simulateRequest(telegrams.createFromBody, {
        headers: { 'X-Auth-Token': testDb.data.nonexistentAuthToken.token },
        body: testDb.data.telegram.telegram
      })

      expect(res.statusCode).to.equal(403)
      await expect(await telegramsService.getForUser({ id: testDb.data.user.id })).to.have.length(1)
    })

    it('rejects telegrams whose CRC does not match, but stores the telegram anyway', async () => {
      const res = await simulateRequest(telegrams.createFromBody, {
        headers: { 'X-Auth-Token': testDb.data.authToken.token },
        body: testDb.data.telegram.corruptTelegram
      })

      expect(res.statusCode).to.equal(400)
      await expect(await telegramsService.getForUser({ id: testDb.data.user.id })).to.have.length(2)
    })

    describe('when passed valid credentials and a valid DSMR 4.0 telegram', async () => {
      let res

      beforeEach(async () => {
        res = await simulateRequest(telegrams.createFromBody, {
          headers: { 'X-Auth-Token': testDb.data.authToken.token },
          body: testDb.data.telegram.telegram
        })
      })

      it('returns a success response', () => {
        expect(res.statusCode).to.equal(200)
      })

      it('creates the telegram', async () => {
        const telegrams = await telegramsService.getForUser({ id: testDb.data.user.id })
        expect(telegrams).to.have.length(2)
        expect(telegrams[1].telegram).to.deep.equal(testDb.data.telegram.telegram)
      })

      it('creates the meters', async () => {
        await expect(await meters.get({ id: testDb.data.electricityReading.meterId })).to.deep.equal({
          id: 'E0005001563265514',
          type: 'electricity',
          ownerUserId: testDb.data.user.id
        })
        await expect(await meters.get({ id: testDb.data.gasReading.meterId })).to.deep.equal({
          id: 'G0002340134445914',
          type: 'gas',
          ownerUserId: testDb.data.user.id
        })
      })

      it('creates the electricity reading', async () => {
        const reading = Object.assign({}, testDb.data.electricityReading, { type: 'electricity' })
        await expect(readings.getForMeter({ id: testDb.data.electricityReading.meterId, type: 'electricity' })).to.eventually.deep.equal([reading])
      })

      it('creates the gas reading', async () => {
        const reading = Object.assign({}, testDb.data.gasReading, { type: 'gas' })
        await expect(readings.getForMeter({ id: testDb.data.gasReading.meterId, type: 'gas' })).to.eventually.deep.equal([reading])
      })
    })

    describe('when passed valid credentials and a valid DSMR 5.0 telegram', async () => {
      let res

      beforeEach(async () => {
        res = await simulateRequest(telegrams.createFromBody, {
          headers: { 'X-Auth-Token': testDb.data.authToken.token },
          body: testDb.data.telegramDsmr50.telegram
        })
      })

      it('returns a success response', () => {
        expect(res.statusCode).to.equal(200)
      })

      it('creates the telegram', async () => {
        const telegrams = await telegramsService.getForUser({ id: testDb.data.user.id })
        expect(telegrams).to.have.length(2)
        expect(telegrams[1].telegram).to.deep.equal(testDb.data.telegramDsmr50.telegram)
      })
    })
  })
})
