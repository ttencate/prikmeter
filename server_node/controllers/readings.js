const meters = require('../services/meters')
const readings = require('../services/readings')

function timestampStringToDate (timestampStringMillis) {
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

    const startTime = timestampStringToDate(req.query.startTime)
    const endTime = timestampStringToDate(req.query.endTime)
    const resolution = req.query.resolution
    if (!startTime || !endTime || ['second', 'minute', 'hour', 'day'].indexOf(resolution) === -1) {
      res.sendStatus(400)
      return
    }

    const meterReadings = await readings.getForMeterAsArray(meter, { startTime, endTime, resolution })

    // Drop the meterId column for compactness
    const meterIdColumnIndex = meterReadings[0].indexOf('meterId')
    if (meterIdColumnIndex >= 0) {
      for (const row of meterReadings) {
        row.splice(meterIdColumnIndex, 1)
      }
    }

    res.json(meterReadings)
  }
}
