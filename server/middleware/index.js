const expressPromiseRouter = require('express-promise-router')

module.exports.install = async function install (app) {
  app.use(expressPromiseRouter())

  await require('./sessions').install(app)
  require('./templating').install(app)
}
