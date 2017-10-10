module.exports.addRoutes = function addRoutes (app) {
  const root = require('./root')
  app.use('/$', root.root)

  const login = require('./login')
  app.post('/login$', login.login)
  app.post('/logout$', login.logout)

  const users = require('./users')
  // User creation is untested, so disabled for now.
  // app.post('/users$', users.create)

  const telegrams = require('./telegrams')
  app.post('/telegrams$', telegrams.create)
}
