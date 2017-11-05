const path = require('path')

const login = require('./login')
const root = require('./root')
const static = require('./static')
const telegrams = require('./telegrams')
// const users = require('./users')

module.exports.addRoutes = async function addRoutes (app) {
  await static.init(app)

  const rootPathTo = pathFromRoot => path.join(__dirname, '..', pathFromRoot)

  app.use('/$', root.root)

  app.use('/static', static.static)

  app.post('/login$', login.login)
  app.post('/logout$', login.logout)

  // User creation from the web interface is untested, so disabled for now.
  // app.post('/users$', users.create)

  app.post('/telegrams$', telegrams.create)
}
