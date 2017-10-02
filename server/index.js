const bodyParser = require('body-parser')
const express = require('express')

const app = express()

// Populates req.body as a Buffer.
app.use(bodyParser.raw({ type: 'text/plain' }))

app.post('/telegrams', function (req, res) {
  const telegram = req.body.toString()
  console.log(telegram)

  res.sendStatus(200)
})

app.listen(3000, '127.0.0.1')
