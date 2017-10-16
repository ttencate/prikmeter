const chai = { expect } = require('chai')
const chaiAsPromised = require('chai-as-promised')

const settings = require('./settings')
const testDb = require('../core/testDb')

chai.use(chaiAsPromised)

describe('services/settings', () => {
  beforeEach(testDb.reset)

  describe('getValue', () => {
    it('returns undefined for nonexistent keys', async () => {
      expect(settings.getValue('nonexistent')).to.eventually.equal(undefined)
    })
  })

  describe('setValue', () => {
    it('sets the value to be retrieved', async () => {
      await settings.setValue('new', 'value')
      expect(await settings.getValue('new')).to.equal('value')
    })

    it('overwrites the previous value', async () => {
      await settings.setValue('overwrite', 'first')
      await settings.setValue('overwrite', 'second')
      expect(await settings.getValue('overwrite')).to.equal('second')
    })
  })
})
