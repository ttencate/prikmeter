const bcryptPromise = require('bcrypt-promise')
const bodyParser = require('body-parser')
const { check, validationResult } = require('express-validator/check')
const crypto = require('crypto')
const express = require('express')
const expressNunjucks = require('express-nunjucks')
const expressPromiseRouter = require('express-promise-router')
const expressSession = require('express-session')
const promisify = require('promisify-node')
require('make-promises-safe')

const db = require('./db')

async function getCookieSecret(knex) {
  const secretSetting = await knex.from('settings').where({ key: 'cookieSecret' }).first('value')
  if (secretSetting) {
    return secretSetting.value
  } else {
    const secret = crypto.randomBytes(32).toString('hex')
    await knex('settings').insert({ key: 'cookieSecret', value: secret })
    return secret
  }
}

async function createApp (env, knex) {
  const app = express()
  app.locals.devMode = app.get('env') === 'development'
  app.set('views', __dirname + '/templates')

  app.use(expressPromiseRouter())

  const cookieSecret = await getCookieSecret(knex)
  app.use(expressSession({
    cookie: { httpOnly: true },
    secret: cookieSecret,
    saveUninitialized: false,
    resave: false
  }))

  const njk = expressNunjucks(app, {
    watch: app.locals.devMode,
    noCache: app.locals.devMode
  })

  app.get('/', async function (req, res) {
    const user = req.session.user;
    let meters
    if (user) {
      const queryResult = await knex.from('meters').where({ ownerUserId: user.id })
          .leftJoin('authTokens', 'meters.id', 'authTokens.meterId')
          .select('meters.id', 'meters.name', 'authTokens.token')
      const metersById = {}
      for (row of queryResult) {
        metersById[row.id] = metersById[row.id] || { id: row.id, name: row.name, authTokens: [] }
        metersById[row.id].authTokens.push(row.token)
      }
      meters = Object.values(metersById)
    }
    res.render('index', {
      user: user,
      return_url: req.url,
      meters: meters
    })
  })

  app.use('/login', bodyParser.urlencoded({ extended: false }))
  app.post('/login',
    check('email').exists(),
    check('password').exists(),
    async function (req, res) {
      const user = await knex.from('users').where({ email: req.body.email }).first('id', 'email', 'passwordHash')
      const passwordHash = user ? user.passwordHash : 'invalid'
      const passwordCorrect = await bcryptPromise.compare(req.body.password, passwordHash)
      if (!passwordCorrect) {
        res.render('index', {
          errors: ['Password incorrect']
        })
        return
      }

      req.session.user = { id: user.id, email: user.email }

      res.redirect(req.body.return_url || '/')
    })

  app.post('/logout', async function (req, res) {
    await promisify(req.session.destroy())
    res.redirect('/')
  })

  app.use('/users', bodyParser.urlencoded({ extended: false }))
  app.post('/users',
    check('email', 'Not a valid email address').isEmail().trim(),
    check('password1', 'Password must be at least 8 characters').isLength({ min: 8 }),
    check('password2', 'Passwords do not match').exists().custom((value, { req }) => value === req.body.password1),
    async function (req, res) {
      const errors = validationResult(req)
      if (!errors.isEmpty()) {
        res.render('index', {
          errors: errors.array().map(error => error.msg)
        })
        return
      }

      session.user = { email: req.body.email }

      res.redirect('/')
    })

  app.use('/telegrams', bodyParser.raw({ type: 'text/plain' })) // Populates req.body as a Buffer.
  app.post('/telegrams', function (req, res) {
    const telegram = req.body.toString()
    console.log(telegram)

    res.sendStatus(200)
  })

  return app
}

async function main () {
  const env = process.env.NODE_ENV || 'development'

  const knex = await db.createKnex(env)
  const app = await createApp(env, knex)

  const host = 'localhost'
  const port = 3000
  app.listen(port, host)
  console.log(`Listening on http://${host}:${port}/`)
}

main()
