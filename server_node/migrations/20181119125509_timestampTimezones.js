module.exports.up = async function (knex) {
  // Knex's alter() function keeps trying to make these columns nullable
  // despite adding notNull() explicitly. Raw queries just work, albeit
  // probably only in PostgreSQL.
  await knex.raw('alter table "telegrams" alter column "uploadTimestamp" type timestamp with time zone;')
  await knex.raw('alter table "electricityReadings" alter column "timestamp" type timestamp with time zone;')
  await knex.raw('alter table "gasReadings" alter column "timestamp" type timestamp with time zone;')
}

module.exports.down = async function (knex) {
  await knex.raw('alter table "telegrams" alter column "uploadTimestamp" type timestamp without time zone;')
  await knex.raw('alter table "electricityReadings" alter column "timestamp" type timestamp without time zone;')
  await knex.raw('alter table "gasReadings" alter column "timestamp" type timestamp without time zone;')
}
