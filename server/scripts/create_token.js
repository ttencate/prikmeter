#!/usr/bin/env node

const readlineSync = require('readline-sync')
require('make-promises-safe')

const authTokens = require('../services/auth-tokens')
const users = require('../services/users')

async function main () {
  await require('../storage/migrate')()

  const email = readlineSync.question('User email: ')
  const user = await users.get({ email })
  if (!user) {
    throw Error('User not found')
  }

  const token = await authTokens.create(user)
  process.stdout.write(`Created token: ${token}\n`)

  process.exit(0)
}

main()
