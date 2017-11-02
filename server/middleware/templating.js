const expressNunjucks = require('express-nunjucks')
const nunjucks = require('nunjucks')
const path = require('path')

const TEMPLATES_DIR = path.join(__dirname, '/../templates')

function jsonInsideScriptTag (value, spaces) {
  if (value instanceof nunjucks.runtime.SafeString) {
    value = value.toString()
  }
  const jsonString = JSON.stringify(value, null, spaces).replace(/</g, '\\u003c')
  return nunjucks.runtime.markSafe(jsonString)
}

module.exports.install = function install (app) {
  app.set('views', TEMPLATES_DIR)
  const nj = expressNunjucks(app, {
    watch: app.locals.devMode,
    noCache: app.locals.devMode
  })

  nj.env.addFilter('jsonInsideScriptTag', jsonInsideScriptTag)
}
