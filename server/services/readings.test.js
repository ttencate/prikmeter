/* eslint-env mocha, chai */

const { expect } = require('../core/chai')

const readings = require('./readings')
const testDb = require('../core/testDb')

describe('services/readings', () => {
  beforeEach(testDb.reset)

  describe('createElectricity', () => {
    it('creates a reading', async () => {
      const reading = testDb.data.electricityReading
      await readings.createElectricity(reading)
      await expect(readings.getElectricityForMeter({ id: reading.meterId })).to.eventually.deep.equal([testDb.datesToTimestamps(reading)])
    })
    
    it('does not create a reading if a matching one already exists', async () => {
      const reading = testDb.data.electricityReading
      await readings.createElectricity(reading)
      await readings.createElectricity(reading)
      await expect(readings.getElectricityForMeter({ id: reading.meterId })).to.eventually.deep.equal([testDb.datesToTimestamps(reading)])
    })
  })

  describe('createGas', () => {
    it('creates a reading', async () => {
      const reading = testDb.data.gasReading
      await readings.createGas(reading)
      await expect(readings.getGasForMeter({ id: reading.meterId })).to.eventually.deep.equal([testDb.datesToTimestamps(reading)])
    })
    
    it('does not create a reading if a matching one already exists', async () => {
      const reading = testDb.data.gasReading
      await readings.createGas(reading)
      await readings.createGas(reading)
      await expect(readings.getGasForMeter({ id: reading.meterId })).to.eventually.deep.equal([testDb.datesToTimestamps(reading)])
    })
  })
})
