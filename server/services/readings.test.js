/* eslint-env mocha, chai */

const { expect } = require('../core/chai')

const readings = require('./readings')
const testDb = require('../core/testDb')

describe('services/readings', () => {
  beforeEach(testDb.reset)

  describe('create', () => {
    it('creates an electricity reading', async () => {
      const reading = Object.assign({}, testDb.data.electricityReading, { type: 'electricity' })
      await readings.create(reading)
      await expect(readings.getForMeter({ id: reading.meterId, type: 'electricity' })).to.eventually.deep.equal([reading])
    })
    
    it('does not create an electricity reading if a matching one already exists', async () => {
      const reading = Object.assign({}, testDb.data.electricityReading, { type: 'electricity' })
      await readings.create(reading)
      await readings.create(reading)
      await expect(readings.getForMeter({ id: reading.meterId, type: 'electricity' })).to.eventually.deep.equal([reading])
    })

    it('creates a gas reading', async () => {
      const reading = Object.assign({}, testDb.data.gasReading, { type: 'gas' })
      await readings.create(reading)
      await expect(readings.getForMeter({ id: reading.meterId, type: 'gas' })).to.eventually.deep.equal([reading])
    })
    
    it('does not create a gas reading if a matching one already exists', async () => {
      const reading = Object.assign({}, testDb.data.gasReading, { type: 'gas' })
      await readings.create(reading)
      await readings.create(reading)
      await expect(readings.getForMeter({ id: reading.meterId, type: 'gas' })).to.eventually.deep.equal([reading])
    })
  })
})
