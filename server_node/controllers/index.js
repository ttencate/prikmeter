const login = require('./login')
const readings = require('./readings')
const root = require('./root')
const staticController = require('./static')
const telegrams = require('./telegrams')
// const users = require('./users')

module.exports.addRoutes = async function addRoutes (app) {
  await staticController.init(app)

  app.use('/$', root.root)

  app.use('/static', staticController.static)

  app.post('/login$', login.login)
  app.post('/logout$', login.logout)

  // User creation from the web interface is untested, so disabled for now.
  // app.post('/users$', users.create)

  app.post('/telegrams$', telegrams.create)

  app.get('/meters/:meterId/readings$', readings.get)
}
