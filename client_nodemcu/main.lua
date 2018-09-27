-- load credentials, 'SSID' and 'PASSWORD' declared and initialize in there
dofile('config.lua')

USER_AGENT = 'prikmeter'
VERSION = '1.0.0'

upload_telegram = function(telegram)
  local socket = tls.createConnection()
  socket:on('connection', function(socket)
    print('Socket connected, uploading telegram')
    socket:send(
      'POST /telegrams HTTP/1.1\r\n' ..
      'Host: ' .. SERVER_HOST .. '\r\n' ..
      'User-Agent: ' .. USER_AGENT .. ' ' .. VERSION .. '\r\n' ..
      'Content-Type: text/plain\r\n' ..
      'Content-Length: ' .. string.len(telegram) .. '\r\n' ..
      'Connection: close\r\n' ..
      '\r\n' ..
      telegram)
  end)
  socket:on('sent', function(socket)
    print('Data sent')
  end)
  socket:on('receive', function(socket, data)
    print(data)
  end)
  socket:on('disconnection', function(socket, reason)
    print('Socket disconnected: ' .. (reason or 'nil'))
  end)
  socket:on('reconnection', function(socket, reason)
    print('Socket reconnected: ' .. (reason or 'nil'))
  end)
  socket:connect(443, SERVER_HOST)
end

sync_clock = function()
  print('Syncing clock to NTP server')
  sntp.sync({'pool.ntp.org', 'time.nist.gov'},
    function(seconds, microseconds, server, info)
      print('Got NTP sync from ' .. server .. ': ' .. seconds .. 's' .. microseconds .. 'us')
      upload_telegram('Hello world!')
    end,
    function(code, details)
      local messages = {[1] = 'DNS lookup failed', [2] = 'memory allocation failed', [3] = 'UDP send failed', [4] = 'timed out waiting for response'}
      print('NTP sync failed: ' .. (messages[code] or code))
    end,
    true) -- autorepeat
end

on_wifi_connected = function(e)
  print('Wifi connected to ' .. e.SSID .. ', waiting for IP address')
end

on_wifi_got_ip = function(e)
  print('Received IP address ' .. e.IP)
  sync_clock()
end

on_wifi_disconnected = function(e)
  if e.reason == wifi.eventmon.reason.ASSOC_LEAVE then
    return
  end

  local reason = 'unknown reason'
  for key, val in pairs(wifi.eventmon.reason) do
    if e.reason == val then
      reason = val .. ' (' .. key .. ')'
      break
    end
  end
  print('Wifi disconnected unexpectedly: ' .. reason)

  print('Retrying connection...')
end

-- print('Enabling TLS certificate verification')
-- local result = tls.cert.verify(CA_CERTIFICATES)
-- if result ~= true then
--   print('Failed to load certificate: ' .. (result or 'nil'))
-- end
-- result = tls.cert.verify(true)
-- if result ~= true then
--   print('Failed to enable certificate verification: ' .. (result or 'nil'))
-- end

wifi.eventmon.register(wifi.eventmon.STA_CONNECTED, on_wifi_connected)
wifi.eventmon.register(wifi.eventmon.STA_GOT_IP, on_wifi_got_ip)
wifi.eventmon.register(wifi.eventmon.STA_DISCONNECTED, on_wifi_disconnected)

print('Connecting to wifi access point')
wifi.setmode(wifi.STATION)
wifi.sta.config({ssid = SSID, pwd = PASSWORD, auto = true})
