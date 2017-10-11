const crypto = require('crypto')
const expressSession = require('express-session')

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

module.exports.install = async function install (app) {
  const cookieSecret = await getCookieSecret()
  app.use(expressSession({
    cookie: { httpOnly: true },
    secret: cookieSecret,
    saveUninitialized: false,
    resave: false
  }))
}