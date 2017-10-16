const data = {
  user: {
    id: undefined,
    email: 'example@example.com',
    password: 'test',
    passwordHash: '$2a$12$.9rM78aPE52sbbLDj4Hj6.EPMUmJjq2pzb7uNX1Bx9mJjJyAJop4a'
  },
  nonexistentUser: {
    id: undefined,
    email: 'nonexistent@example.com',
    password: 'test',
    passwordHash: '$2a$12$.9rM78aPE52sbbLDj4Hj6.EPMUmJjq2pzb7uNX1Bx9mJjJyAJop4a'
  },

  authToken: {
    token: '372554dadeb3affe2ffeb73890bec1203c4a814198965a2a0c51d9a9dcb4d0bc',
    ownerUserId: undefined
  },
  nonexistentAuthToken: {
    token: '0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef',
    ownerUserId: undefined
  }
}

module.exports = {
  data: data,

  seed: async function (db) {
    await db('users').delete();
    ([ data.user.id ] = await db('users').insert({ email: data.user.email, passwordHash: data.user.passwordHash }).returning('id'))
    data.nonexistentUser.id = data.user.id + 1000000

    await db('authTokens').delete()
    await db('authTokens').insert({ token: data.authToken.token, ownerUserId: data.user.id })
    data.authToken.ownerUserId = data.user.id
  }
}
