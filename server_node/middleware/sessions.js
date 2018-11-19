const crypto = require('crypto')
const expressSession = require('express-session')
const knexSessionStore = require('connect-session-knex')

const db = require('../core/db')
const settings = require('../services/settings')

const COOKIE_SECRET_KEY = 'cookieSecret'

async function getCookieSecret () {
  let secret = await settings.getValue(COOKIE_SECRET_KEY)
  if (!secret) {
    secret = crypto.randomBytes(32).toString('hex')
    await settings.setValue(COOKIE_SECRET_KEY, secret)
  }
  return secret
}

function createStore () {
  const KnexSessionStore = knexSessionStore(expressSession)
  return new KnexSessionStore({
    tablename: 'sessions',
    knex: db,
    createtable: true
  })
}

module.exports.install = async function install (app) {
  const cookieSecret = await getCookieSecret()
  app.use(expressSession({
    cookie: {
      httpOnly: true,
      secure: app.get('env') === 'production',
      maxAge: 1000 * 60 * 60 * 24 * 31 // One month
    },
    name: 's',
    secret: cookieSecret,
    saveUninitialized: false,
    store: createStore(),
    resave: false
  }))
}
