const moment = require('moment')

const meters = require('../services/meters')
const readings = require('../services/readings')

function compressReadings (readings) {
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

    const nowMillis = Date.now()
    const now = new Date(nowMillis);
    const oneWeekAgo = new Date(nowMillis - 7 * 24 * 60 * 60 * 1000);
    const meterReadings = await readings.getForMeter(meter, { startTime: oneWeekAgo, endTime: now, resolution: 'hour' })

    res.json(compressReadings(meterReadings))
  }
}
