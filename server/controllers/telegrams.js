const bodyParser = require('body-parser')
const check = require('express-validator/check')

const authTokens = require('../services/auth-tokens')
const log = require('../core/log')
const telegrams = require('../services/telegrams')

const AUTH_TOKEN_HEADER = 'x-auth-token'

async function createFromBody (req, res) {
  const errors = check.validationResult(req)
  if (!errors.isEmpty()) {
    log.warn(errors.array())
    res.sendStatus(400)
    return
  }

  const token = req.headers[AUTH_TOKEN_HEADER] || ''
  const telegram = req.body.toString('ascii') // TODO see what happens if we shove it to the database as a Buffer directly

  const user = await authTokens.getOwnerUser({ token })
  if (!user) {
    log.warn(`Auth token ${token} is invalid`)
    res.sendStatus(403)
    return
  }

  await telegrams.create({ ownerUserId: user.id, telegram })
  log.info(`Stored ${telegram.length} byte telegram for user ${user.id}`)

  res.sendStatus(200)
}

module.exports = {
  create: [
    bodyParser.raw({ type: 'text/plain' }), // Populates req.body as a Buffer.
    createFromBody
  ],
  createFromBody
}
