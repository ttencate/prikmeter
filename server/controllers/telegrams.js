const bodyParser = require('body-parser')
const check = require('express-validator/check')

const authTokens = require('../services/auth-tokens')
const meters = require('../services/meters')
const telegrams = require('../services/telegrams')

const AUTH_TOKEN_HEADER = 'x-auth-token'

module.exports.create = [
  bodyParser.raw({ type: 'text/plain' }), // Populates req.body as a Buffer.
  async function (req, res) {
    const errors = check.validationResult(req)
    if (!errors.isEmpty()) {
      console.warn(errors.array())
      res.sendStatus(400)
      return
    }

    const token = req.headers[AUTH_TOKEN_HEADER]
    const telegram = req.body.toString('ascii') // TODO see what happens if we shove it to the database as a Buffer directly

    const user = await authTokens.getOwnerUser({ token })
    if (!user) {
      console.warn(`Auth token ${token} is invalid`)
      res.sendStatus(403)
      return
    }

    await telegrams.create({ ownerUserId: user.id, telegram })
    console.log(`Stored ${telegram.length} byte telegram for user ${user.id}`)

    res.sendStatus(200)
  }
]
