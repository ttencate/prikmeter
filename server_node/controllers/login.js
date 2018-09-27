const bodyParser = require('body-parser')
const { check, validationResult } = require('express-validator/check')
const promisify = require('promisify-node')

const users = require('../services/users')

module.exports.login = [
  bodyParser.urlencoded({ extended: false }),
  check('email').exists(),
  check('password').exists(),
  async function (req, res) {
    // TODO extract this to a common function somewhere
    const errors = validationResult(req)
    if (!errors.isEmpty()) {
      res.render('index', {
        errors: errors.array().map(error => error.msg)
      })
      return
    }

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
