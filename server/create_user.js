#!/usr/bin/env node

const bcryptPromise = require('bcrypt-promise')
const readlineSync = require('readline-sync')
require('make-promises-safe')

async function main () {
  const knex = await require('./db').createKnex()

  const email = readlineSync.question('Email: ')
  const password = readlineSync.question('Password: ', { hideEchoBack: true, mask: '' })

  process.stdout.write('Hashing password...\n')
  const hash = await bcryptPromise.hash(password, 12)

  process.stdout.write('Creating user...\n')
  const userId = await knex('users').insert({ email: email, passwordHash: hash }).returning('id')
  process.stdout.write(`User ${userId} created\n`)

  process.exit(0)
}

main()
