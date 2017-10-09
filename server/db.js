module.exports.createKnex = async function createKnex (env) {
  env = env || 'development'

  const knexfile = require('./knexfile')
  const knexConfig = knexfile[env]
  if (typeof knexConfig !== 'object') {
    throw new Error(`knexfile.js does not contain a configuration for the "${env}" environment`)
  }

  const knex = require('knex')(knexConfig)

  await knex.migrate.latest()

  return knex
}
