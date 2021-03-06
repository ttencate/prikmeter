const bodyParser = require('body-parser')
const check = require('express-validator/check')

const authTokens = require('../services/authTokens')
const log = require('../core/log')
const meters = require('../services/meters')
const readings = require('../services/readings')
const telegrams = require('../services/telegrams')
const telegramParser = require('../services/telegramParser')

const AUTH_TOKEN_HEADER = 'x-auth-token'

async function createFromBody (req, res) {
  const errors = check.validationResult(req)
  if (!errors.isEmpty()) {
    log.warn(errors.array())
    res.sendStatus(400)
    return
  }

  const token = req.headers[AUTH_TOKEN_HEADER] || ''
  const dataBuffer = req.body

  const user = await authTokens.getOwnerUser({ token })
  if (!user) {
    log.warn(`Auth token ${token} is invalid`)
    res.sendStatus(403)
    return
  }

  // TODO add columns "crcValid" and "parsed" to telegrams, clean up parsed
  // telegrams more aggressively

  await telegrams.create({ ownerUserId: user.id, telegram: dataBuffer })
  log.info(`Stored ${dataBuffer.length} byte telegram for user ${user.id}`)

  if (!telegramParser.isCrcValid(dataBuffer)) {
    res.status(400)
    res.send('CRC mismatch')
    return
  }

  let telegramReadings
  try {
    telegramReadings = telegramParser.parse(dataBuffer)
  } catch (ex) {
    log.warn(`Error parsing telegram: ${ex}`)
    throw ex
  }
  log.debug(`Parsed telegram: ${JSON.stringify(telegramReadings)}`)

  // TODO parallelize
  for (const reading of telegramReadings) {
    await meters.createOrUpdate({ id: reading.meterId, type: reading.type, ownerUserId: user.id })
    await readings.create(reading)
  }

  res.sendStatus(200)
}

module.exports = {
  create: [
    bodyParser.raw({ type: 'text/plain' }), // Populates req.body as a Buffer.
    createFromBody
  ],
  createFromBody
}
