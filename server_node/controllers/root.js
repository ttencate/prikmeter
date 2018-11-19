const authTokens = require('../services/authTokens')
const meters = require('../services/meters')

module.exports.root = [
  async function (req, res) {
    // TODO put logged-in check into a helper middleware
    const user = req.session.user
    if (!user) {
      // TODO use a different template for this case
      // TODO put return_url into all template contexts through middleware
      res.render('index', {
        return_url: req.url
      })
      return
    }

    const [ userMeters, userAuthTokens ] = await Promise.all([
      meters.getForUser(user),
      authTokens.getForUser(user)
    ])
    res.render('index', {
      return_url: req.url,
      user: user,
      meters: userMeters,
      authTokens: userAuthTokens
    })
  }
]
