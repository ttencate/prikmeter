const moment = require('moment')

const authTokens = require('../services/authTokens')
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

module.exports.root = [
  async function (req, res) {
    const user = req.session.user
    if (!user) {
      // TODO use a different template for this case
      // TODO put return_url into all template contexts through middleware
      res.render('index', {
        return_url: req.url,
      })
      return
    }

    const [ userMeters, userAuthTokens ] = await Promise.all([
      meters.getForUser(user),
      authTokens.getForUser(user)
    ])
    const nowMillis = Date.now()
    const now = new Date(nowMillis);
    const oneWeekAgo = new Date(nowMillis - 7 * 24 * 60 * 60 * 1000);
    await Promise.all(userMeters.map(async (meter) => {
      meter.readings = compressReadings(await readings.getForMeter(meter, { startTime: oneWeekAgo, endTime: now, resolution: 'hour' }))
    }))
    res.render('index', {
      return_url: req.url,
      user: user,
      meters: userMeters,
      authTokens: userAuthTokens
    })
  }
]
