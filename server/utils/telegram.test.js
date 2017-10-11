const { expect } = require('chai')

const telegram = require('./telegram')

const DATA = '/XMX5LGBBFFB231117791\r\n\r\n1-3:0.2.8(42)\r\n0-0:1.0.0(171011174059S)\r\n0-0:96.1.1(4530303035303031353633323635353134)\r\n1-0:1.8.1(001677.034*kWh)\r\n1-0:2.8.1(000000.000*kWh)\r\n1-0:1.8.2(002060.771*kWh)\r\n1-0:2.8.2(000000.000*kWh)\r\n0-0:96.14.0(0002)\r\n1-0:1.7.0(00.330*kW)\r\n1-0:2.7.0(00.000*kW)\r\n0-0:96.7.21(00005)\r\n0-0:96.7.9(00002)\r\n1-0:99.97.0(2)(0-0:96.7.19)(151007113802S)(0000003567*s)(150817150911S)(0000003252*s)\r\n1-0:32.32.0(00000)\r\n1-0:52.32.0(00001)\r\n1-0:72.32.0(00001)\r\n1-0:32.36.0(00000)\r\n1-0:52.36.0(00000)\r\n1-0:72.36.0(00000)\r\n0-0:96.13.1()\r\n0-0:96.13.0()\r\n1-0:31.7.0(000*A)\r\n1-0:51.7.0(002*A)\r\n1-0:71.7.0(000*A)\r\n1-0:21.7.0(00.003*kW)\r\n1-0:41.7.0(00.319*kW)\r\n1-0:61.7.0(00.008*kW)\r\n1-0:22.7.0(00.000*kW)\r\n1-0:42.7.0(00.000*kW)\r\n1-0:62.7.0(00.000*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(4730303032333430313334343435393134)\r\n0-1:24.2.1(171011170000S)(03964.814*m3)\r\n!D0FD\r\n'

const CORRUPT_DATA = '/XMX5LGBBFFB231117791\r\n\r\n1-3:1.2.8(42)\r\n0-0:1.0.0(171011174059S)\r\n0-0:96.1.1(4530303035303031353633323635353134)\r\n1-0:1.8.1(001677.034*kWh)\r\n1-0:2.8.1(000000.000*kWh)\r\n1-0:1.8.2(002060.771*kWh)\r\n1-0:2.8.2(000000.000*kWh)\r\n0-0:96.14.0(0002)\r\n1-0:1.7.0(00.330*kW)\r\n1-0:2.7.0(00.000*kW)\r\n0-0:96.7.21(00005)\r\n0-0:96.7.9(00002)\r\n1-0:99.97.0(2)(0-0:96.7.19)(151007113802S)(0000003567*s)(150817150911S)(0000003252*s)\r\n1-0:32.32.0(00000)\r\n1-0:52.32.0(00001)\r\n1-0:72.32.0(00001)\r\n1-0:32.36.0(00000)\r\n1-0:52.36.0(00000)\r\n1-0:72.36.0(00000)\r\n0-0:96.13.1()\r\n0-0:96.13.0()\r\n1-0:31.7.0(000*A)\r\n1-0:51.7.0(002*A)\r\n1-0:71.7.0(000*A)\r\n1-0:21.7.0(00.003*kW)\r\n1-0:41.7.0(00.319*kW)\r\n1-0:61.7.0(00.008*kW)\r\n1-0:22.7.0(00.000*kW)\r\n1-0:42.7.0(00.000*kW)\r\n1-0:62.7.0(00.000*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(4730303032333430313334343435393134)\r\n0-1:24.2.1(171011170000S)(03964.814*m3)\r\n!D0FD\r\n'

describe('telegram', () => {
  describe('isCrcValid', () => {
    it('accepts valid CRCs', () => {
      expect(telegram.isCrcValid(DATA)).to.equal(true)
    })

    it('rejects the empty string', () => {
      expect(telegram.isCrcValid('')).to.equal(false)
    })

    it('rejects invalid CRCs', () => {
      expect(telegram.isCrcValid(CORRUPT_DATA)).to.equal(false)
    })
  })

  describe('parse', () => {
    it('parses the example telegram', () => {
      const actual = telegram.parse(DATA)
    })
  })
})