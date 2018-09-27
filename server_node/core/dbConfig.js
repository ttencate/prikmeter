const knexfile = require('../knexfile')

const env = process.env.NODE_ENV || 'production'
const knexConfig = knexfile[env]
if (typeof knexConfig !== 'object') {
  throw new Error(`knexfile.js does not contain a configuration for the "${env}" environment`)
}

module.exports = knexConfig
