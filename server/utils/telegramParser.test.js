/* eslint-env mocha, chai */

const { expect } = require('chai')
const moment = require('moment')

const telegramParser = require('./telegramParser')
const { data } = require('../seeds/testdata')

describe('utils/telegramParser', () => {
  describe('isCrcValid', () => {
    it('accepts valid CRCs', () => {
      expect(telegramParser.isCrcValid(data.telegram.telegram)).to.equal(true)
    })

    it('rejects the empty string', () => {
      expect(telegramParser.isCrcValid('')).to.equal(false)
    })

    it('rejects invalid CRCs', () => {
      expect(telegramParser.isCrcValid(data.corruptTelegram.telegram)).to.equal(false)
    })
  })

  describe('parse', () => {
    it('parses the telegram', () => {
      actual = telegramParser.parse(data.telegram.telegram)
      expect(actual).to.deep.equal([
        {
          type: 'electricity',
          meterId: 'E0005001563265514',
          timestamp: moment('2017-10-11T17:40:59+02:00').unix(),
          totalConsumptionKwhLow: 1677.034,
          totalConsumptionKwhHigh: 2060.771,
          totalProductionKwhLow: 0.0,
          totalProductionKwhHigh: 0.0,
          currentConsumptionKw: 0.330,
          currentProductionKw: 0.0
        },
        {
          type: 'gas',
          meterId: 'G0002340134445914',
          timestamp: moment('2017-10-11T17:00:00+02:00').unix(),
          totalConsumptionM3: 3964.814
        }
      ])
    })
  })
})
