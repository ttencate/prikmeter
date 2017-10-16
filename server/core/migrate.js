const db = require('./db')

module.exports = async function migrate () {
  let currentVersion = await db.migrate.currentVersion()
  await db.migrate.latest()
  let latestVersion = await db.migrate.currentVersion()

  if (currentVersion !== latestVersion) {
    console.log(`Upgrading database from version ${currentVersion} to version ${latestVersion}...`)
    await db.migrate.latest()
    console.log('Database upgraded')
  } else {
    console.log('Database is already at latest version ' + latestVersion)
  }
}
