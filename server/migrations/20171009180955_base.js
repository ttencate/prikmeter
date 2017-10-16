module.exports.up = async function (knex) {
  await knex.schema.createTable('settings', function (table) {
    table.string('key').primary()
    table.string('value')
  })

  await knex.schema.createTable('users', function (table) {
    table.increments('id')
    table.string('email').unique()
    table.string('passwordHash', 60)
    table.index('email')
  })

  await knex.schema.createTable('meters', function (table) {
    table.increments('id')
    table.string('name')
    table.integer('ownerUserId')
    table.foreign('ownerUserId').references('users.id')
  })

  await knex.schema.createTable('authTokens', function (table) {
    table.string('token', 64).primary().notNullable()
    table.integer('ownerUserId')
    table.foreign('ownerUserId').references('users.id')
  })

  await knex.schema.createTable('telegrams', function (table) {
    table.increments('id')
    table.integer('ownerUserId')
    table.foreign('ownerUserId').references('users.id')
    table.binary('telegram')
    table.timestamp('uploadTimestamp').defaultTo(knex.fn.now())
  })
}

module.exports.down = async function (knex) {
  await knex.schema.dropTable('telegrams')
  await knex.schema.dropTable('authTokens')
  await knex.schema.dropTable('meters')
  await knex.schema.dropTable('users')
  await knex.schema.dropTable('settings')
}
