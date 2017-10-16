const winston = require('winston')

function createLogger () {
  const env = process.env.NODE_ENV
  switch (env) {

    case 'development':
      return winston.createLogger({
        level: 'debug',
        format: winston.format.simple(),
        transports: [
          new winston.transports.Console({
            colorize: true,
            humanReadableUnhandledException: true,
          })
        ]
      })

    case 'testing':
      return winston.createLogger({
        level: 'error',
        format: winston.format.simple(),
        transports: [
          new winston.transports.Console({
            colorize: true,
            humanReadableUnhandledException: true,
          })
        ]
      })

    case 'production':
    default:
      return winston.createLogger({
        level: 'warn',
        format: winston.format.simple(),
        transports: [
          new winston.transports.Console({
            colorize: false,
            humanReadableUnhandledException: true,
            depth: 1,
          })
        ]
      })

  }
}

module.exports = createLogger()
