/* eslint-env mocha, chai */

const { expect } = require('../core/chai')
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
      expect(telegramParser.parse(data.telegram.telegram)).to.deep.equal([
        Object.assign({}, data.electricityReading, { type: 'electricity' }),
        Object.assign({}, data.gasReading, { type: 'gas' })
      ])
    })
  })
})
