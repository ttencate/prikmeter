/* eslint-env mocha, chai */

const { expect } = require('chai')

const telegrams = require('./telegrams')
const testDb = require('../core/testDb')

describe('controllers/telegram', () => {
  beforeEach(testDb.reset)

  describe('create', () => {
    xit('creates the telegram', async () => {

    })
  })
})
