#!/usr/bin/env node

const readlineSync = require('readline-sync')
require('make-promises-safe')

const meters = require('../services/meters')

async function main () {
  await require('../storage/migrate')()

  const meterId = readlineSync.question('Meter ID: ')
  const meter = await meters.get({ id: meterId })
  if (!meter) {
    throw Error('Meter not found')
  }

  const token = await meters.createAuthToken(meter)
  process.stdout.write(`Created token: ${token}\n`)

  process.exit(0)
}

main()
