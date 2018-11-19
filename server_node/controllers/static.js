const express = require('express')
const path = require('path')
const webpack = require('webpack')
const webpackDevMiddleware = require('webpack-dev-middleware')

const log = require('../core/log')

async function init (app) {
  // https://webpack.js.org/api/node/
  const webpackConfig = require('../webpack.' + app.get('env'))
  const webpackCompiler = webpack(webpackConfig)
  const promise = new Promise(function (resolve, reject) {
    if (app.get('env') === 'development') {
      app.use(webpackDevMiddleware(webpackCompiler, {
        publicPath: webpackConfig.output.publicPath,
        stats: webpackConfig.stats
      }))
      resolve()
    } else {
      log.info('Compiling static assets...')
      webpackCompiler.run(function (err, stats) {
        if (err) {
          log.error(err.stack || err)
          if (err.details) {
            log.error(err.details)
          }
          reject(err)
          return
        }

        const info = stats.toJson()

        if (stats.hasErrors()) {
          log.error(info.errors)
          reject(info.errors)
          return
        }

        if (stats.hasWarnings()) {
          log.warn(info.warnings)
        }

        resolve()
      })
    }
  })
  return promise
}

module.exports = {
  init: init,
  static: express.static(path.join(__dirname, '..', 'static'))
}
