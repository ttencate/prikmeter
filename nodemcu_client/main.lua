on_wifi_connected = function(e) 
  print("Connected to access point")
  print("Waiting for IP address...")
end

on_wifi_got_ip = function(e) 
  print("IP address obtained")
end

on_wifi_disconnected = function(e)
  if e.reason == wifi.eventmon.reason.ASSOC_LEAVE then 
    return 
  end

  print("Wifi disconnected unexpectedly")
  for key, val in pairs(wifi.eventmon.reason) do
    if e.reason == val then
      print("Disconnect reason: " .. val .. " (" .. key .. ")")
      break
    end
  end

  print("Retrying connection...")
end

wifi.eventmon.register(wifi.eventmon.STA_CONNECTED, on_wifi_connected)
wifi.eventmon.register(wifi.eventmon.STA_GOT_IP, on_wifi_got_ip)
wifi.eventmon.register(wifi.eventmon.STA_DISCONNECTED, on_wifi_disconnected)

print("Connecting to WiFi access point...")
wifi.setmode(wifi.STATION)
wifi.sta.config({ssid = SSID, pwd = PASSWORD, auto = true})
