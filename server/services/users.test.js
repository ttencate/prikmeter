const chai = { expect } = require('chai')
const chaiAsPromised = require('chai-as-promised')

const users = require('./users')
const testDb = require('../core/testDb')

chai.use(chaiAsPromised)

const BCRYPT_ROUNDS = 4 // Make tests fast. This appears to be the minimum.

describe('services/users', () => {
  beforeEach(testDb.reset)

  describe('get', () => {

    it('returns undefined if the user ID is not found', async () => {
      await expect(users.get({ id: testDb.data.nonexistentUser.id })).to.eventually.equal(undefined)
    })

    it('returns undefined if the user email is not found', async () => {
      await expect(users.get({ email: testDb.data.nonexistentUser.email })).to.eventually.equal(undefined)
    })

    it('returns the user by ID', async () => {
      const user = await users.get({ id: testDb.data.user.id })
      expect(user.id).to.equal(testDb.data.user.id)
      expect(user.email).to.equal(testDb.data.user.email)
      expect(user.passwordHash).to.equal(testDb.data.user.passwordHash)
    })

    it('returns the user by email', async () => {
      const user = await users.get({ email: testDb.data.user.email })
      expect(user.id).to.equal(testDb.data.user.id)
      expect(user.email).to.equal(testDb.data.user.email)
      expect(user.passwordHash).to.equal(testDb.data.user.passwordHash)
    })
  })

  describe('create', () => {
    it('throws if the email is already in use', async () => {
      await expect(users.create({ email: testDb.data.user.email, password: 'somethingelse', bcryptRounds: BCRYPT_ROUNDS })).to.be.rejected
    })

    it('creates and returns the user', async () => {
      const user = await users.create({ email: testDb.data.nonexistentUser.email, password: testDb.data.nonexistentUser.password, bcryptRounds: BCRYPT_ROUNDS })
      expect(user.id).to.be.a('number')
    })
  })
})

