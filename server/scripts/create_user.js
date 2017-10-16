#!/usr/bin/env node

const readlineSync = require('readline-sync')
require('make-promises-safe')

const users = require('../services/users')

async function main () {
  await require('../storage/migrate')()

  const email = readlineSync.question('Email: ')
  const password = readlineSync.question('Password: ', { hideEchoBack: true, mask: '' })

  process.stdout.write('Creating user...\n')
  const user = await users.create({ email, password })
  process.stdout.write(`User ${user.id} created\n`)

  process.exit(0)
}

main()
