const chai = { expect } = require('chai')
const chaiAsPromised = require('chai-as-promised')

const users = require('./users')
const testDb = require('../storage/testDb')

chai.use(chaiAsPromised)

const EMAIL = 'example@example.com'
const PASSWORD = 'supersecret'
const BCRYPT_ROUNDS = 4 // Make tests fast. This appears to be the minimum.
const BCRYPT_RE = /^\$(2a|2b|2y)\$04\$.*$/

describe('users', () => {
  let userId

  beforeEach(testDb.reset)

  beforeEach(async () => {
    userId = await users.create({ email: EMAIL, password: PASSWORD, bcryptRounds: BCRYPT_ROUNDS })
  })

  describe('get', () => {

    it('returns undefined if the user ID is not found', async () => {
      await expect(users.get({ id: userId + 1 })).to.eventually.equal(undefined)
    })

    it('returns undefined if the user email is not found', async () => {
      await expect(users.get({ email: 'notfound@example.com' })).to.eventually.equal(undefined)
    })

    it('returns the user by ID', async () => {
      const user = await users.get({ id: userId })
      expect(user.id).to.equal(userId)
      expect(user.email).to.equal(EMAIL)
      expect(user.passwordHash).to.match(BCRYPT_RE)
    })

    it('returns the user by email', async () => {
      const user = await users.get({ email: EMAIL })
      expect(user.id).to.equal(userId)
      expect(user.email).to.equal(EMAIL)
      expect(user.passwordHash).to.match(BCRYPT_RE)
    })
  })

  describe('create', () => {
    it('throws if the email is already in use', async () => {
      await expect(users.create({ email: EMAIL, password: 'somethingelse', bcryptRounds: BCRYPT_ROUNDS })).to.be.rejected
    })
  })
})

