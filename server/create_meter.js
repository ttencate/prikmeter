#!/usr/bin/env node

const readlineSync = require('readline-sync')
require('make-promises-safe')

async function main () {
  const knex = await require('./db').createKnex()

  const email = readlineSync.question('Email: ')

  const user = await knex('users').where({ email: email }).first('id')
  if (!user) {
    throw Error('User not found')
  }

  const name = readlineSync.question('Meter name: ')
  const meterId = await knex('meters').insert({ name: name, ownerUserId: user.id }).returning('id')
  process.stdout.write(`Meter ${meterId} created\n`)

  process.exit(0)
}

main()
