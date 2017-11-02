const login = require('./login')
const root = require('./root')
const static = require('./static')
const telegrams = require('./telegrams')
// const users = require('./users')

module.exports.addRoutes = function addRoutes (app) {
  app.use('/$', root.root)

  app.use('/static/scripts', static.scripts);

  app.post('/login$', login.login)
  app.post('/logout$', login.logout)

  // User creation from the web interface is untested, so disabled for now.
  // app.post('/users$', users.create)

  app.post('/telegrams$', telegrams.create)
}
