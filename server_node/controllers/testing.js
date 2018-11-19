const nodeMocksHttp = require('node-mocks-http')

async function simulateRequest (controller, reqObj) {
  const req = nodeMocksHttp.createRequest(reqObj)
  const res = nodeMocksHttp.createResponse()
  await controller(req, res)
  return res
}

module.exports = {
  simulateRequest
}
