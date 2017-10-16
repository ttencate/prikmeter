const db = require('../core/db')

module.exports = {
  createElectricity: async function (reading) {
    const object = {}
    for (key of ['meterId', 'timestamp', 'totalConsumptionKwhLow', 'totalConsumptionKwhHigh', 'totalProductionKwhLow', 'totalProductionKwhHigh', 'currentConsumptionKw', 'currentProductionKw']) {
      object[key] = reading[key] || null
    }
    await db('electricityReadings').insert(object)
  },

  createGas: async function (reading) {
    const object = {}
    for (key of ['meterId', 'timestamp', 'totalConsumptionM3']) {
      object[key] = reading[key] || null
    }
    await db('gasReadings').insert(object)
  },

  getElectricityForMeter: async function (meter) {
    const readings = await db.from('electricityReadings').where({ meterId: meter.id }).select('meterId', 'timestamp', 'totalConsumptionKwhLow', 'totalConsumptionKwhHigh', 'totalProductionKwhLow', 'totalProductionKwhHigh', 'currentConsumptionKw', 'currentProductionKw')
    return readings
  },

  getGasForMeter: async function (meter) {
    const readings = await db.from('gasReadings').where({ meterId: meter.id }).select('meterId', 'timestamp', 'totalConsumptionM3')
    return readings
  }
}
