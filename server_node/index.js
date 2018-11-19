const express = require('express')
require('make-promises-safe')

const log = require('./core/log')

async function main () {
  // Err on the side of caution: development can leak stack traces and such.
  if (process.env.NODE_ENV !== 'development' && process.env.NODE_ENV !== 'testing') {
    process.env.NODE_ENV = 'production'
  }

  await require('./core/migrate')()

  const app = express()
  if (app.get('env') === 'production') {
    app.set('trust proxy', 1) // Needed for https-only cookies
  }

  await require('./middleware').install(app)
  await require('./controllers').addRoutes(app)

  const LISTEN_HOST = process.env.LISTEN_HOST || 'localhost'
  const LISTEN_PORT = parseInt(process.env.LISTEN_PORT) || 3000
  app.listen(LISTEN_PORT, LISTEN_HOST)
  log.info(`Listening on http://${LISTEN_HOST}:${LISTEN_PORT}/`)
}

main()
