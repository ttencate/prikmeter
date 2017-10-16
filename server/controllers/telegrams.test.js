/* eslint-env mocha, chai */

const { expect } = require('chai')

const { simulateRequest } = require('./testing')
const telegrams = require('./telegrams')
const telegramsService = require('../services/telegrams')
const testDb = require('../core/testDb')

describe('controllers/telegram', () => {
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

    describe('when passed valid credentials and a valid telegram', async () => {
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
        await expect(await telegramsService.getForUser({ id: testDb.data.user.id })).to.have.length(2)
      })

      it('creates the meters', async () => {
      })
    })
  })
})
