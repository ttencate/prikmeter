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
  },

  telegram: {
    ownerUserId: undefined,
    telegram: '/XMX5LGBBFFB231117791\r\n\r\n1-3:0.2.8(42)\r\n0-0:1.0.0(171011174059S)\r\n0-0:96.1.1(4530303035303031353633323635353134)\r\n1-0:1.8.1(001677.034*kWh)\r\n1-0:2.8.1(000000.000*kWh)\r\n1-0:1.8.2(002060.771*kWh)\r\n1-0:2.8.2(000000.000*kWh)\r\n0-0:96.14.0(0002)\r\n1-0:1.7.0(00.330*kW)\r\n1-0:2.7.0(00.000*kW)\r\n0-0:96.7.21(00005)\r\n0-0:96.7.9(00002)\r\n1-0:99.97.0(2)(0-0:96.7.19)(151007113802S)(0000003567*s)(150817150911S)(0000003252*s)\r\n1-0:32.32.0(00000)\r\n1-0:52.32.0(00001)\r\n1-0:72.32.0(00001)\r\n1-0:32.36.0(00000)\r\n1-0:52.36.0(00000)\r\n1-0:72.36.0(00000)\r\n0-0:96.13.1()\r\n0-0:96.13.0()\r\n1-0:31.7.0(000*A)\r\n1-0:51.7.0(002*A)\r\n1-0:71.7.0(000*A)\r\n1-0:21.7.0(00.003*kW)\r\n1-0:41.7.0(00.319*kW)\r\n1-0:61.7.0(00.008*kW)\r\n1-0:22.7.0(00.000*kW)\r\n1-0:42.7.0(00.000*kW)\r\n1-0:62.7.0(00.000*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(4730303032333430313334343435393134)\r\n0-1:24.2.1(171011170000S)(03964.814*m3)\r\n!D0FD\r\n'
  },
  corruptTelegram: {
    ownerUserId: undefined,
    telegram: '/YMX5LGBBFFB231117791\r\n\r\n1-3:0.2.8(42)\r\n0-0:1.0.0(171011174059S)\r\n0-0:96.1.1(4530303035303031353633323635353134)\r\n1-0:1.8.1(001677.034*kWh)\r\n1-0:2.8.1(000000.000*kWh)\r\n1-0:1.8.2(002060.771*kWh)\r\n1-0:2.8.2(000000.000*kWh)\r\n0-0:96.14.0(0002)\r\n1-0:1.7.0(00.330*kW)\r\n1-0:2.7.0(00.000*kW)\r\n0-0:96.7.21(00005)\r\n0-0:96.7.9(00002)\r\n1-0:99.97.0(2)(0-0:96.7.19)(151007113802S)(0000003567*s)(150817150911S)(0000003252*s)\r\n1-0:32.32.0(00000)\r\n1-0:52.32.0(00001)\r\n1-0:72.32.0(00001)\r\n1-0:32.36.0(00000)\r\n1-0:52.36.0(00000)\r\n1-0:72.36.0(00000)\r\n0-0:96.13.1()\r\n0-0:96.13.0()\r\n1-0:31.7.0(000*A)\r\n1-0:51.7.0(002*A)\r\n1-0:71.7.0(000*A)\r\n1-0:21.7.0(00.003*kW)\r\n1-0:41.7.0(00.319*kW)\r\n1-0:61.7.0(00.008*kW)\r\n1-0:22.7.0(00.000*kW)\r\n1-0:42.7.0(00.000*kW)\r\n1-0:62.7.0(00.000*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(4730303032333430313334343435393134)\r\n0-1:24.2.1(171011170000S)(03964.814*m3)\r\n!D0FD\r\n'
  },
}

module.exports = {
  data: data,

  seed: async function (db) {
    await db('users').delete();
    ([ data.user.id ] = await db('users').insert({ email: data.user.email, passwordHash: data.user.passwordHash }).returning('id'))
    data.authToken.ownerUserId = data.user.id
    data.telegram.ownerUserId = data.user.id
    data.nonexistentUser.id = data.user.id + 1000000

    await db('authTokens').delete()
    await db('authTokens').insert({ token: data.authToken.token, ownerUserId: data.authToken.ownerUserId })

    await db('telegrams').delete()
    await db('telegrams').insert({ ownerUserId: data.telegram.ownerUserId, telegram: data.telegram.telegram })
  }
}
