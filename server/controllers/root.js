const authTokens = require('../services/authTokens')
const meters = require('../services/meters')

module.exports.root = [
  async function (req, res) {
    const user = req.session.user
    const userMeters = user ? await meters.getForUser(user) : null
    const userAuthTokens = user ? await authTokens.getForUser(user) : null
    res.render('index', {
      return_url: req.url,
      user: user,
      meters: userMeters,
      authTokens: userAuthTokens
    })
  }
]
