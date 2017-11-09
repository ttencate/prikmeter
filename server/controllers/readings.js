const moment = require('moment')

const meters = require('../services/meters')
const readings = require('../services/readings')

function compressReadings (readings) {
  if (!readings.length) {
    return [[]];
  }
  function includeKey (key) {
    return ['meterId', 'type'].indexOf(key) === -1
  }
  const keys = Object.keys(readings[0]).filter(includeKey)

  const out = [ keys ]

  function getValue (reading, key) {
    const value = reading[key]
    switch (key) {
      case 'timestamp':
        return moment(reading[key]).valueOf() / 1000
      default:
        return value
    }
  }
  for (reading of readings) {
    out.push(keys.map(key => getValue(reading, key)))
  }

  return out
}

function parseDate(timestampStringMillis) {
  const timestampMillis = parseInt(timestampStringMillis)
  if (isNaN(timestampMillis)) {
    return null
  }
  return new Date(timestampMillis)
}

module.exports = {
  get: async function (req, res) {
    const user = req.session.user
    if (!user) {
      res.sendStatus(403)
      return
    }

    const meter = await meters.get({ id: req.params.meterId })
    if (!meter || meter.ownerUserId !== user.id) {
      res.sendStatus(403)
      return
    }

    const startTime = parseDate(req.query.startTime)
    const endTime = parseDate(req.query.endTime)
    const resolution = req.query.resolution
    if (!startTime || !endTime || ['second', 'minute', 'hour', 'day'].indexOf(resolution) === -1) {
      console.log(startTime, endTime, resolution, req.query)
      res.sendStatus(400)
      return
    }

    const meterReadings = await readings.getForMeter(meter, { startTime, endTime, resolution })

    res.json(compressReadings(meterReadings))
  }
}
