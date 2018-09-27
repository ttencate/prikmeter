const expressNunjucks = require('express-nunjucks')
const nunjucks = require('nunjucks')
const path = require('path')

const TEMPLATES_DIR = path.join(__dirname, '/../templates')

function jsonInsideScriptTag (value, spaces) {
  if (value instanceof nunjucks.runtime.SafeString) {
    value = value.toString()
  }
  let jsonString = JSON.stringify(value, null, spaces)
  if (jsonString) {
    jsonString = jsonString.replace(/</g, '\\u003c')
  }
  return nunjucks.runtime.markSafe(jsonString)
}

module.exports.install = function install (app) {
  app.set('views', TEMPLATES_DIR)
  const nj = expressNunjucks(app, {
    watch: app.get('env') === 'development',
    noCache: app.get('env') === 'development'
  })

  nj.env.addFilter('jsonInsideScriptTag', jsonInsideScriptTag)
}
