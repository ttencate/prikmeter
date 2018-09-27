#!/usr/bin/env node

const readlineSync = require('readline-sync')
require('make-promises-safe')

const authTokens = require('../services/authTokens')
const users = require('../services/users')

async function main () {
  await require('../core/migrate')()

  const email = readlineSync.question('User email: ')
  const user = await users.get({ email })
  if (!user) {
    throw Error('User not found')
  }

  const authToken = await authTokens.create(user)
  process.stdout.write(`Created token: ${authToken.token}\n`)

  process.exit(0)
}

main()
