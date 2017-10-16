const express = require('express')
require('make-promises-safe')

async function main () {
  // Err on the side of caution: development can leak stack traces and such.
  if (process.env.NODE_ENV !== 'development') {
    process.env.NODE_ENV = 'production'
  }

  await require('./core/migrate')()

  const app = express()
  app.locals.devMode = app.get('env') === 'development'

  await require('./middleware').install(app)
  require('./controllers').addRoutes(app)

  const LISTEN_HOST = process.env.LISTEN_HOST || 'localhost'
  const LISTEN_PORT = parseInt(process.env.LISTEN_PORT) || 3000
  app.listen(LISTEN_PORT, LISTEN_HOST)
  console.log(`Listening on http://${LISTEN_HOST}:${LISTEN_PORT}/`)
}

main()
