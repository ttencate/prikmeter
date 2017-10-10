const bodyParser = require('body-parser')

module.exports.create = [
  bodyParser.raw({ type: 'text/plain' }), // Populates req.body as a Buffer.
  function (req, res) {
    const telegram = req.body.toString()
    console.log(telegram)

    res.sendStatus(200)
  }
]
