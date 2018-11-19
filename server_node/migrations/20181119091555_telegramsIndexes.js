module.exports.up = async function (knex) {
  await knex.schema.table('telegrams', function (table) {
    table.index('uploadTimestamp')
  })
}

module.exports.down = async function (knex) {
  await knex.schema.table('telegrams', function (table) {
    table.dropIndex('uploadTimestamp')
  })
}
