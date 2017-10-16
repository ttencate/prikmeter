/* eslint-env mocha, chai */

const { expect } = require('../core/chai')

const settings = require('./settings')
const testDb = require('../core/testDb')

describe('services/readings', () => {
  beforeEach(testDb.reset)

  describe('createElectricity', () => {
    xit('creates a reading', () => {
    })
    
    xit('does not create a reading if a matching one already exists', () => {
    })
  })

  describe('createGas', () => {
    xit('creates a reading', () => {
    })
    
    xit('does not create a reading if a matching one already exists', () => {
    })
  })

  describe('getElectricityForMeter', () => {
    xit('returns all readings for the meter', () => {
    })
  })

  describe('getGasForMeter', () => {
    xit('returns all readings for the meter', () => {
    })
  })
})
