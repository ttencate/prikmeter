module.exports.addRoutes = function addRoutes (app) {
  const root = require('./root')
  app.use('/$', root.root)

  const login = require('./login')
  app.post('/login$', login.login)
  app.post('/logout$', login.logout)

  // User creation is untested, so disabled for now.
  // const users = require('./users')
  // app.post('/users$', users.create)

  const telegrams = require('./telegrams')
  app.post('/telegrams$', telegrams.create)
}
