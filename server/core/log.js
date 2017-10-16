const winston = require('winston')

const LEVELS = {
  development: 'debug',
  testing: 'error',
  production: 'warn'
}

function createLogger () {
  const env = process.env.NODE_ENV || 'production'

  let format = winston.format.combine(
    winston.format.timestamp(),
    winston.format.align(),
    winston.format.printf(info => `${info.timestamp} ${info.level} ${info.message}`)
  )

  if (env !== 'production') {
    format = winston.format.combine(
      winston.format.colorize(),
      format
    )
  }

  return winston.createLogger({
    level: LEVELS[env] || LEVELS['production'],
    format: format,
    transports: [
      new winston.transports.Console({
        depth: 1,
        humanReadableUnhandledException: true
      })
    ]
  })
}

module.exports = createLogger()
