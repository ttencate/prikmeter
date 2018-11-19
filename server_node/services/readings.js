const db = require('../core/db')

const KEYS = {
  electricity: ['meterId', 'timestamp', 'totalConsumptionKwhLow', 'totalConsumptionKwhHigh', 'totalProductionKwhLow', 'totalProductionKwhHigh', 'currentConsumptionKw', 'currentProductionKw'],
  gas: ['meterId', 'timestamp', 'totalConsumptionM3']
}

const TABLES = {
  electricity: 'electricityReadings',
  gas: 'gasReadings'
}

async function createOrIgnore (table, keys, reading) {
  const object = {}
  for (const key of keys) {
    object[key] = reading[key] !== undefined ? reading[key] : null
  }
  try {
    await db(table).insert(object)
  } catch (ex) {
    if (ex.code === 'SQLITE_CONSTRAINT' /* SQLite */ ||
        (ex.constraint || '').endsWith('_pkey') /* PostgreSQL */) {
      return
    }
    throw ex
  }
}

/**
 * resolution: one of https://www.postgresql.org/docs/9.1/static/functions-datetime.html#FUNCTIONS-DATETIME-TRUNC
 * e.g. 'day', 'hour', 'minute', 'second'
 */
async function getForMeter (table, keys, meter, { startTime, endTime, resolution }) {
  let query = db.from(table)
      .where({ meterId: meter.id })
      .orderBy('timestamp', 'asc')
  if (startTime) {
    query = query.where(db.raw('timestamp >= ?', [startTime]))
  }
  if (endTime) {
    query = query.where(db.raw('timestamp <= ?', [endTime]))
  }
  let select
  if (resolution) {
    query = query.groupBy(db.raw('date_trunc(?, timestamp)', [resolution]))
    select = db.raw(keys.map(key => `max("${key}") as "${key}"`).join(', '))
  } else {
    select = keys
  }
  const readings = await query.select(select)
  readings.forEach((reading) => {
    reading.type = meter.type
  })
  return readings
}

function objectsToArrays (keys, objects) {
  const arrays = objects.map(obj => keys.map(key => obj[key]))
  arrays.unshift(keys.slice())
  return arrays
}

module.exports = {
  create: async function (reading) {
    const table = TABLES[reading.type]
    const keys = KEYS[reading.type]
    if (!table || !keys) {
      throw new Error(`Unknown reading type "${reading.type}"`)
    }
    await createOrIgnore(table, keys, reading)
  },

  getForMeter: async function (meter, params = {}) {
    const table = TABLES[meter.type]
    const keys = KEYS[meter.type]
    if (!table || !keys) {
      throw new Error(`Unknown meter type "${meter.type}"`)
    }
    return getForMeter(table, keys, meter, params)
  },

  getForMeterAsArray: async function (meter, params = {}) {
    const table = TABLES[meter.type]
    const keys = KEYS[meter.type]
    if (!table || !keys) {
      throw new Error(`Unknown meter type "${meter.type}"`)
    }
    return objectsToArrays(keys, await getForMeter(table, keys, meter, params))
  }
}
