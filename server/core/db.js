const dbConfig = require('./dbConfig')

module.exports = require('knex')(dbConfig)
