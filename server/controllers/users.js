const bodyParser = require('body-parser')
const { check, validationResult } = require('express-validator/check')

const users = require('../services/users')

module.exports.create = [
  bodyParser.urlencoded({ extended: false }),
  check('email', 'Not a valid email address').isEmail().trim(),
  check('password1', 'Password must be at least 8 characters').isLength({ min: 8 }),
  check('password2', 'Passwords do not match').exists().custom((value, { req }) => value === req.body.password1),
  async function (req, res) {
    const errors = validationResult(req)
    if (!errors.isEmpty()) {
      res.render('index', {
        errors: errors.array().map(error => error.msg)
      })
      return
    }

    const userId = await users.create(req.body.email, req.body.password1)

    session.user = users.get(userId)

    res.redirect('/')
  }
]
