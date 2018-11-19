const chai = require('chai')
const chaiAsPromised = require('chai-as-promised')
const chaiMoment = require('chai-moment')
const moment = require('moment')

// Copied and adapted from
// https://github.com/picardy/chai-moment/blob/080ba06fcb89392d6c9e9863f65326779e7d88c4/index.js
const moreChaiMoment = function (chai, utils) {
  const _format = 'LLLL'

  chai.Assertion.addMethod('sameOrBeforeMoment', function (expected, granularity) {
    const obj = this._obj
    const objMoment = moment(obj)
    const expectedMoment = moment(expected)
    const granularityStr = granularity ? ` (granularity: ${granularity})` : ''
    this.assert(
      objMoment.isSameOrBefore(expectedMoment, granularity),
      `expected ${objMoment.format(_format)} to be same as or before ${expectedMoment.format(_format)}${granularityStr}`,
      `expected ${objMoment.format(_format)} not to be same as or before ${expectedMoment.format(_format)}${granularityStr}`,
      expected,
      obj,
      true
    )
  })

  chai.Assertion.addMethod('sameOrAfterMoment', function (expected, granularity) {
    const obj = this._obj
    const objMoment = moment(obj)
    const expectedMoment = moment(expected)
    const granularityStr = granularity ? ` (granularity: ${granularity})` : ''
    this.assert(
      objMoment.isSameOrAfter(expectedMoment, granularity),
      `expected ${objMoment.format(_format)} to be same as or after ${expectedMoment.format(_format)}${granularityStr}`,
      `expected ${objMoment.format(_format)} not to be same as or after ${expectedMoment.format(_format)}${granularityStr}`,
      expected,
      obj,
      true
    )
  })
}

chai.use(chaiAsPromised)
chai.use(chaiMoment)
chai.use(moreChaiMoment)

module.exports = chai
