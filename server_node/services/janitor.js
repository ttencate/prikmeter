const telegrams = require('./telegrams')

async function cleanUpTelegrams () {
  await telegrams.cleanUp()
}

function start () {
  cleanUpTelegrams()
  setInterval(cleanUpTelegrams, 60 * 60 * 1000)
}

module.exports = {
  start
}
