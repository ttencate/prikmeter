#!/usr/bin/env node

const crypto = require('crypto')
const readlineSync = require('readline-sync')
require('make-promises-safe')

async function main () {
  const knex = await require('./db').createKnex()

  const meterId = readlineSync.question('Meter ID: ')

  const token = crypto.randomBytes(32).toString('hex')
  await knex('authTokens').insert({ token: token, meterId: meterId })
  process.stdout.write(`Created token: ${token}\n`)

  process.exit(0)
}

main()

