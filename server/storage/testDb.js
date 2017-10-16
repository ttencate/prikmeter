const db = require('./db')

function listTablesQuery (client) {
  switch (client) {
    case 'postgresql': return 'SELECT tablename FROM pg_catalog.pg_tables WHERE schemaname="public"'
    case 'sqlite3': return 'SELECT name FROM sqlite_master WHERE type="table"'
    default: throw new Error(`Listing tables for connection type '${client}' is not currently supported`)
  }
}

async function listTables (db) {
  const tables = await db.raw(listTablesQuery(db.client.config.client))
  return tables.map(row => row.name)
}

function shouldClearTable (table) {
  return table !== db.client.config.migrations.tableName
}

module.exports.reset = async function () {
  await db.migrate.latest()

  const tables = (await listTables(db)).filter(shouldClearTable)
  const promises = tables.map(table => db(table).delete())
  await Promise.all(promises)
}
