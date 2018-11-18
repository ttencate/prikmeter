const moment = require('moment')

const dbConfig = require('../core/dbConfig')

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
  telegramDsmr50: {
    ownerUserId: undefined,
    telegram: '/Ene5\\XS210 ESMR 5.0\r\n\r\n1-3:0.2.8(50)\r\n0-0:1.0.0(181118190728W)\r\n0-0:96.1.1(4530303437303030303231393535383138)\r\n1-0:1.8.1(000439.905*kWh)\r\n1-0:1.8.2(000393.772*kWh)\r\n1-0:2.8.1(000174.566*kWh)\r\n1-0:2.8.2(000407.609*kWh)\r\n0-0:96.14.0(0001)\r\n1-0:1.7.0(00.841*kW)\r\n1-0:2.7.0(00.000*kW)\r\n0-0:96.7.21(00068)\r\n0-0:96.7.9(00001)\r\n1-0:99.97.0(0)(0-0:96.7.19)\r\n1-0:32.32.0(00001)\r\n1-0:32.36.0(00000)\r\n0-0:96.13.0()\r\n1-0:32.7.0(223.0*V)\r\n1-0:31.7.0(003*A)\r\n1-0:21.7.0(00.841*kW)\r\n1-0:22.7.0(00.000*kW)\r\n0-1:24.1.0(003)\r\n0-1:96.1.0(4730303533303033363933343335313138)\r\n0-1:24.2.1(181118190500W)(00256.644*m3)\r\n!6E6D\r\n',
  },

  meter: {
    id: 'E0123456789',
    type: 'electricity',
    ownerUserId: undefined
  },
  electricityMeter: {
    id: 'E0005001563265514',
    type: 'electricity',
    ownerUserId: undefined
  },
  gasMeter: {
    id: 'G0002340134445914',
    type: 'gas',
    ownerUserId: undefined
  },
  nonexistentMeter: {
    id: 'G9876543210',
    type: 'gas',
    ownerUserId: undefined
  },

  electricityReading: {
    meterId: 'E0005001563265514',
    timestamp: moment('2017-10-11T15:40:59+00:00').toDate(),
    totalConsumptionKwhLow: 1677.034,
    totalConsumptionKwhHigh: 2060.771,
    totalProductionKwhLow: 0.0,
    totalProductionKwhHigh: 0.0,
    currentConsumptionKw: 0.330,
    currentProductionKw: 0.0
  },
  gasReading: {
    meterId: 'G0002340134445914',
    timestamp: moment('2017-10-11T15:00:00+00:00').toDate(),
    totalConsumptionM3: 3964.814
  }
}

module.exports = {
  data: data,

  seed: async function (db) {
    await db('users').delete();
    const [ userId ] = await db('users').insert({ email: data.user.email, passwordHash: data.user.passwordHash }).returning('id')
    data.user.id = userId
    data.authToken.ownerUserId = userId
    data.telegram.ownerUserId = userId
    data.meter.ownerUserId = userId
    data.electricityMeter.ownerUserId = userId
    data.gasMeter.ownerUserId = userId
    data.nonexistentMeter.ownerUserId = userId
    data.nonexistentUser.id = userId + 1000000

    await db('authTokens').delete()
    await db('authTokens').insert(data.authToken)

    await db('telegrams').delete()
    await db('telegrams').insert(data.telegram)

    await db('meters').delete()
    await db('meters').insert(data.meter)

    await db('electricityReadings').delete()

    await db('gasReadings').delete()
  }
}
