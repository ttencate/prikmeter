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


/**
 * resolution: one of https://www.postgresql.org/docs/9.1/static/functions-datetime.html#FUNCTIONS-DATETIME-TRUNC
 */
async function getForMeter (table, keys, meter, { startTime, endTime, resolution }) {
  resolution = resolution || 'second'
  let query = db.from(table)
    .where({ meterId: meter.id })
    .orderBy('timestamp', 'asc')
  if (startTime) {
    query = query.where(db.raw('timestamp >= ?', [startTime]))
  }
  if (endTime) {
    query = query.where(db.raw('timestamp <= ?', [endTime]))
  }
  query = query.groupBy(db.raw('date_trunc(?, timestamp)', [resolution]))
  const select = db.raw(keys.map(key => `max("${key}") as "${key}"`).join(', '))
  const readings = await query.select(select)
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
