function createKnex () {
  const env = process.env.NODE_ENV || 'production'

  const knexfile = require('../knexfile')
  const knexConfig = knexfile[env]
  if (typeof knexConfig !== 'object') {
    throw new Error(`knexfile.js does not contain a configuration for the "${env}" environment`)
  }

  const knex = require('knex')(knexConfig)

  return knex
}

module.exports = createKnex()
