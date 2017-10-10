#!/usr/bin/env node

const readlineSync = require('readline-sync')
require('make-promises-safe')

const users = require('../services/users')
const meters = require('../services/meters')

async function main () {
  await require('../storage/migrate')()

  const email = readlineSync.question('Email: ')

  const user = await users.get({ email })
  if (!user) {
    throw Error('User not found')
  }

  const name = readlineSync.question('Meter name: ')
  const meterId = await meters.create({ name: name, ownerUserId: user.id })
  process.stdout.write(`Meter ${meterId} created\n`)

  process.exit(0)
}

main()
