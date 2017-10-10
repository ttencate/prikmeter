const bodyParser = require('body-parser')
const { check, validationResult } = require('express-validator/check')
const promisify = require('promisify-node')
const users = require('../services/users')

module.exports.login = [
  bodyParser.urlencoded({ extended: false }),
  check('email').exists(),
  check('password').exists(),
  async function (req, res) {
    const user = await users.get({ email: req.body.email })
    if (!await users.isPasswordCorrect(user, req.body.password)) {
      res.render('index', {
        errors: ['Password incorrect']
      })
      return
    }

    req.session.user = user

    res.redirect(req.body.return_url || '/')
  }
]

module.exports.logout = [
  async function (req, res) {
    await promisify(req.session.destroy())
    res.redirect('/')
  }
]
