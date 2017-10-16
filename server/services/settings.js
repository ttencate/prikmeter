const db = require('../core/db')

module.exports = {
  getValue: async function (key) {
    const setting = await db.from('settings').where({ key: key }).first('value')
    return setting ? setting.value : undefined
  },

  setValue: async function (key, value) {
    await db.transaction(async function (trx) {
      const present = !!await trx.from('settings').where({ key }).first([])
      if (present) {
        await trx('settings').where({ key }).update({ value })
      } else {
        await trx('settings').insert({ key, value })
      }
    })
  }
}
