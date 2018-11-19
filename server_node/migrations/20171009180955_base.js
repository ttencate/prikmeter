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
    table.string('id').primary()
    table.string('type')
    table.integer('ownerUserId')
    table.foreign('ownerUserId').references('users.id').onDelete('set null')
  })

  await knex.schema.createTable('authTokens', function (table) {
    table.string('token', 64).primary().notNullable()
    table.integer('ownerUserId')
    table.foreign('ownerUserId').references('users.id').onDelete('set null')
  })

  await knex.schema.createTable('telegrams', function (table) {
    table.increments('id')
    table.integer('ownerUserId')
    table.foreign('ownerUserId').references('users.id').onDelete('set null')
    table.binary('telegram')
    // "true" means *don't* store a timezone with the date/time and got here by
    // accident.
    table.timestamp('uploadTimestamp', true).defaultTo(knex.fn.now())
  })

  await knex.schema.createTable('electricityReadings', function (table) {
    table.string('meterId')
    // "true" means *don't* store a timezone with the date/time and got here by
    // accident.
    table.timestamp('timestamp', true)
    table.double('totalConsumptionKwhLow')
    table.double('totalConsumptionKwhHigh')
    table.double('totalProductionKwhLow')
    table.double('totalProductionKwhHigh')
    table.double('currentConsumptionKw')
    table.double('currentProductionKw')
    table.primary(['meterId', 'timestamp'])
  })

  await knex.schema.createTable('gasReadings', function (table) {
    table.string('meterId')
    // "true" means *don't* store a timezone with the date/time and got here by
    // accident.
    table.timestamp('timestamp', true)
    table.double('totalConsumptionM3')
    table.primary(['meterId', 'timestamp'])
  })
}

module.exports.down = async function (knex) {
  await knex.schema.dropTable('gasReadings')
  await knex.schema.dropTable('electricityReadings')
  await knex.schema.dropTable('telegrams')
  await knex.schema.dropTable('authTokens')
  await knex.schema.dropTable('meters')
  await knex.schema.dropTable('users')
  await knex.schema.dropTable('settings')
}
