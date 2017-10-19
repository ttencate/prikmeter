const authTokens = require('../services/authTokens')
const meters = require('../services/meters')
const readings = require('../services/readings')

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
    const [ userReadings ] = await Promise.all(userMeters.map((meter) => {
      readings.getForMeter(meter, { start: new Date(nowMillis - 7 * 24 * 60 * 60 * 1000), end: new Date(nowMillis) })
    }))
    res.render('index', {
      return_url: req.url,
      user: user,
      meters: userMeters,
      authTokens: userAuthTokens,
      readings: userReadings
    })
  }
]
