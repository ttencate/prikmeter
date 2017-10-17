const db = require('../core/db')

const ELECTRICITY_KEYS = ['meterId', 'timestamp', 'totalConsumptionKwhLow', 'totalConsumptionKwhHigh', 'totalProductionKwhLow', 'totalProductionKwhHigh', 'currentConsumptionKw', 'currentProductionKw']
const GAS_KEYS = ['meterId', 'timestamp', 'totalConsumptionM3']

async function createOrIgnore (table, keys, reading) {
  const object = {}
  for (key of keys) {
    object[key] = reading[key] !== undefined ? reading[key] : null
  }
  try {
    await db(table).insert(object)
  } catch (ex) {
    if (ex.code == 'SQLITE_CONSTRAINT') {
      return
    }
    throw ex
  }
}

async function getForMeter (table, keys, meter) {
  const readings = await db.from(table).where({ meterId: meter.id }).select(...keys)
  return readings
}

module.exports = {
  createElectricity: async function (reading) {
    await createOrIgnore('electricityReadings', ELECTRICITY_KEYS, reading)
  },

  createGas: async function (reading) {
    await createOrIgnore('gasReadings', GAS_KEYS, reading)
  },

  getElectricityForMeter: async function (meter) {
    return getForMeter('electricityReadings', ELECTRICITY_KEYS, meter)
  },

  getGasForMeter: async function (meter) {
    return getForMeter('gasReadings', GAS_KEYS, meter)
  }
}
