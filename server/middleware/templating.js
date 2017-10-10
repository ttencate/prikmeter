const expressNunjucks = require('express-nunjucks')

const TEMPLATES_DIR = __dirname + '/../templates'

module.exports.install = function install (app) {
  app.set('views', TEMPLATES_DIR)
  const njk = expressNunjucks(app, {
    watch: app.locals.devMode,
    noCache: app.locals.devMode
  })
}
