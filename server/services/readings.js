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
    if (ex.code === 'SQLITE_CONSTRAINT' /* SQLite */
      || (ex.constraint || '').endsWith('_pkey') /* PostgreSQL */) {
      return
    }
    throw ex
  }
}

async function getForMeter (table, keys, meter, { startTime, endTime, resolution }) {
  let query = db.from(table)
    .where({ meterId: meter.id })
    .orderBy('timestamp', 'asc')
  if (startTime) {
    query = query.whereRaw('timestamp >= ?', [startTime])
  }
  if (endTime) {
    query = query.whereRaw('timestamp <= ?', [endTime])
  }
  // TODO downsample using max() and mean() aggregates per column as appropriate
  // switch (resolution) {
  //   case 'hour':
  //     query = query.groupBy()
  //     break
  //   default:
  // }

  const readings = await query.select(...keys)
  readings.forEach((reading) => {
    reading.type = meter.type
  })
  return readings
}

module.exports = {
  create: async function (reading) {
    switch (reading.type) {
      case 'electricity':
        return createOrIgnore('electricityReadings', ELECTRICITY_KEYS, reading)
      case 'gas':
        return createOrIgnore('gasReadings', GAS_KEYS, reading)
      default:
        throw new Error(`Unknown reading type "${reading}"`)
    }
  },

  getForMeter: async function (meter, params = {}) {
    switch (meter.type) {
      case 'electricity':
        return getForMeter('electricityReadings', ELECTRICITY_KEYS, meter, params)
      case 'gas':
        return getForMeter('gasReadings', GAS_KEYS, meter, params)
      default:
        throw new Error(`Unknown reading type "${reading}"`)
    }
  }
}
