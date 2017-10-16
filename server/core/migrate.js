const db = require('./db')
const log = require('./log')

module.exports = async function migrate () {
  let currentVersion = await db.migrate.currentVersion()
  await db.migrate.latest()
  let latestVersion = await db.migrate.currentVersion()

  if (currentVersion !== latestVersion) {
    log.info(`Upgrading database from version ${currentVersion} to version ${latestVersion}...`)
    await db.migrate.latest()
    log.info('Database upgraded')
  } else {
    log.info('Database is already at latest version ' + latestVersion)
  }
}
