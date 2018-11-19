/* eslint-env mocha, chai */

const { expect } = require('../core/chai')
const moment = require('moment')

const db = require('../core/db')
const telegrams = require('./telegrams')
const testDb = require('../core/testDb')

describe('services/telegrams', () => {
  beforeEach(testDb.reset)

  describe('create', () => {
    it('creates the telegram', async () => {
      const start = moment()
      await telegrams.create({ ownerUserId: testDb.data.user.id, telegram: Buffer.from('testTelegram') })
      const end = moment()
      const userTelegrams = await telegrams.getForUser({ id: testDb.data.user.id })
      expect(userTelegrams[1].uploadTimestamp).to.be.sameOrAfterMoment(start)
      expect(userTelegrams[1].uploadTimestamp).to.be.sameOrBeforeMoment(end)
    })

    it('creates the telegram with the given timestamp', async () => {
      const uploadTimestamp = moment('2016-05-12T08:00:00+05:00') // An arbitrary time before now.
      await telegrams.create({
        ownerUserId: testDb.data.user.id,
        telegram: Buffer.from('testTelegram'),
        uploadTimestamp: uploadTimestamp.toDate()
      })
      const userTelegrams = await telegrams.getForUser({ id: testDb.data.user.id })
      expect(userTelegrams[0].uploadTimestamp).to.be.sameMoment(uploadTimestamp)
    })
  })

  describe('getForUser', () => {
    it('returns all the user\'s telegrams', async () => {
      const userTelegrams = await telegrams.getForUser({ id: testDb.data.user.id })

      expect(userTelegrams).to.have.length(1)

      const telegram = userTelegrams[0]
      expect(telegram.uploadTimestamp).to.be.a('Date')
      delete telegram.uploadTimestamp
      expect(telegram).to.deep.equal(testDb.data.telegram)
    })
  })

  describe('cleanUp', () => {
    beforeEach(async () => {
      await db.from('telegrams').del()
    })

    it('removes all telegrams uploaded more than 2 weeks ago', async () => {
      const now = moment('2018-07-30T08:00:00+00:00')

      // This is a bit slow because we are not using a transaction. But that
      // would require some API redesign.
      for (let daysAgo = 0; daysAgo <= 30; daysAgo++) {
        await telegrams.create({
          ownerUserId: testDb.data.user.id,
          telegram: Buffer.from(`testTelegram${daysAgo}`),
          uploadTimestamp: now.clone().subtract(daysAgo, 'days').subtract(1, 'hours').toDate()
        })
      }

      await telegrams.cleanUp(now)

      const userTelegrams = await telegrams.getForUser({ id: testDb.data.user.id })
      const telegramDatas = userTelegrams.map((t) => t.telegram.toString('ascii'))

      expect(userTelegrams.length).to.equal(14)
      expect(telegramDatas).to.contain('testTelegram0')
      expect(telegramDatas).to.contain('testTelegram13')
      expect(telegramDatas).not.to.contain('testTelegram14')
      expect(telegramDatas).not.to.contain('testTelegram30')
    })
  })
})
