const path = require('path')

const expressNunjucks = require('express-nunjucks')

const TEMPLATES_DIR = path.join(__dirname, '/../templates')

module.exports.install = function install (app) {
  app.set('views', TEMPLATES_DIR)
  expressNunjucks(app, {
    watch: app.locals.devMode,
    noCache: app.locals.devMode
  })
}
