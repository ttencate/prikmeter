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

print("Startup will resume in 3 seconds. To abort, run file.remove(\"init.lua\") on the serial console.")
tmr.create():alarm(3000, tmr.ALARM_SINGLE, startup)
