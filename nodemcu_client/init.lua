-- load credentials, 'SSID' and 'PASSWORD' declared and initialize in there
dofile("config.lua")

function startup()
    if file.open("init.lua") == nil then
        print("init.lua deleted or renamed")
    else
        print("Running")
        file.close("init.lua")
        -- the actual application is stored in 'main.lua'
        dofile("main.lua")
    end
end

-- Define WiFi station event callbacks 
wifi_connect_event = function(T) 
  print("Connected to access point")
  print("Waiting for IP address...")
end

wifi_got_ip_event = function(T) 
  -- Note: Having an IP address does not mean there is internet access!
  -- Internet connectivity can be determined with net.dns.resolve().    
  print("IP address obtained")
  print("Startup will resume in 3 seconds. To abort, run file.remove(\"init.lua\") on the serial console.")
  tmr.create():alarm(3000, tmr.ALARM_SINGLE, startup)
end

wifi_disconnect_event = function(T)
  if T.reason == wifi.eventmon.reason.ASSOC_LEAVE then 
    return 
  end

  print("Wifi disconnected unexpectedly")
  for key, val in pairs(wifi.eventmon.reason) do
    if T.reason == val then
      print("Disconnect reason: " .. val .. " (" .. key .. ")")
      break
    end
  end

  print("Retrying connection...")
end

wifi.eventmon.register(wifi.eventmon.STA_CONNECTED, wifi_connect_event)
wifi.eventmon.register(wifi.eventmon.STA_GOT_IP, wifi_got_ip_event)
wifi.eventmon.register(wifi.eventmon.STA_DISCONNECTED, wifi_disconnect_event)

print("Connecting to WiFi access point...")
wifi.setmode(wifi.STATION)
wifi.sta.config({ssid = SSID, pwd = PASSWORD})
